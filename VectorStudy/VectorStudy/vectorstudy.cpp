
#include <windows.h>
#include <math.h>
#include <string>
#include <vector>
#include <objidl.h>
#include <gdiplus.h> 
#include <sstream>
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
Brush *g_brush2; // 브러쉬 객체.
vector<Brush*> g_brushes;
Image *g_image;
Bitmap *g_bg;


struct Vector2
{
	float x,y;

	Vector2() {}
	Vector2(float x0, float y0) {
		x = x0;
		y = y0;
	}

	float Length() 
	{
		return sqrt(x*x + y*y);		
	}

	Vector2 Normal()
	{
		const float len = Length();
		return Vector2(x/len, y/len);
	}

	void Normalize()
	{
		const float len = Length();
		x = x / len;
		y = y / len;
	}

	Vector2 operator+ (Vector2 rhs)
	{
		return Vector2(x + rhs.x, y + rhs.y);
	}

	Vector2 operator- (Vector2 rhs)
	{
		return Vector2(x - rhs.x, y - rhs.y);
	}

	Vector2 operator* (float rhs)
	{
		return Vector2(x * rhs, y * rhs);
	}

	Vector2 operator/ (float rhs)
	{
		return Vector2(x / rhs, y / rhs);
	}

};


Vector2 g_point[2];
int g_pointCount = 0;

bool g_IsMove = false;
Vector2 g_MovePos;
Vector2 g_MoveDirection;
HWND g_hWnd = NULL;
struct Ball
{
	bool collision;
	bool bcollision;
	Vector2 pos;
	Vector2 dir;
};
vector<Ball> g_balls;
int g_maxTimeAttack = 0;
int g_timeAttack = 0;

const float B_R = 20;
const float C_R = 15;
const int MAX_BALL_COUNT = 60;


// 콜백 프로시져 함수 프로토 타입
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, 
	WPARAM wParam, LPARAM lParam );
void InitGdiPlus(HWND hWnd);
void ReleaseGdiPlus();
void DrawString( int x, int y, const wstring &str);
void MainLoop( int elapseT );



int APIENTRY WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	srand(GetTickCount());

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

		int curT = GetTickCount();
		int elapseT = curT - oldT;
		oldT = curT;

		MainLoop(elapseT);
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

			std::wstringstream ss;
			ss << g_timeAttack;
			DrawString( 100, 30, ss.str() );

			std::wstringstream ss1;
			ss1 << g_maxTimeAttack;
			DrawString( 100, 10, ss1.str() );

			RECT cr;
			GetClientRect(hWnd, &cr);
			Rect wndSize(cr.left, cr.top, cr.right, cr.bottom);
			graph->DrawImage(g_image, wndSize);

			for (int i=0; i < g_pointCount; ++i)
			{
				graph->FillRectangle(g_blackBrush, 
					(int)g_point[i].x, (int)g_point[ i].y,
					5,5);
			}

			
			for (int i=0; i < (int)g_balls.size(); ++i)
			{
				if (g_balls[ i].collision)
				{
					graph->FillEllipse(g_blackBrush, 
						(int)g_balls[ i].pos.x-B_R, (int)g_balls[ i].pos.y-B_R,
						B_R*2,B_R*2);
				}
				else if (g_balls[ i].bcollision)
				{
					graph->FillEllipse( g_brushes[ rand()%g_brushes.size()],
						(int)g_balls[ i].pos.x-B_R, (int)g_balls[ i].pos.y-B_R,
						B_R*2,B_R*2);
				}
				else
				{
					graph->FillEllipse(g_brush, 
						(int)g_balls[ i].pos.x-B_R, (int)g_balls[ i].pos.y-B_R,
						B_R*2,B_R*2);
				}
				g_balls[ i].collision = false;
				g_balls[ i].bcollision = false;
			}

			graph->FillEllipse(g_brush2, 
				(int)g_MovePos.x-C_R, (int)g_MovePos.y-C_R,C_R*2,C_R*2);

			g_graphics->DrawImage(g_bg, wndSize);
			EndPaint(hWnd, &ps);
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if (g_pointCount < 2)
			{
				Vector2 p;
				p.x = LOWORD(lParam);
				p.y = HIWORD(lParam);

				g_point[ g_pointCount] = p;
				++g_pointCount;
			}
			::InvalidateRect(hWnd, NULL, TRUE);
		}
		break;


	case WM_LBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		{
			g_MovePos = Vector2(LOWORD(lParam), HIWORD(lParam));
		}
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
	g_brush2 = new SolidBrush(Color::Yellow);
	g_blackBrush = new SolidBrush(0xFF000000);

	g_brushes.push_back( new SolidBrush(Color::Yellow) );
	g_brushes.push_back( new SolidBrush(Color::Tomato) );
	g_brushes.push_back( new SolidBrush(Color::Yellow) );
	g_brushes.push_back( new SolidBrush(Color::Sienna) );
	g_brushes.push_back( new SolidBrush(Color::NavajoWhite) );
	g_brushes.push_back( new SolidBrush(Color::MediumTurquoise) );
	g_brushes.push_back( new SolidBrush(Color::MediumAquamarine) );
	g_brushes.push_back( new SolidBrush(Color::Lime) );
	g_brushes.push_back( new SolidBrush(Color::LightGoldenrodYellow) );
	g_brushes.push_back( new SolidBrush(Color::DarkBlue) );
	g_brushes.push_back( new SolidBrush(Color::DarkCyan             ) );
	g_brushes.push_back( new SolidBrush(Color::Beige             ) );
	g_brushes.push_back( new SolidBrush(Color::AliceBlue             ) );
	g_brushes.push_back( new SolidBrush(Color::AntiqueWhite             ) );
	g_brushes.push_back( new SolidBrush(Color::BlueViolet             ) );
	g_brushes.push_back( new SolidBrush(Color::Brown             ) );
	g_brushes.push_back( new SolidBrush(Color::DarkOliveGreen             ) );
	g_brushes.push_back( new SolidBrush(Color::DeepPink             ) );
	



	g_font = new Font(L"Arial", 16);
	g_image = Image::FromFile(L"bg.jpg");
	g_bg = new Bitmap(800,600);

	for (int i=0; i < MAX_BALL_COUNT; ++i)
	{
		Ball ball;
		ball.collision = false;
		ball.bcollision = false;
		ball.dir = Vector2(rand()%10, rand()%10);
		ball.dir.Normalize();
		ball.pos = Vector2(rand()%800, rand()%600);
		g_balls.push_back(ball);
	}

}


