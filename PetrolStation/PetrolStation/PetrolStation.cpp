#include <windows.h>
#include <math.h>
#include <string>
#include <vector>
#include <sstream>
#include "../math/math.h"
#include <objidl.h>
#include <gdiplus.h> 
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
Brush *g_redBrush; // �귯�� ��ü.
Brush *g_blueBrush; // �귯�� ��ü.
Brush *g_brush; // �귯�� ��ü.
Bitmap *g_bg;
Image *g_image;
Image *g_carImage;
HWND g_hWnd = NULL;

int g_incT = 0;
int frameT = 0;
int frame = 0;
wstring frameStr;

struct sPath
{
	int type; // 0:move, 1:stop
	Vector3 pos;
};

vector<sPath> g_Path;
struct sCar
{
	bool isStop;
	int waitTime;
	int curPathIdx;
	Vector3 pos;
	float speed; // �ʴ� �ȼ� �̵��ӵ�.
};
sCar g_car;


// �ݹ� ���ν��� �Լ� ������ Ÿ��
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, 
	WPARAM wParam, LPARAM lParam );
void InitGdiPlus(HWND hWnd);
void ReleaseGdiPlus();
void DrawString( Graphics *graph, int x, int y, const wstring &str);
void MainLoop(int elapseT);
void MoveCar(int elapseT);


int APIENTRY WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	wchar_t className[32] = L"Petrol Station";
	wchar_t windowName[32] = L"Petrol Station";

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

	g_hWnd = hWnd;

	//�����츦 ��Ȯ�� �۾����� ũ��� �����
	RECT rcClient = { 0, 0, 800, 600};
	AdjustWindowRect( &rcClient, WS_OVERLAPPEDWINDOW, FALSE );	//rcClient ũ�⸦ �۾� �������� �� ������ ũ�⸦ rcClient �� ���ԵǾ� ���´�.

	//������ ũ��� ������ ��ġ�� �ٲپ��ش�.
	SetWindowPos( hWnd, NULL, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, 
		SWP_NOZORDER | SWP_NOMOVE );

	// �����츦 ȭ�鿡 ����.
	ShowWindow( hWnd, nCmdShow );

	InitGdiPlus(hWnd);

	//�޽��� ����ü
	MSG msg;
	ZeroMemory( &msg, sizeof(MSG) );

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

		const int curT = GetTickCount();
		const int elapseT = curT - oldT;
		if (elapseT > 15)
		{
			oldT = curT;
			MainLoop(elapseT);
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
			Graphics *graph = Graphics::FromImage(g_bg);

			RECT cr;
			GetClientRect(hWnd, &cr);
			Rect wndSize(cr.left, cr.top, cr.right, cr.bottom);
			graph->DrawImage(g_image, wndSize);

			for (int i=0; i < (int)g_Path.size(); ++i)
			{
				graph->FillEllipse((g_Path[ i].type==0)? g_redBrush : g_blueBrush, 
					(int)g_Path[ i].pos.x, (int)g_Path[ i].pos.z, 5, 5);
			}

			const int W = 113;
			const int H = 58;
			Vector3 p1(-W/2, 0, -H/2);
			Vector3 p2(W/2, 0, -H/2);
			Vector3 p3(-W/2, 0, H/2);

			Matrix44 m;
			if ((g_car.curPathIdx >= 0) && 
				(g_car.curPathIdx+1 < (int)g_Path.size()))
			{
				Vector3 dir = g_Path[ g_car.curPathIdx+1].pos
				- g_car.pos;
				dir.Normalize();

				Quaternion q(Vector3(1,0,0), 
					Vector3(dir.x, 0, -dir.z));
				m = q.GetMatrix();
			}

			Vector3 pp1 = (p1 * m) + g_car.pos;
			Vector3 pp2 = (p2 * m) + g_car.pos;
			Vector3 pp3 = (p3 * m) + g_car.pos;


			Point pp[3];
			pp[0] = Point((int)pp1.x, (int)pp1.z);
			pp[1] = Point((int)pp2.x, (int)pp2.z);
			pp[2] = Point((int)pp3.x, (int)pp3.z);

			graph->DrawImage(g_carImage,
				pp,
				3,
				0,0,W,H, UnitPixel);

			DrawString(graph, 50, 0, frameStr);
			g_graphics->DrawImage(g_bg, wndSize);
			EndPaint(hWnd, &ps);
		}
		break;

	case WM_LBUTTONDOWN:
		{
			Vector3 pos(LOWORD(lParam), 0, HIWORD(lParam));
			sPath path;
			path.pos = pos;
			path.type = 0;
			g_Path.push_back(path);
		}
		break;

	case WM_RBUTTONDOWN:
		{
			Vector3 pos(LOWORD(lParam), 0, HIWORD(lParam));
			sPath path;
			path.pos = pos;
			path.type = 1;
			g_Path.push_back(path);
		}
		break;

	case WM_LBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_KEYDOWN:
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
	Gdiplus::GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL); 
	g_graphics = new Graphics(hWnd);
	g_pen = new Pen(Color::Red);
	g_brush = new SolidBrush(Color::White);
	g_blackBrush = new SolidBrush(0xFF000000);
	g_redBrush = new SolidBrush(0xFFFF0000);
	g_blueBrush = new SolidBrush(0xFF0000FF);
	g_font = new Font(L"Arial", 16);
	g_bg = new Bitmap(800,600);
	g_image = Image::FromFile(L"oilbank.png");
	g_carImage = Image::FromFile(L"car.png");

	g_car.isStop = false;
	g_car.waitTime = 0;
	g_car.curPathIdx = -1;
	g_car.pos = Vector3(0,0,0);
	g_car.speed = 40;

}


// Gdiplus ����.
void ReleaseGdiPlus()
{
	delete g_image;
	delete g_font;
	delete g_pen;
	delete g_brush;
	delete g_blackBrush;
	delete g_redBrush;
	delete g_blueBrush;
	delete g_graphics;
	delete g_bg;
	delete g_carImage;
	// Shutdown Gdiplus 
	Gdiplus::GdiplusShutdown(g_gdiplusToken); 
}


void DrawString( Graphics *graph, int x, int y, const wstring &str)
{
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);

	graph->DrawString( str.c_str(), -1, g_font,
		PointF((REAL)x, (REAL)y),
		&format, g_blackBrush);
}


void MainLoop(int elapseT)
{
	MoveCar(elapseT);


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


void MoveCar(int elapseT)
{
	//if (g_car.isStop)
	//	return;

	if (2 > (int)g_Path.size())
		return;
	if ((g_car.curPathIdx+1) >= (int)g_Path.size())
		return;

	if (g_car.curPathIdx == -1)
	{
		g_car.pos = g_Path[ 0].pos;
		g_car.curPathIdx = 0;
	}

	Vector3 dir = g_Path[ g_car.curPathIdx+1].pos - g_car.pos;
	const float len = dir.Length();

	if (len < 1.9f)
	{
		// ���� ��η� �̵��Ѵ�.
		if ((g_car.curPathIdx + 1) < (int)g_Path.size())
		{
			if (g_Path[ g_car.curPathIdx+1].type == 1)
			{
				// �����ð� ���� ����Ѵ�.
				g_car.waitTime += elapseT;
				if (g_car.waitTime > 5000)
				{
					++g_car.curPathIdx;
					g_car.waitTime =0;
				}
			}
			else
			{
				++g_car.curPathIdx;
			}
		}
		else
		{
			g_car.isStop = true;
		}
	}

	dir.Normalize();
	g_car.pos += dir * g_car.speed * (elapseT * 0.001f);
}
