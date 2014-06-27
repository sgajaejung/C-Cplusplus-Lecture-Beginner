#include <windows.h>
#include <math.h>
#include <string>
using namespace std;
bool isClick = false;
int click_mouse_x;
int click_mouse_y;
int mouse_x;
int mouse_y;
int paint_x;
int paint_y;
int state = 0;
int state2 = 0;
HINSTANCE g_hInstance;


// 콜백 프로시져 함수 프로토 타입
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, 
	WPARAM wParam, LPARAM lParam );
void Render(HDC hdc);
bool PointInRect( RECT r, int x, int y );


int APIENTRY WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	g_hInstance = hInstance;
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
	RECT rcClient = { 0, 0, 800, 600};
	AdjustWindowRect( &rcClient, WS_OVERLAPPEDWINDOW, FALSE );	//rcClient 크기를 작업 영영으로 할 윈도우 크기를 rcClient 에 대입되어 나온다.

	//윈도우 크기와 윈도우 위치를 바꾸어준다.
	SetWindowPos( hWnd, NULL, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, 
		SWP_NOZORDER | SWP_NOMOVE );

	// 윈도우를 화면에 띄운다.
	ShowWindow( hWnd, nCmdShow );

	//메시지 구조체
	MSG msg;
	ZeroMemory( &msg, sizeof(MSG) );

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
	case WM_MOUSEWHEEL:
		break;

	case WM_PAINT: // 화면이 갱신될 때 호출된다.
		{
			hdc = BeginPaint(hWnd, &ps);

			string str;
			TextOutA(hdc, 100, 100, str.c_str(), str.length());

			Rectangle(hdc, 100, 30, 150, 60);
			TextOutA(hdc, 100, 30, "Pen", 3);
			
			Rectangle(hdc, 200, 30, 250, 60);
			TextOutA(hdc, 200, 30, "Paint", 5);

			COLORREF rgb = GetPixel(hdc, 100, 100);

			if (state2 == 0)
			{
				static int sx = paint_x;
				static int sy = paint_y;
				MoveToEx(hdc, sx, sy, NULL);
				LineTo(hdc, paint_x, paint_y);
				sx = paint_x;
				sy = paint_y;
			}

			EndPaint(hWnd, &ps);
		}
		break;

//	case WM_ERASEBKGND:
//		return 0;

	case WM_LBUTTONDOWN:
		{
			POINT pos;
			pos.x = LOWORD(lParam);
			pos.y = HIWORD(lParam);

			RECT r1 = {100, 30, 150, 60};
			RECT r2 = {200, 30, 250, 60};

			if (PtInRect(&r1,pos))
			{
				state2 = 0;
			}

			if (PtInRect(&r2,pos))
			{
				state2 = 1;
			}

			isClick = true;
			if (state == 0)
			{
				click_mouse_x = LOWORD(lParam);
				click_mouse_y = HIWORD(lParam);
			}
			else if (state == 1)
			{
				mouse_x = LOWORD(lParam);
				mouse_y = HIWORD(lParam);
			}
			++state;
		}		
		break;
	case WM_LBUTTONUP:
		{
			isClick = false;
		}		
		break;
	case WM_MOUSEMOVE:
		{
			paint_x = LOWORD(lParam);
			paint_y = HIWORD(lParam);
			::InvalidateRect(hWnd, NULL, FALSE);			
		}
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			break;
		case 'A':
			break;
		}
		break;

	case WM_DESTROY: //윈도우가 파괴된다면..
		PostQuitMessage(0);	//프로그램 종료 요청 ( 메시지 루프를 빠져나가게 된다 )
		break;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam ); // 기본적인 메세지 처리를 담당한다.
}

void Render(HDC hdc)
{
	const int w = 30;
	for (int x=0; x < 10; ++x)
	{
		for (int y=0; y < 10; ++y)
		{
			const int left = x*w;
			const int top = y*w;
			const RECT r = {left, top, left+w, top+w};

			Rectangle(hdc, r.left, r.top, r.right, r.bottom);
		}
	}

}

bool PointInRect( RECT r, int x, int y )
{
	return ((r.left < x) && 
		(r.right > x) && 
		(r.top < y) &&
		(r.bottom > y));
}
