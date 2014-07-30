#include <windows.h>
#include <math.h>
#include <string>
#include <vector>
#include <sstream>
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
Brush *g_brush; // 브러쉬 객체.
Bitmap *g_image;
Bitmap *g_tmpImage;

Rect g_SelectR;
bool g_mouseDown = false;
Rect g_MouseR;



// 콜백 프로시져 함수 프로토 타입
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, 
	WPARAM wParam, LPARAM lParam );
void InitGdiPlus(HWND hWnd);
void ReleaseGdiPlus();
void DrawString( int x, int y, const wstring &str);
Rect FindRectRange( Rect range );
Rect FindRect( Point mousePos );
Rect TrimRect( Rect range );
RECT FindRectRec( Point pos, Color bgColor, bool checkRange );


int APIENTRY WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	wchar_t className[32] = L"Sprite Tool";
	wchar_t windowName[32] = L"Sprite Tool";

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

	InitGdiPlus(hWnd);

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

			RECT cr;
			GetClientRect(hWnd, &cr);
			Rect wndSize(cr.left, cr.top, cr.right, cr.bottom);
			g_graphics->DrawImage(g_image, wndSize, 
				wndSize.X, wndSize.Y, wndSize.Width, wndSize.Height,
				UnitPixel);

//			Rect r(100, 100, 200, 200);
//			g_graphics->DrawRectangle(g_pen, r);
//			DrawString( r.X, r.Y, L"Hello" );

			g_graphics->DrawRectangle(g_pen, g_SelectR);
			g_graphics->DrawRectangle(g_pen, g_MouseR);			

			EndPaint(hWnd, &ps);
		}
		break;

	case WM_LBUTTONDOWN:
		{
			Point pos(LOWORD(lParam), HIWORD(lParam));
			g_MouseR = Rect(pos.X, pos.Y, 1, 1);
			g_mouseDown = true;
			::InvalidateRect(hWnd, NULL, TRUE);
		}
		break;

	case WM_LBUTTONUP:
		{
			Point pos(LOWORD(lParam), HIWORD(lParam));
			g_MouseR = Rect(
				min(pos.X, g_MouseR.X), 
				min(pos.Y, g_MouseR.Y),
				abs(pos.X - g_MouseR.X),
				abs(pos.Y - g_MouseR.Y));

			g_mouseDown = false;
			g_SelectR = FindRectRange(g_MouseR);
			g_MouseR = Rect(0,0,0,0);

			std::stringstream ss;
			ss << "SelectR = " << g_SelectR.X << " " << g_SelectR.Y <<
				" " << g_SelectR.Width << " " << g_SelectR.Height << endl;
			OutputDebugStringA( ss.str().c_str() );

			::InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_MOUSEMOVE:
		{
			if (g_mouseDown)
			{
				Point pos(LOWORD(lParam), HIWORD(lParam));
				g_MouseR = Rect(
					min(pos.X, g_MouseR.X), 
					min(pos.Y, g_MouseR.Y),
					abs(pos.X - g_MouseR.X),
					abs(pos.Y - g_MouseR.Y));
				::InvalidateRect(hWnd, NULL, TRUE);
			}
			else
			{
				//Point pos(LOWORD(lParam), HIWORD(lParam));
				//Color color;
				//g_image->GetPixel(pos.X, pos.Y, &color);
				//std::stringstream ss;
				//ss << color.GetValue() << endl;
				//OutputDebugStringA( ss.str().c_str() );
			}
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
	g_pen = new Pen(Color::White);
	g_brush = new SolidBrush(Color::White);
	g_blackBrush = new SolidBrush(0xFF000000);
	g_font = new Font(L"Arial", 16);
	//g_image = Image::FromFile(L"5114.png");
	g_image = new Bitmap(L"FF10-2.png");
	g_tmpImage = new Bitmap(g_image->GetWidth(), g_image->GetHeight());
}


// Gdiplus 제거.
void ReleaseGdiPlus()
{
	delete g_image;
	delete g_tmpImage;
	delete g_font;
	delete g_pen;
	delete g_brush;
	delete g_blackBrush;
	delete g_graphics;
	// Shutdown Gdiplus 
	Gdiplus::GdiplusShutdown(g_gdiplusToken); 
}


void DrawString( int x, int y, const wstring &str)
{
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);

	g_graphics->DrawString( str.c_str(), -1, g_font,
		PointF((REAL)x, (REAL)y),
		&format, g_blackBrush);
}


