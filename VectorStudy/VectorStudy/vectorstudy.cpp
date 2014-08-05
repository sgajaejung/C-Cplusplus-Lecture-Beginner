
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


// Gdiplus ��
GdiplusStartupInput g_gdiplusStartupInput; 
ULONG_PTR g_gdiplusToken;
Graphics *g_graphics;
Font *g_font;
Pen *g_pen; // �� ��ü.
Brush *g_blackBrush; // �귯�� ��ü.
Brush *g_brush; // �귯�� ��ü.
Brush *g_brush2; // �귯�� ��ü.
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


// �ݹ� ���ν��� �Լ� ������ Ÿ��
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

		int curT = GetTickCount();
		int elapseT = curT - oldT;
		oldT = curT;

		MainLoop(elapseT);
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


// Gdiplus ����.
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
