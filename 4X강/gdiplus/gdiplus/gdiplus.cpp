
#include <windows.h>
#include <math.h>
#include <string>
#include <vector>
#include <objidl.h>
#include <gdiplus.h> 
#include <sstream>
#include <time.h>
#pragma comment( lib, "gdiplus.lib" ) 
using namespace std;
using namespace Gdiplus;


// Gdiplus ��
GdiplusStartupInput g_gdiplusStartupInput; 
ULONG_PTR g_gdiplusToken;
Graphics *g_graphics;
Font *g_font;
Pen *g_pen; // �� ��ü.
Brush *g_blackBrush; // �귯�� ��ü.
Brush *g_brush; // �귯�� ��ü.
Brush *g_yellowBrush;
Image *g_image;
Image *g_image2;
Image *g_image3;
Bitmap *g_bmp;
HWND g_hWnd;

Point g_mousePos;
Point g_mouseClickPos;
bool g_IsClick = false;
int g_hatchStyle = 0;

Rect g_bullet(300, 400, 120, 120);
Rect g_bulletSrc;
Rect g_block(800, 300, 150, 1000);
Rect g_block2(1300, 0, 150, 400);
Rect g_explosion(0, 0, 64, 64);
bool g_isCollision = false;

int g_incT = 0;
int frameT = 0;
int frame = 0;
wstring frameStr;


// �ݹ� ���ν��� �Լ� ������ Ÿ��
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, 
	WPARAM wParam, LPARAM lParam );
void InitGdiPlus(HWND hWnd);
void ReleaseGdiPlus();
void DrawString( Graphics *graphics, int x, int y, const wstring &str);
void MainLoop(int elapseT);
Rect GetAnimationRect(int elapseT);
Rect GetAnimationRect2(int elapseT);



int APIENTRY WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	wchar_t className[32] = L"SimpleWindow";
	wchar_t windowName[32] = L"SimpleWindow";

	//������ Ŭ���� ���� ����
	//���� �̷��� ������ ����ڴ� ��� ����
	WNDCLASS WndClass;
	WndClass.cbClsExtra = 0;			//�����쿡�� ����ϴ� ������ �޸𸮼���( �׳� 0 �̴�  �Ű澲������ )
	WndClass.cbWndExtra = 0;			//�����쿡�� ����ϴ� ������ �޸𸮼���( �׳� 0 �̴�  �Ű澲������ )
	WndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);		//������ ������
	WndClass.hCursor = LoadCursor( NULL, IDC_ARROW );			//�������� Ŀ����� ����
	WndClass.hIcon = LoadIcon( NULL, IDI_APPLICATION );		//����������ܸ�� ����
	WndClass.hInstance = hInstance;				//���α׷��ν��Ͻ��ڵ� 
	WndClass.lpfnWndProc = (WNDPROC)WndProc;			//������ ���ν��� �Լ� ������
	WndClass.lpszMenuName = NULL;						//�޴��̸� ������ NULL
	WndClass.lpszClassName = className;				//���� �ۼ��ϰ� �ִ� ������ Ŭ������ �̸�
	WndClass.style	 = CS_HREDRAW | CS_VREDRAW;	//������ �׸��� ��� ���� ( ����� ����ɶ� ȭ�鰻�� CS_HREDRAW | CS_VREDRAW )

	//������ �ۼ��� ������ Ŭ�������� ���
	RegisterClass( &WndClass );

	//������ ����
	//������ ������ �ڵ��� �������� g_hWnd �� �޴´�.
	HWND hWnd = CreateWindow(
		className,				//�����Ǵ� �������� Ŭ�����̸�
		windowName,				//������ Ÿ��Ʋ�ٿ� ��µǴ� �̸�
		WS_OVERLAPPEDWINDOW,	//������ ��Ÿ�� WS_OVERLAPPEDWINDOW
		0,				//������ ���� ��ġ X 
		0,				//������ ���� ��ġ Y
		800,				//������ ���� ũ�� ( �۾������� ũ�Ⱑ �ƴ� )
		600,				//������ ���� ũ�� ( �۾������� ũ�Ⱑ �ƴ� )
		GetDesktopWindow(),		//�θ� ������ �ڵ� ( ���α׷����� �ֻ��� ������� NULL �Ǵ� GetDesktopWindow() )
		NULL,					//�޴� ID ( �ڽ��� ��Ʈ�� ��ü�� �������ΰ�� ��Ʈ�� ID �� ��	
		hInstance,				//�� �����찡 ���� ���α׷� �ν��Ͻ� �ڵ�
		NULL					//�߰� ���� NULL ( �Ű���� )
		);

	//�����츦 ��Ȯ�� �۾����� ũ��� �����
	RECT rcClient = { 0, 0, 1024, 640};
	AdjustWindowRect( &rcClient, WS_OVERLAPPEDWINDOW, FALSE );	//rcClient ũ�⸦ �۾� �������� �� ������ ũ�⸦ rcClient �� ���ԵǾ� ���´�.

	//������ ũ��� ������ ��ġ�� �ٲپ��ش�.
	SetWindowPos( hWnd, NULL, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, 
		SWP_NOZORDER | SWP_NOMOVE );

	g_hWnd = hWnd;

	// �����츦 ȭ�鿡 ����.
	ShowWindow( hWnd, nCmdShow );

	InitGdiPlus(hWnd);

	SetTimer(hWnd, 1, 5000, NULL);
	SetTimer(hWnd, 2, 1000, NULL);

	srand( (int)time(NULL) );


	//�޽��� ����ü
	MSG msg;
	ZeroMemory( &msg, sizeof(MSG) );
