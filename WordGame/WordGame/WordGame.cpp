#include <windows.h>
#include <math.h>
#include <string>
#include <vector>
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
Brush *g_whiteBrush; // �귯�� ��ü.
Brush *g_redBrush; // �귯�� ��ü.
Brush *g_brush; // �귯�� ��ü.
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
	float velocity; // �������� �ӵ�.
	wstring word;
};
std::vector<sWord> g_words;
int g_blockCount = 9;
wstring g_inputWord = L"�Է�â";


// �ݹ� ���ν��� �Լ� ������ Ÿ��
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


// Gdiplus ����.
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
		L"�׽�Ʈ1",
		L"�׽�Ʈ2",
		L"�׽�Ʈ3",
		L"�׽�Ʈ4",
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

