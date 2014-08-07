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


// Gdiplus 용
GdiplusStartupInput g_gdiplusStartupInput; 
ULONG_PTR g_gdiplusToken;
Graphics *g_graphics;
Font *g_font;
Pen *g_pen; // 펜 객체.
Brush *g_blackBrush; // 브러쉬 객체.
Brush *g_redBrush; // 브러쉬 객체.
Brush *g_blueBrush; // 브러쉬 객체.
Brush *g_brush; // 브러쉬 객체.
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
	float speed; // 초당 픽셀 이동속도.
};
sCar g_car;


// 콜백 프로시져 함수 프로토 타입
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

	g_hWnd = hWnd;

	//윈도우를 정확한 작업영역 크기로 맞춘다
	RECT rcClient = { 0, 0, 800, 600};
	AdjustWindowRect( &rcClient, WS_OVERLAPPEDWINDOW, FALSE );	//rcClient 크기를 작업 영영으로 할 윈도우 크기를 rcClient 에 대입되어 나온다.

	//윈도우 크기와 윈도우 위치를 바꾸어준다.
	SetWindowPos( hWnd, NULL, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, 
		SWP_NOZORDER | SWP_NOMOVE );

	// 윈도우를 화면에 띄운다.
	ShowWindow( hWnd, nCmdShow );

	InitGdiPlus(hWnd);

	//메시지 구조체
	MSG msg;
	ZeroMemory( &msg, sizeof(MSG) );

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


// Gdiplus 제거.
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
		// 다음 경로로 이동한다.
		if ((g_car.curPathIdx + 1) < (int)g_Path.size())
		{
			if (g_Path[ g_car.curPathIdx+1].type == 1)
			{
				// 일정시간 동안 대기한다.
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