/*
	while (msg.message != WM_QUIT)
	{
		//PeekMessage �� �޽��� ť�� �޽����� ��� ���α׷��� ���߱� �ʰ� ������ �ȴ�.
		//�̶� �޽���ť�� �޽����� ������ false �� ���ϵǰ� �޽����� ������ true �� �����̵ȴ�.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage( &msg ); //���� Ű���� �� ���ڸ� �����Ͽ� WM_CHAR �޽����� �߻���Ų��.
			DispatchMessage( &msg );  //�޾ƿ� �޽��� ������ ������ ���ν��� �Լ��� �����Ų��.
		}
	}
*/
	int oldT = GetTickCount();
	while (msg.message != WM_QUIT)
	{
		//PeekMessage �� �޽��� ť�� �޽����� ��� ���α׷��� ���߱� �ʰ� ������ �ȴ�.
		//�̶� �޽���ť�� �޽����� ������ false �� ���ϵǰ� �޽����� ������ true �� �����̵ȴ�.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage( &msg ); //���� Ű���� �� ���ڸ� �����Ͽ� WM_CHAR �޽����� �߻���Ų��.
			DispatchMessage( &msg );  //�޾ƿ� �޽��� ������ ������ ���ν��� �Լ��� �����Ų��.
		}


		{
			const int curT = GetTickCount();
			const int elapseT = curT - oldT;
			if (elapseT > 15)
			{
				oldT = curT;
				MainLoop(elapseT);
			}
		}
	}

	ReleaseGdiPlus();
	return 0;
}


//
// ������ ���ν��� �Լ� ( �޽��� ť���� �޾ƿ� �޽����� ó���Ѵ� )
//
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (msg)
	{
	case WM_PAINT: // ȭ���� ���ŵ� �� ȣ��ȴ�.
		{
			hdc = BeginPaint(hWnd, &ps);

			Graphics *graph = Graphics::FromImage(g_bmp);

			RECT cr;
			GetClientRect(hWnd, &cr);
			Rect wndSize(cr.left, cr.top, cr.right, cr.bottom);

			graph->DrawImage(g_image, wndSize);

			graph->FillRectangle(g_yellowBrush, g_block);
			graph->FillRectangle(g_yellowBrush, g_block2);


			graph->DrawImage(g_image2, g_bullet, 
				g_bulletSrc.X, g_bulletSrc.Y, g_bulletSrc.Width, g_bulletSrc.Height,
				UnitPixel);


			float fBlend = 1.f; //set the alpha value
			ColorMatrix BitmapMatrix = {
				1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, fBlend, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f, 1.0f
			};

			ImageAttributes ImgAttr;
			ImgAttr.SetColorMatrix(&BitmapMatrix,
				ColorMatrixFlagsDefault,
				ColorAdjustTypeBitmap);

			//graph->SetCompositingMode(CompositingModeSourceCopy);
			if (g_isCollision)
			{
				graph->DrawImage(g_image3, g_bullet, 
					g_explosion.X, g_explosion.Y, g_explosion.Width, g_explosion.Height,
					UnitPixel, &ImgAttr);
			}

//			graph->DrawImage(g_image3, g_bullet, 
//				g_explosion.X, g_explosion.Y, g_explosion.Width, g_explosion.Height,
//				UnitPixel);


			DrawString(graph, 50, 0, frameStr);

			g_graphics->DrawImage(g_bmp, wndSize);


			EndPaint(hWnd, &ps);
		}
		break;

//	case WM_ERASEBKGND:
//		return 0;

	case WM_LBUTTONDOWN:
		{
			g_IsClick = true;
			g_mouseClickPos = Point(LOWORD(lParam), HIWORD(lParam));
		}
		break;

	case WM_LBUTTONUP:
		{
			g_IsClick = false;
		}
		break;

	case WM_MOUSEMOVE:
		{
		}
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
		case VK_UP:
			++g_hatchStyle;
			break;

		case VK_RIGHT:
		case VK_DOWN:
			--g_hatchStyle;
			break;
		}
		break;

	case WM_TIMER:
		{
			if (wParam == 1)
			{
				int a = 0;
			}
			else if (wParam == 2)
			{
				int a = 0;
			}
		}
		break;

	case WM_DESTROY: //�����찡 �ı��ȴٸ�..
		PostQuitMessage(0);	//���α׷� ���� ��û ( �޽��� ������ ���������� �ȴ� )
		break;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam ); // �⺻���� �޼��� ó���� ����Ѵ�.
}


