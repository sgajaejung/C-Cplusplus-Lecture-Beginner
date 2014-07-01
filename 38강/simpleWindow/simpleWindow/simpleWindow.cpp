#include <windows.h>
#include <math.h>
#include <string>
#include <vector>
using namespace std;
bool isClick = false;
int click_mouse_x;
int click_mouse_y;
int mouse_x;
int mouse_y;
int paint_x;
int paint_y;
int fill_x;
int fill_y;
int state = 0;
int state2 = 0;
HINSTANCE g_hInstance;
bool fillPaint = false;


// �ݹ� ���ν��� �Լ� ������ Ÿ��
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
	case WM_MOUSEWHEEL:
		break;

	case WM_PAINT: // ȭ���� ���ŵ� �� ȣ��ȴ�.
		{
			hdc = BeginPaint(hWnd, &ps);

			string str;
			TextOutA(hdc, 100, 100, str.c_str(), str.length());

			Rectangle(hdc, 100, 30, 150, 60);
			TextOutA(hdc, 100, 30, "Pen", 3);
			
			Rectangle(hdc, 200, 30, 250, 60);
			TextOutA(hdc, 200, 30, "Paint", 5);

			
			if (state2 == 0)
			{
				static int sx = paint_x;
				static int sy = paint_y;
				MoveToEx(hdc, sx, sy, NULL);
				LineTo(hdc, paint_x, paint_y);
				sx = paint_x;
				sy = paint_y;
			}


			if (fillPaint == true)
			{
				fillPaint = false;
				std::vector<POINT> array(1000000);
				array[ 0].x = fill_x;
				array[ 0].y = fill_y;
				int cnt = 1;
				COLORREF orig_color = GetPixel(hdc, fill_x, fill_y);

				while (cnt > 0)
				{
					POINT pos = array[ cnt-1];
					--cnt;
					COLORREF color = GetPixel(hdc, pos.x, pos.y);
					if (color == orig_color)
					{
						SetPixel(hdc, pos.x, pos.y, RGB(255,0,255));

						POINT env[4] = {{-1,0}, {0,-1}, {1,0}, {0,1}};
						for (int i=0; i < 4; ++i)
						{
							int x = pos.x + env[ i].x;
							int y = pos.y + env[ i].y;
							if ((x < 0) || (x > 1000))
								continue;
							if ((y < 0) || (y > 1000))
								continue;
							if (GetPixel(hdc, x, y) != orig_color)
								continue;
							
							array[ cnt].x = x;
							array[ cnt].y = y;
							++cnt;

							if (cnt >= 1000000)
							{
								cnt = 0;
								break;
							}

						}
					}
				}
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
			else if (PtInRect(&r2,pos))
			{
				state2 = 1;
			}
			else
			{
				if (state2 == 1)
				{
					fill_x = pos.x;
					fill_y = pos.y;
					fillPaint = true;
				}
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

	case WM_DESTROY: //�����찡 �ı��ȴٸ�..
		PostQuitMessage(0);	//���α׷� ���� ��û ( �޽��� ������ ���������� �ȴ� )
		break;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam ); // �⺻���� �޼��� ó���� ����Ѵ�.
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
