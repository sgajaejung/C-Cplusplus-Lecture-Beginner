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
