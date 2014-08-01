#include <windows.h>
#include <math.h>
#include <string>
#include <vector>
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
Brush *g_whiteBrush; // 브러쉬 객체.
Brush *g_redBrush; // 브러쉬 객체.
Brush *g_brush; // 브러쉬 객체.
Image *g_image;
Bitmap *g_bg;
HWND g_hWnd;
HWND g_Edit;
const UINT IDC_EDITBOX = 100;

struct sWord
{
	bool isBreak;
	Point pos;
	PointF posF;
	float velocity; // 떨어지는 속도.
	wstring word;
};
std::vector<sWord> g_words;
int g_blockCount = 9;
wstring g_inputWord = L"입력창";


// 콜백 프로시져 함수 프로토 타입
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, 
	WPARAM wParam, LPARAM lParam );
void InitGdiPlus(HWND hWnd);
void ReleaseGdiPlus();
void DrawString( Graphics *graph, int x, int y, const wstring &str);
void DrawString2( Graphics *graph, int x, int y, const wstring &str);
void MainLoop(int elapseT);
wstring GetNewWord();
void RenderBlock( Graphics *graph);


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

WNDPROC oldEditProc;
LRESULT CALLBACK subEditProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_RETURN:
			{
				WCHAR text[128];
				GetWindowText(wnd, text, 128);
				SetWindowText(wnd, L"");

				wstring str = text;
				for (int i=0; i < (int)g_words.size(); ++i)
				{
					if ((!g_words[ i].isBreak) && (str == g_words[ i].word))
					{
						//g_words[ i].isBreak = true;
						sWord w1;
						w1.isBreak = false;
						w1.posF = PointF((float)(rand()%800), 10.f);
						w1.velocity = (rand() % 4) * 0.01f;
						w1.word = GetNewWord();
						g_words[ i] = w1;
						break;
					}
				}

			}
			break;  //or return 0; if you don't want to pass it further to def proc
			//If not your key, skip to default:
		}
	default:
		return CallWindowProc(oldEditProc, wnd, msg, wParam, lParam);
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
	case WM_PAINT: // 화면이 갱신될 때 호출된다.
		{
			hdc = BeginPaint(hWnd, &ps);

			Graphics *graph = Graphics::FromImage(g_bg);

			RECT cr;
			GetClientRect(hWnd, &cr);
			Rect wndSize(cr.left, cr.top, cr.right, cr.bottom);
			graph->DrawImage(g_image, wndSize);

			for (int i=0; i < (int)g_words.size(); ++i)
			{
				if (!g_words[ i].isBreak)
				{
					DrawString( graph, (int)g_words[ i].posF.X, 
						(int)g_words[ i].posF.Y,
						g_words[ i].word.c_str());
				}
			}

			RenderBlock(graph);

			g_graphics->DrawImage(g_bg, wndSize);
			EndPaint(hWnd, &ps);
		}
		break;

	case WM_CREATE:
		{
			g_Edit = CreateWindowEx(WS_EX_CLIENTEDGE,
				L"EDIT",
				L"",
				WS_CHILD|WS_VISIBLE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,
				50,
				100,
				200,
				100,
				hWnd,
				(HMENU)IDC_EDITBOX,
				GetModuleHandle(NULL),
				NULL);
			 oldEditProc = (WNDPROC)SetWindowLongPtr(g_Edit, 
				 GWLP_WNDPROC, (LONG_PTR)subEditProc);

			SetFocus(g_Edit);
		}
		break;

	case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case IDC_EDITBOX:
				{
					WCHAR text[ 128];
					GetWindowText(g_Edit, text, 128);
					g_inputWord = text;
				}
				break;
			}
		}
		break;

	case WM_SETTEXT:
		{
			int a = 0;
		}
		break;

	case WM_LBUTTONDOWN:
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
	g_whiteBrush = new SolidBrush(0xFFFFFFFF);
	g_redBrush = new SolidBrush(0xFFFF0000);
	g_font = new Font(L"Arial", 16);
	g_image = Image::FromFile(L"bg.jpg");
	g_bg = new Bitmap(800,600);


	for (int i=0; i < 4; ++i)
	{
		sWord w1;
		w1.isBreak = false;
		w1.posF = PointF((float)(rand()%800), 10.f);
		w1.velocity = (rand() % 4) * 0.01f;
		w1.word = GetNewWord();
		g_words.push_back(w1);
	}

}


// Gdiplus 제거.
void ReleaseGdiPlus()
{
	delete g_image;
	delete g_font;
	delete g_pen;
	delete g_brush;
	delete g_blackBrush;
	delete g_whiteBrush;
	delete g_redBrush;
	delete g_bg;
	delete g_graphics;
	// Shutdown Gdiplus 
	Gdiplus::GdiplusShutdown(g_gdiplusToken); 
}


void DrawString( Graphics *graph, int x, int y, const wstring &str)
{
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);

	graph->DrawString( str.c_str(), -1, g_font,
		PointF((REAL)x, (REAL)y),
		&format, g_whiteBrush);
}

void DrawString2( Graphics *graph, int x, int y, const wstring &str)
{
	StringFormat format;
	format.SetAlignment(StringAlignmentNear);

	graph->DrawString( str.c_str(), -1, g_font,
		PointF((REAL)x, (REAL)y),
		&format, g_blackBrush);
}

void MainLoop(int elapseT)
{
	for (int i=0; i < (int)g_words.size(); ++i)
	{
		g_words[ i].posF.Y += (elapseT * g_words[ i].velocity);
		if (g_words[ i].posF.Y > 600)
		{
			g_words[ i].posF.Y = 0;
			g_words[ i].posF.X = (float)(rand() % 800);
			g_words[ i].word = GetNewWord();

			if (!g_words[ i].isBreak)
				--g_blockCount;

			if (g_blockCount<0)
				g_blockCount = 0;
		}
	}

	::InvalidateRect(g_hWnd, NULL, FALSE);
}


wstring GetNewWord()
{
	wstring words[] = 
	{
		L"테스트1",
		L"테스트2",
		L"테스트3",
		L"테스트4",
	};

	wstring ret = words[ rand()%4];	
	return ret;
}


void RenderBlock( Graphics *graph)
{
	int blockW = 60;
	int blockH = 20;
	Point offset(300,530);

	graph->FillRectangle(g_whiteBrush, 
		offset.X, offset.Y-20,
		blockW*3, blockH);
	DrawString2(graph, offset.X, offset.Y-20, g_inputWord);

	for (int i=0; i < g_blockCount; ++i)
	{
		int x = blockW * (i % 3);
		int y = blockH * (i / 3);
		graph->FillRectangle(g_whiteBrush, 
			x + offset.X, y + offset.Y,
			blockW, blockH);
		graph->DrawRectangle(g_pen, 
			x + offset.X, y + offset.Y,
			blockW, blockH);
	}

	for (int i=g_blockCount; i < 9; ++i)
	{
		int x = blockW * (i % 3);
		int y = blockH * (i / 3);
		graph->FillRectangle(g_redBrush, 
			x + offset.X, y + offset.Y,
			blockW, blockH);
		graph->DrawRectangle(g_pen, 
			x + offset.X, y + offset.Y,
			blockW, blockH);
	}
}