Rect FindRectRange( Rect range )
{
	Rect rt;
	if ((range.Width == 1)  && (range.Height == 1))
	{
		rt = FindRect( Point(range.X, range.Y));
	}
	else
	{
		Color bgColor;
		Status status = g_image->GetPixel(0, 0, &bgColor);

		Graphics graphic(g_tmpImage);
		graphic.FillRectangle(g_brush, 
			Rect(0,0,g_tmpImage->GetWidth(), g_tmpImage->GetHeight()));

		Rect tr = TrimRect( range );
		RECT rect = {tr.X, tr.Y, tr.X+tr.Width, tr.Y+tr.Height};
		RECT tmp = rect;

		for (int i=rect.left; i < rect.right; ++i)
		{
			for (int k=rect.top; k < rect.bottom; ++k)
			{
				Point pos(i, k);

				Color color, checkColor;
				g_image->GetPixel(pos.X, pos.Y, &color);
				g_tmpImage->GetPixel(pos.X, pos.Y, &checkColor);

				if ((color.GetValue() != bgColor.GetValue()) &&
					(checkColor.GetValue() == 0xFFFFFFFF) &&
					(color.GetAlpha() != 0))
				{
					RECT r = FindRectRec(pos, bgColor, false);
					RECT newR;
					newR.left = min(r.left, tmp.left);
					newR.top = min(r.top, tmp.top);
					newR.right = max(r.right, tmp.right);
					newR.bottom = max(r.bottom, tmp.bottom);

					if ((tmp.left != newR.left) ||
						(tmp.top != newR.top) ||
						(tmp.right != newR.right) ||
						(tmp.bottom != newR.bottom))
					{
						tmp = newR;
						// 처음 부터 다시 검색한다.
						rect = newR;
						i = newR.left-1;
						k = newR.top - 1;
					}
				}
			}
		}

		rt = Rect(rect.left, rect.top, 
			rect.right-rect.left, rect.bottom-rect.top);
	}

	return rt;
}


// 백그라운드 컬러 값을 제외한 최대 렉트크기를 리턴한다.
Rect TrimRect( Rect range )
{
	RECT r = {0, 0, 0, 0};
	RECT checkRange = {range.X, range.Y, 
		range.X+range.Width, range.Y+range.Height};

	Color bgColor;
	g_image->GetPixel(0, 0, &bgColor);
	
	bool checkLeft = false;
	for (int i=checkRange.left; (!checkLeft) && (i < checkRange.right); ++i)
	{
		for (int k=checkRange.top; k < checkRange.bottom; ++k)
		{
			Point pos(i, k);

			Color color;
			g_image->GetPixel(pos.X, pos.Y, &color);
			if ((color.GetValue() != bgColor.GetValue()) &&
				(color.GetAlpha() != 0))
			{
				r.left = i;
				checkLeft = true;
				break;
			}
		}
	}

	bool checkTop = false;
	for (int i=checkRange.top; (!checkTop) && (i < checkRange.bottom); ++i)
	{
		for (int k=checkRange.left; k < checkRange.right; ++k)
		{
			Point pos(k, i);

			Color color;
			g_image->GetPixel(pos.X, pos.Y, &color);
			if ((color.GetValue() != bgColor.GetValue()) &&
				(color.GetAlpha() != 0))
			{
				r.top = i;
				checkTop = true;
				break;
			}
		}
	}

	bool checkRight = false;
	for (int i=checkRange.right; (!checkRight) && (i >= checkRange.left); --i)
	{
		for (int k=checkRange.top; k < checkRange.bottom; ++k)
		{
			Point pos(i, k);

			Color color;
			g_image->GetPixel(pos.X, pos.Y, &color);
			if ((color.GetValue() != bgColor.GetValue()) &&
				(color.GetAlpha() != 0))
			{
				r.right = i;
				checkRight = true;
				break;
			}
		}
	}

	bool checkBottom = false;
	for (int i=checkRange.bottom; (!checkBottom) && (i >= checkRange.top); --i)
	{
		for (int k=checkRange.left; k < checkRange.right; ++k)
		{
			Point pos(k, i);

			Color color;
			g_image->GetPixel(pos.X, pos.Y, &color);
			if ((color.GetValue() != bgColor.GetValue()) &&
				(color.GetAlpha() != 0))
			{
				r.bottom = i;
				checkBottom = true;
				break;
			}
		}
	}

	return Rect(r.left,r.top,r.right-r.left,r.bottom-r.top);
}

