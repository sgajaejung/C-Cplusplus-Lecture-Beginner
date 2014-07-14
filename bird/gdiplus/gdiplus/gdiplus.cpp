
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


// Gdiplus 용
GdiplusStartupInput g_gdiplusStartupInput; 
ULONG_PTR g_gdiplusToken;
Graphics *g_graphics;
Font *g_font;
Pen *g_pen; // 펜 객체.
Brush *g_blackBrush; // 브러쉬 객체.
Brush *g_brush; // 브러쉬 객체.
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


// 콜백 프로시져 함수 프로토 타입
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

	//윈도우 클레스 정보 생성
	//내가 이러한 윈도를 만들겠다 라는 정보
	WNDCLASS WndClass;
	WndClass.cbClsExtra = 0;			//윈도우에서 사용하는 여분의 메모리설정( 그냥 0 이다  신경쓰지말자 )
	WndClass.cbWndExtra = 0;			//윈도우에서 사용하는 여분의 메모리설정( 그냥 0 이다  신경쓰지말자 )
	WndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);		//윈도우 배경색상
	WndClass.hCursor = LoadCursor( NULL, IDC_ARROW );			//윈도우의 커서모양 결정
	WndClass.hIcon = LoadIcon( NULL, IDI_APPLICATION );		//윈도우아이콘모양 결정
	WndClass.hInstance = hInstance;				//프로그램인스턴스핸들 
	WndClass.lpfnWndProc = (WNDPROC)WndProc;			//윈도우 프로시져 함수 포인터
	WndClass.lpszMenuName = NULL;						//메뉴이름 없으면 NULL
	WndClass.lpszClassName = className;				//지금 작성하고 있는 윈도우 클레스의 이름
	WndClass.style	 = CS_HREDRAW | CS_VREDRAW;	//윈도우 그리기 방식 설정 ( 사이즈가 변경될때 화면갱신 CS_HREDRAW | CS_VREDRAW )

	//위에서 작성한 윈도우 클레스정보 등록
	RegisterClass( &WndClass );

	//윈도우 생성
	//생성된 윈도우 핸들을 전역변수 g_hWnd 가 받는다.
	HWND hWnd = CreateWindow(
		className,				//생성되는 윈도우의 클래스이름
		windowName,				//윈도우 타이틀바에 출력되는 이름
		WS_OVERLAPPEDWINDOW,	//윈도우 스타일 WS_OVERLAPPEDWINDOW
		0,				//윈도우 시작 위치 X 
		0,				//윈도우 시작 위치 Y
		800,				//윈도우 가로 크기 ( 작업영역의 크기가 아님 )
		600,				//윈도우 세로 크기 ( 작업영역의 크기가 아님 )
		GetDesktopWindow(),		//부모 윈도우 핸들 ( 프로그램에서 최상위 윈도우면 NULL 또는 GetDesktopWindow() )
		NULL,					//메뉴 ID ( 자신의 컨트롤 객체의 윈도우인경우 컨트롤 ID 가 된	
		hInstance,				//이 윈도우가 물릴 프로그램 인스턴스 핸들
		NULL					//추가 정보 NULL ( 신경끄자 )
		);

	//윈도우를 정확한 작업영역 크기로 맞춘다
	RECT rcClient = { 0, 0, 1024, 640};
	AdjustWindowRect( &rcClient, WS_OVERLAPPEDWINDOW, FALSE );	//rcClient 크기를 작업 영영으로 할 윈도우 크기를 rcClient 에 대입되어 나온다.

	//윈도우 크기와 윈도우 위치를 바꾸어준다.
	SetWindowPos( hWnd, NULL, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, 
		SWP_NOZORDER | SWP_NOMOVE );

	g_hWnd = hWnd;

	// 윈도우를 화면에 띄운다.
	ShowWindow( hWnd, nCmdShow );

	InitGdiPlus(hWnd);

	SetTimer(hWnd, 1, 5000, NULL);
	SetTimer(hWnd, 2, 1000, NULL);

	srand( (int)time(NULL) );


	//메시지 구조체
	MSG msg;
	ZeroMemory( &msg, sizeof(MSG) );
/*
	while (msg.message != WM_QUIT)
	{
		//PeekMessage 는 메시지 큐에 메시지가 없어도 프로그램이 멈추기 않고 진행이 된다.
		//이때 메시지큐에 메시지가 없으면 false 가 리턴되고 메시지가 있으면 true 가 리턴이된다.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage( &msg ); //눌린 키보드 의 문자를 번역하여 WM_CHAR 메시지를 발생시킨다.
			DispatchMessage( &msg );  //받아온 메시지 정보로 윈도우 프로시져 함수를 실행시킨다.
		}
	}
*/
	int oldT = GetTickCount();
	while (msg.message != WM_QUIT)
	{
		//PeekMessage 는 메시지 큐에 메시지가 없어도 프로그램이 멈추기 않고 진행이 된다.
		//이때 메시지큐에 메시지가 없으면 false 가 리턴되고 메시지가 있으면 true 가 리턴이된다.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage( &msg ); //눌린 키보드 의 문자를 번역하여 WM_CHAR 메시지를 발생시킨다.
			DispatchMessage( &msg );  //받아온 메시지 정보로 윈도우 프로시져 함수를 실행시킨다.
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
// 윈도우 프로시져 함수 ( 메시지 큐에서 받아온 메시지를 처리한다 )
//
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (msg)
	{
	case WM_PAINT: // 화면이 갱신될 때 호출된다.
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

	case WM_DESTROY: //윈도우가 파괴된다면..
		PostQuitMessage(0);	//프로그램 종료 요청 ( 메시지 루프를 빠져나가게 된다 )
		break;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam ); // 기본적인 메세지 처리를 담당한다.
}


// Gdiplus 초기화.
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


// Gdiplus 제거.
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