// Gdiplus 제거.
void ReleaseGdiPlus()
{
	delete g_image;
	delete g_font;
	delete g_pen;
	delete g_brush;
	delete g_brush2;
	delete g_blackBrush;
	delete g_graphics;
	for (int i=0; i < g_brushes.size(); ++i)
		delete g_brushes[ i];
	// Shutdown Gdiplus 
	Gdiplus::GdiplusShutdown(g_gdiplusToken); 
}


void DrawString( int x, int y, const wstring &str)
{
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);

	g_graphics->DrawString( str.c_str(), -1, g_font,
		PointF((REAL)x, (REAL)y),
		&format, g_brush);
}

void MainLoop( int elapseT )
{
	g_timeAttack += elapseT;

	for (int i=0; i < (int)g_balls.size(); ++i)
	{
		g_balls[ i].pos = g_balls[ i].pos + g_balls[ i].dir * ((float)elapseT * 0.1f);

		if ((g_balls[ i].dir.x > 0) && (g_balls[ i].pos.x > 800-B_R))
		{
			g_balls[ i].dir.x = -g_balls[ i].dir.x;
		}
		if ((g_balls[ i].dir.x < 0) && (g_balls[ i].pos.x < B_R))
		{
			g_balls[ i].dir.x = -g_balls[ i].dir.x;
		}

		if ((g_balls[ i].dir.y > 0) && (g_balls[ i].pos.y > 600-B_R))
		{
			g_balls[ i].dir.y = -g_balls[ i].dir.y;
		}
		if ((g_balls[ i].dir.y < 0) && (g_balls[ i].pos.y < B_R))
		{
			g_balls[ i].dir.y = -g_balls[ i].dir.y;
		}
	}


	for (int i=0; i < (int)g_balls.size(); ++i)
	{
		for (int k=0; k < (int)g_balls.size(); ++k)
		{
			if (i==k)
				continue;

			Vector2 p = g_balls[ i].pos - g_balls[ k].pos;
			if (p.Length() < B_R*2)
			{
				p.Normalize();
				Vector2 v1 = g_balls[ i].dir + p;
				v1.Normalize();

				g_balls[ i].dir = v1;
				g_balls[ i].bcollision = true;
			}
		}
	}

	for (int i=0; i < (int)g_balls.size(); ++i)
	{
		Vector2 p = g_balls[ i].pos - g_MovePos;
		if (p.Length() < C_R+B_R)
		{
			g_balls[ i].collision = true;

			if (g_maxTimeAttack < g_timeAttack)
				g_maxTimeAttack = g_timeAttack;
			g_timeAttack = 0;
		}
	}

	::InvalidateRect(g_hWnd, NULL, FALSE);
}