Rect FindRect( Point mousePos )
{
	Graphics graphic(g_tmpImage);
	graphic.FillRectangle(g_brush, 
		Rect(0,0,g_tmpImage->GetWidth(), g_tmpImage->GetHeight()));

	Color color;
	g_image->GetPixel(0, 0, &color);
	RECT r = FindRectRec(mousePos, color, true);

	return Rect(r.left, r.top, r.right-r.left, r.bottom-r.top);
}


RECT FindRectRec( Point firstPos, Color bgColor, bool checkRange )
{
	vector<Point> q;
	q.reserve(10000);
	q.push_back(firstPos);

	RECT rect = {firstPos.X,firstPos.Y,firstPos.X,firstPos.Y};

	while (!q.empty())
	{
		Point pos = q.back();
		q.pop_back();

		if ((pos.X < 0) || (pos.Y < 0))
			continue;
		if ((pos.X >= (int)g_image->GetWidth()) || 
			(pos.Y >= (int)g_image->GetHeight()))
			continue;

		Color checkColor;
		g_tmpImage->GetPixel(pos.X, pos.Y, &checkColor);
		if (checkColor.GetValue() != 0xFFFFFFFF)
			continue;

		g_tmpImage->SetPixel(pos.X, pos.Y, Color::Black);

		Color color;
		g_image->GetPixel(pos.X, pos.Y, &color);
		if ((color.GetValue() == bgColor.GetValue()) ||
			(color.GetAlpha() == 0))
			continue;

		q.push_back(Point(pos.X-1, pos.Y));
		q.push_back(Point(pos.X, pos.Y-1));
		q.push_back(Point(pos.X+1, pos.Y));
		q.push_back(Point(pos.X, pos.Y+1));

		rect.left = min(pos.X, rect.left);
		rect.top = min(pos.Y, rect.top);
		rect.right = max(pos.X, rect.right);
		rect.bottom = max(pos.Y, rect.bottom);
	}


	//// 전체 영역에서 체크되지 않은 픽셀을 재검색한다.
	RECT tmp = rect;
	//if (checkRange)
	//{
	//	for (int i=rect.left; i < rect.right; ++i)
	//	{
	//		for (int k=rect.top; k < rect.bottom; ++k)
	//		{
	//			Point pos(i, k);

	//			Color color, checkColor;
	//			g_image->GetPixel(pos.X, pos.Y, &color);
	//			g_tmpImage->GetPixel(pos.X, pos.Y, &checkColor);

	//			if ((color.GetValue() != bgColor.GetValue()) &&
	//				(checkColor.GetValue() == 0xFFFFFFFF))
	//			{
	//				RECT r = FindRectRec(pos, bgColor, false);
	//				RECT newR;
	//				newR.left = min(r.left, tmp.left);
	//				newR.top = min(r.top, tmp.top);
	//				newR.right = max(r.right, tmp.right);
	//				newR.bottom = max(r.bottom, tmp.bottom);

	//				if ((tmp.left != newR.left) ||
	//					(tmp.top != newR.top) ||
	//					(tmp.right != newR.right) ||
	//					(tmp.bottom != newR.bottom))
	//				{
	//					tmp = newR;
	//					// 처음 부터 다시 검색한다.
	//					rect = newR;
	//					i = newR.left-1;
	//					k = newR.top - 1;
	//				}
	//			}
	//		}
	//	}
	//}

	return tmp;
}