// Gdiplus �ʱ�ȭ.
void InitGdiPlus(HWND hWnd)
{
	//Start Gdiplus 
	Gdiplus::GdiplusStartup(&g_gdiplusToken, 
		&g_gdiplusStartupInput, NULL); 
	g_graphics = new Graphics(hWnd);
	g_pen = new Pen(Color::Red);
	g_brush = new SolidBrush(Color::White);
	//g_yellowBrush = new SolidBrush(Color::Yellow);
	g_yellowBrush = new HatchBrush(HatchStyleNarrowVertical, Color::Yellow);
	g_blackBrush = new SolidBrush(0xFF000000);
	g_font = new Font(L"Arial", 16);
	//g_image = Image::FromFile(L"Kang_So_Ra9.jpg");
	g_image = Image::FromFile(L"space-wallpapers-13-610x381.jpg");
	g_image2 = Image::FromFile(L"sprite1.png");
	//g_image3 = Image::FromFile(L"explosion_opaque.png");
	g_image3 = Image::FromFile(L"explode_1.png");


	RECT cr;
	GetClientRect(hWnd, &cr);
	g_bmp = new Bitmap(cr.right-cr.left, cr.bottom-cr.top);

}


// Gdiplus ����.
void ReleaseGdiPlus()
{
	delete g_image;
	delete g_image2;
	delete g_font;
	delete g_pen;
	delete g_brush;
	delete g_yellowBrush;
	delete g_blackBrush;
	delete g_graphics;
	delete g_bmp;
	// Shutdown Gdiplus 
	Gdiplus::GdiplusShutdown(g_gdiplusToken); 
}


void DrawString( Graphics *graphics, int x, int y, const wstring &str)
{
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);

	graphics->DrawString( str.c_str(), -1, g_font,
		PointF((REAL)x, (REAL)y),
		&format, g_yellowBrush);
}


float g_X = 0;
float velY = 0;
void MainLoop(int elapseT)
{
	//g_bullet.Y += 3;
	velY += 0.01f;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		//g_bullet.Y -= 6;
		velY -= 0.05f;
	}
	g_bullet.Y += (int)(velY * elapseT);
	if (g_bullet.Y < 0)
	{
		velY = 0;
		g_bullet.Y = 0;
	}
	if (g_bullet.Y >= 550)
	{
		velY = 0;
		g_bullet.Y = 550;
	}

	
	g_X += (elapseT * 0.1f);
	//g_bullet.X = (int)g_X;
	g_bulletSrc = GetAnimationRect(elapseT);
	g_explosion = GetAnimationRect2(elapseT);


	g_block.X -= (int)(elapseT * 0.2f);
	if (g_block.X <= -200)
	{
		g_block.X = 1000;
		g_block.Y = rand()%600 + 100;
	}

	g_block2.X -= (int)(elapseT * 0.2f);
	if (g_block2.X <= -200)
	{
		g_block2.X = 1000;
		g_block2.Height = rand()%400 + 100;
	}

	g_isCollision = false;
	if (g_bullet.IntersectsWith(g_block))
		g_isCollision = true;
	if (g_bullet.IntersectsWith(g_block2))
		g_isCollision = true;	


	if (g_X > 1000)
		g_X = 0;

	++frame;
	frameT += elapseT;
	if (frameT > 1000)
	{
		std::wstringstream ss;
		ss << frame;
		frameStr = ss.str();
		frame = 0;
		frameT = 0;
	}

	::InvalidateRect(g_hWnd, NULL, FALSE);
}


Rect GetAnimationRect(int elapseT)
{
	static int t = 0;
	static int idx = 0;
	t += elapseT;

	if (t > 25)
	{
		++idx;
		if (idx >= 14)
			idx = 0;
	}

	const int width = 110;
	const int height = 101;
	
	return Rect((idx%5)*width, (idx/5)*height, width, height);
}


Rect GetAnimationRect2(int elapseT)
{
	static int t = 0;
	static int idx = 0;
	t += elapseT;

	if (t > 33)
	{
		++idx;
		if (idx >= 16)
			idx = 0;
	}

	const int width = 64;
	const int height = 64;

	return Rect((idx%4)*width, (idx/4)*height, width, height);
}
