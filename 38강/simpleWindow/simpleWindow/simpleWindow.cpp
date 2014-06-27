#include <windows.h>
#include <math.h>
bool isClick = false;
int click_mouse_x;
int click_mouse_y;
int mouse_x;
int mouse_y;
int paint_x;
int paint_y;
int state = 0;

struct Vector
{
	float x,y;
};

float Magnitude(Vector v)
{
	return sqrt( (v.x*v.x + v.y*v.y) );
}

Vector Normalize(Vector v)
{
	const float len = Magnitude(v);
	Vector r;
	r.x = v.x / len;
	r.y = v.y / len;
	return r;
}

Vector Multi(Vector v, float a)
{
	Vector r;
	r.x = v.x * a;
	r.y = v.y * a;
	return r;
}


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
/*
			RECT r = {100, 100, 140, 140};
			Rectangle(hdc, r.left, r.top, r.right, r.bottom);

			if (PointInRect(r, mouse_x, mouse_y))
			{
				TextOutA(hdc, 10, 10, "ssss", 4);
			}
*/
//			Rectangle(hdc, r.left, r.top, r.right, r.bottom);
//			Ellipse(hdc, r.left+100, r.top+200, r.right+100, r.bottom+200);

//			MoveToEx(hdc, 100, 100, NULL);
//			LineTo(hdc, mouse_x, mouse_y);
//			EndPaint(hWnd, &ps);
//			Render(hdc);

/*
			if (state >= 2)
			{
				int pos1_x = click_mouse_x;
				int pos1_y = click_mouse_y;
				int pos2_x = mouse_x;
				int pos2_y = mouse_y;
				float slop = (float)(pos2_y - pos1_y) / 
					(float)(pos2_x - pos1_x);
				const int inc = (pos2_x > pos1_x)? 1 : -1;
				for (int i=0; abs(i) < abs(pos2_x-pos1_x); i += inc)
				{
					int x = i + pos1_x;
					int y = (int)(slop*i + pos1_y);
					SetPixel(hdc, x, y, RGB(255,0,0));
				}
			}
*/

			const int maxCnt = 20;
			int cnt=0;
			while (cnt < maxCnt)
			{
				const int x = rand() % 30 + paint_x - 15;
				const int y = rand() % 30 + paint_y - 15;
				SetPixel(hdc, x, y, RGB(255,0,0));
				++cnt;
			}
/**/
			
/*
			if (isClick)
			{
				RECT dragR = {click_mouse_x, click_mouse_y,
					mouse_x, mouse_y};
				Rectangle(hdc, dragR.left, dragR.top,
					dragR.right, dragR.bottom);
			}
*/
/*
			Vector p0;
			p0.x = 100;
			p0.y = 100;
			Vector p1;
			p1.x = 400;
			p1.y = 200;

			Vector v;
			v.x = p1.x - p0.x;
			v.y = p1.y - p0.y;
			const float len = Magnitude(v);
			Vector r = Normalize(v);

			for (int i=0; i < len; ++i)
			{
				Vector p = Multi(r,i);
				Vector dest;
				dest.x = p0.x + p.x;
				dest.y = p0.y + p.y;
				SetPixel(hdc, dest.x, dest.y, RGB(255,0,0));
			}
*/
			COLORREF rgb = GetPixel(hdc, 100, 100);
			if (rgb == RGB(100,100,100))
			{

			}


			EndPaint(hWnd, &ps);
		}
		break;

//	case WM_ERASEBKGND:
//		return 0;

	case WM_LBUTTONDOWN:
		{
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
