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


// Gdiplus ��
GdiplusStartupInput g_gdiplusStartupInput; 
ULONG_PTR g_gdiplusToken;
Graphics *g_graphics;
Font *g_font;
Pen *g_pen; // �� ��ü.
Brush *g_blackBrush; // �귯�� ��ü.
Brush *g_brush; // �귯�� ��ü.
Bitmap *g_image;
Bitmap *g_tmpImage;

Rect g_SelectR;
bool g_mouseDown = false;
Rect g_MouseR;



// �ݹ� ���ν��� �Լ� ������ Ÿ��
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
	g_pen = new Pen(Color::White);
	g_brush = new SolidBrush(Color::White);
	g_blackBrush = new SolidBrush(0xFF000000);
	g_font = new Font(L"Arial", 16);
	//g_image = Image::FromFile(L"5114.png");
	g_image = new Bitmap(L"FF10-2.png");
	g_tmpImage = new Bitmap(g_image->GetWidth(), g_image->GetHeight());
}


// Gdiplus ����.
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
						// ó�� ���� �ٽ� �˻��Ѵ�.
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


// ��׶��� �÷� ���� ������ �ִ� ��Ʈũ�⸦ �����Ѵ�.
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


	//// ��ü �������� üũ���� ���� �ȼ��� ��˻��Ѵ�.
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
	//					// ó�� ���� �ٽ� �˻��Ѵ�.
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
