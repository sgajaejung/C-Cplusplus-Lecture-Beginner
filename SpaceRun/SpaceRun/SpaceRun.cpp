
#include <windows.h>
#include <math.h>
#include <string>
#include <vector>
#include <objidl.h>
#include <gdiplus.h> 
#include <sstream>
#include <time.h>
#pragma comment( lib, "gdiplus.lib" ) 
using namespace std;
using namespace Gdiplus;


// Gdiplus ��
GdiplusStartupInput g_gdiplusStartupInput; 
ULONG_PTR g_gdiplusToken;
Graphics *g_graphics;
Font *g_font;
Pen *g_pen; // �� ��ü.
Pen *g_whitePen; // �� ��ü.
Pen *g_blackPen; // �� ��ü.
Pen *g_greenPen; // �� ��ü.
Pen *g_yellowPen; // �� ��ü.
Pen *g_pinkPen; // �� ��ü.
Brush *g_blackBrush; // �귯�� ��ü.
Brush *g_whiteBrush; // �귯�� ��ü.
Brush *g_greyBrush; // �귯�� ��ü.
Brush *g_greenBrush;
Brush *g_blueBrush;
Brush *g_brush; // �귯�� ��ü.
Brush *g_yellowBrush;
Image *g_image;
Image *g_image2;
Image *g_flameImage;
Image *g_enemyflameImage;
Bitmap *g_bmp;
HWND g_hWnd;

Point g_mousePos;
Point g_mouseClickPos;
bool g_IsClick = false;
int g_hatchStyle = 0;

Rect g_bullet(300, 400, 120, 120);
Rect g_bulletSrc;
Rect g_explosion(0, 0, 64, 64);
Rect g_flameSrc(0, 0, 0, 0);
Rect g_flameDst(100, 100, 64, 64);

int g_incT = 0;
int frameT = 0;
int frame = 0;
wstring frameStr;

const float pi = 3.1415f;
const float cellSize = 20.f;


struct Block
{
	int x,y;
	int type; // 0, 1=gun, 2=engine, 3=enemy engine
	int t;
	Point target;

	Block() {}
	Block(int x0, int y0):x(x0), y(y0), type(0), t(0) {}
	Block(int x0, int y0, int type0):x(x0), y(y0), type(type0), t(0) {}
};
std::vector<Block> g_ship;
Point g_shipPos(100,100);

std::vector<Block> g_enemy;
Point g_enemyPos(900,100);
PointF g_enemyPosF(900,100);

vector<Point> g_dust;
float g_scrollPos = 0;
Rect g_scrollDstRect1;
Rect g_scrollSrcRect1;
Rect g_scrollDstRect2;
Rect g_scrollSrcRect2;


struct Bullet
{
	bool use;
	int owner;
	float x;
	float y;
	float vx;
	float vy;
};
vector<Bullet> g_bullets(200);




// �ݹ� ���ν��� �Լ� ������ Ÿ��
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, 
	WPARAM wParam, LPARAM lParam );
void InitGdiPlus(HWND hWnd);
void ReleaseGdiPlus();
void DrawString( Graphics *graphics, int x, int y, const wstring &str);
void MainLoop(int elapseT);
Rect GetAnimationRect(int elapseT);
Rect GetAnimationRect2(int elapseT);

void DrawHexagon(Graphics *graphic, const int x, const int y, 
	Brush *brush, Pen *pen, const bool isFill);

void DrawHexagonGun(Graphics *graphic, const int x, const int y, 
	Block block, Brush *brush, Pen *pen );

void DrawHexagonEngine(Graphics *graphic, const int x, const int y, 
	Brush *brush, Pen *pen, int type );

void DrawShip( Graphics *graphic, std::vector<Block> &ship, Point pos,
	Brush *brush, Pen *pen);
Block& GetBlockInPos( std::vector<Block> &ship, Point shipPos, Point pos);
void DrawShipBlock( Graphics *graphic, Point shipPos, Block block, Brush *brush, Pen *pen);
void DrawShipBlockOption( Graphics *graphic, 
	Point shipPos, Block block, Brush *brush, Pen *pen);

Point GetBlockPos(Block block, Point shipPos);

Point GetMostNearBlockPos(Point originPos, std::vector<Block> &ship, Point shipPos);
void SetBullet( Bullet bullet );



int APIENTRY WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	wchar_t className[32] = L"Space Run";
	wchar_t windowName[32] = L"Space Run";

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
	RECT rcClient = { 0, 0, 1024, 640};
	AdjustWindowRect( &rcClient, WS_OVERLAPPEDWINDOW, FALSE );	//rcClient ũ�⸦ �۾� �������� �� ������ ũ�⸦ rcClient �� ���ԵǾ� ���´�.

	//������ ũ��� ������ ��ġ�� �ٲپ��ش�.
	SetWindowPos( hWnd, NULL, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, 
		SWP_NOZORDER | SWP_NOMOVE );

	g_hWnd = hWnd;

	// �����츦 ȭ�鿡 ����.
	ShowWindow( hWnd, nCmdShow );

	InitGdiPlus(hWnd);

	SetTimer(hWnd, 1, 5000, NULL);
	SetTimer(hWnd, 2, 1000, NULL);

	srand( (int)time(NULL) );


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


		{
			const int curT = GetTickCount();
			const int elapseT = curT - oldT;
			if (elapseT > 15)
			{
				oldT = curT;
				MainLoop(elapseT);
			}
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

			Graphics *graph = Graphics::FromImage(g_bmp);

			RECT cr;
			GetClientRect(hWnd, &cr);
			Rect wndSize(cr.left, cr.top, cr.right, cr.bottom);
			//graph->DrawImage(g_image, wndSize);
			graph->DrawImage(g_image, g_scrollDstRect1, 
				g_scrollSrcRect1.X, g_scrollSrcRect1.Y, 
				g_scrollSrcRect1.Width, g_scrollSrcRect1.Height, UnitPixel);

			if (g_scrollSrcRect1.Width < 1024)
			{
				graph->DrawImage(g_image, g_scrollDstRect2, 
					g_scrollSrcRect2.X, g_scrollSrcRect2.Y, 
					g_scrollSrcRect2.Width, g_scrollSrcRect2.Height, UnitPixel);
			}

			//graph->DrawImage(g_flameImage, wndSize);

			for (int i=0; i < g_dust.size(); ++i)
			{
				graph->DrawRectangle(g_whitePen, g_dust[ i].X, g_dust[ i].Y, 5, 1);
			}

			
			DrawShip(graph, g_ship, g_shipPos, g_whiteBrush, g_blackPen);

			Block selBlock = GetBlockInPos(g_ship, g_shipPos, g_mousePos);
			if (selBlock.x >= 0 && selBlock.y >= 0)
			{
				DrawShipBlock(graph, g_shipPos, selBlock, 
					g_yellowBrush, g_blackPen);
			}


			DrawShip(graph, g_enemy, g_enemyPos, g_greyBrush, g_blackPen);


			for (int i=0; i < g_bullets.size(); ++i)
			{
				if (!g_bullets[ i].use)
					continue;

				int x = g_bullets[ i].x + g_bullets[ i].vx * 50.f;
				int y = g_bullets[ i].y + g_bullets[ i].vy * 50.f;

				Pen *pen;
				if (g_bullets[ i].owner == 0)
				{
					pen = g_pinkPen;
				}
				else
				{
					pen = g_yellowPen;
				}

				graph->DrawLine(pen, 
					Point((int)g_bullets[ i].x, (int)g_bullets[ i].y),
					Point(x,y));
				graph->DrawLine(pen, 
					Point((int)g_bullets[ i].x, (int)g_bullets[ i].y+1),
					Point(x,y+1));
				graph->DrawLine(pen, 
					Point((int)g_bullets[ i].x, (int)g_bullets[ i].y+2),
					Point(x,y+2));
			}





			DrawString(graph, 50, 0, frameStr);
			g_graphics->DrawImage(g_bmp, wndSize);
			EndPaint(hWnd, &ps);
		}
		break;

//	case WM_ERASEBKGND:
//		return 0;

	case WM_LBUTTONDOWN:
		{
			g_IsClick = true;
			g_mouseClickPos = Point(LOWORD(lParam), HIWORD(lParam));

			Block &selBlock = GetBlockInPos(g_ship, g_shipPos, g_mouseClickPos);
			if (selBlock.x >= 0 && selBlock.y >= 0)
			{
				selBlock.type = 1;
			}
		}
		break;

	case WM_RBUTTONDOWN:
		{
			g_IsClick = true;
			g_mouseClickPos = Point(LOWORD(lParam), HIWORD(lParam));

			Block &selBlock = GetBlockInPos(g_ship, g_shipPos, g_mouseClickPos);
			if (selBlock.x >= 0 && selBlock.y >= 0)
			{
				selBlock.type = 2;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			g_IsClick = false;
		}
		break;

	case WM_MOUSEMOVE:
		{
			g_mousePos = Point(LOWORD(lParam), HIWORD(lParam));
		}
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
		case VK_UP:
			++g_hatchStyle;
			break;

		case VK_RIGHT:
		case VK_DOWN:
			--g_hatchStyle;
			break;
		}
		break;

	case WM_TIMER:
		{
			if (wParam == 1)
			{
				int a = 0;
			}
			else if (wParam == 2)
			{
				int a = 0;
			}
		}
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
	Gdiplus::GdiplusStartup(&g_gdiplusToken, 
		&g_gdiplusStartupInput, NULL); 
	g_graphics = new Graphics(hWnd);
	g_pen = new Pen(Color::Red);
	g_whitePen = new Pen(Color::White);
	g_blackPen = new Pen(Color::Black);
	g_greenPen = new Pen(Color::Green);
	g_yellowPen = new Pen(Color::Yellow);
	g_pinkPen = new Pen(Color::Red);
	g_brush = new SolidBrush(Color::White);
	//g_yellowBrush = new SolidBrush(Color::Yellow);
	g_yellowBrush = new HatchBrush(HatchStyleNarrowVertical, Color::Yellow);
	g_blackBrush = new SolidBrush(0xFF000000);
	g_whiteBrush = new SolidBrush(0xFFFFFFFF);
	g_greyBrush = new SolidBrush(0xFF999999);
	g_greenBrush = new SolidBrush(0xFF00FF00);
	g_blueBrush = new SolidBrush(0xFF0000FF);
	g_font = new Font(L"Arial", 16);
	//g_image = Image::FromFile(L"Kang_So_Ra9.jpg");
	//g_image = Image::FromFile(L"space-wallpaper-5_2.jpg");
	g_image = Image::FromFile(L"milky_way.jpg");	
	g_image2 = Image::FromFile(L"sprite1.png");
	//g_image3 = Image::FromFile(L"explosion_opaque.png");
	g_flameImage = Image::FromFile(L"flame.png");
	g_flameImage->RotateFlip(Rotate270FlipNone);

	g_enemyflameImage = Image::FromFile(L"flame.png");
	g_enemyflameImage->RotateFlip(Rotate90FlipNone);	

	for (int i=0; i < 30; ++i)
		g_dust.push_back( Point(rand()%1024, rand()%640) );

	RECT cr;
	GetClientRect(hWnd, &cr);
	g_bmp = new Bitmap(cr.right-cr.left, cr.bottom-cr.top);

	g_ship.push_back(Block(3,0));
	g_ship.push_back(Block(4,0));
	g_ship.push_back(Block(5,0));
	g_ship.push_back(Block(6,0));

	g_ship.push_back(Block(1,1));
	g_ship.push_back(Block(2,1));
	g_ship.push_back(Block(3,1));
	g_ship.push_back(Block(4,1));
	g_ship.push_back(Block(5,1));
	g_ship.push_back(Block(6,1));
	g_ship.push_back(Block(7,1));

	g_ship.push_back(Block(1,2));
	g_ship.push_back(Block(2,2));
	g_ship.push_back(Block(3,2));
	g_ship.push_back(Block(4,2));
	g_ship.push_back(Block(5,2));
	g_ship.push_back(Block(6,2));
	g_ship.push_back(Block(7,2));
	g_ship.push_back(Block(8,2));

	g_ship.push_back(Block(0,3));
	g_ship.push_back(Block(1,3));
	g_ship.push_back(Block(2,3));
	g_ship.push_back(Block(3,3));
	g_ship.push_back(Block(4,3));
	g_ship.push_back(Block(5,3));
	g_ship.push_back(Block(6,3));
	g_ship.push_back(Block(7,3));
	g_ship.push_back(Block(8,3));

	g_ship.push_back(Block(4,4));
	g_ship.push_back(Block(5,4));

	g_ship.push_back(Block(4,5));

	g_ship.push_back(Block(4,6));
	g_ship.push_back(Block(5,6));

	g_ship.push_back(Block(0,7));
	g_ship.push_back(Block(1,7));
	g_ship.push_back(Block(2,7));
	g_ship.push_back(Block(3,7));
	g_ship.push_back(Block(4,7));
	g_ship.push_back(Block(5,7));
	g_ship.push_back(Block(6,7));
	g_ship.push_back(Block(7,7));
	g_ship.push_back(Block(8,7));

	g_ship.push_back(Block(1,8));
	g_ship.push_back(Block(2,8));
	g_ship.push_back(Block(3,8));
	g_ship.push_back(Block(4,8));
	g_ship.push_back(Block(5,8));
	g_ship.push_back(Block(6,8));
	g_ship.push_back(Block(7,8));
	g_ship.push_back(Block(8,8));

	g_ship.push_back(Block(1,9));
	g_ship.push_back(Block(2,9));
	g_ship.push_back(Block(3,9));
	g_ship.push_back(Block(4,9));
	g_ship.push_back(Block(5,9));
	g_ship.push_back(Block(6,9));
	g_ship.push_back(Block(7,9));

	g_ship.push_back(Block(3,10));
	g_ship.push_back(Block(4,10));
	g_ship.push_back(Block(5,10));
	g_ship.push_back(Block(6,10));



	g_enemy.push_back(Block(1,0, 1));
	g_enemy.push_back(Block(2,0, 3));
	g_enemy.push_back(Block(0,1));
	g_enemy.push_back(Block(1,1));
	g_enemy.push_back(Block(2,1));
	g_enemy.push_back(Block(1,2, 1));
	g_enemy.push_back(Block(2,2, 3));

}


// Gdiplus ����.
void ReleaseGdiPlus()
{
	delete g_image;
	delete g_image2;
	delete g_font;
	delete g_pen;
	delete g_whitePen;
	delete g_blackPen;
	delete g_greenPen;
	delete g_yellowPen;
	delete g_pinkPen;
	delete g_brush;
	delete g_yellowBrush;
	delete g_blackBrush;
	delete g_whiteBrush;
	delete g_greyBrush;
	delete g_greenBrush;
	delete g_blueBrush;
	delete g_graphics;
	delete g_bmp;
	delete g_enemyflameImage;
	delete g_flameImage;
	// Shutdown Gdiplus 
	Gdiplus::GdiplusShutdown(g_gdiplusToken); 
}


void DrawString( Graphics *graphics, int x, int y, const wstring &str)
{
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);

	graphics->DrawString( str.c_str(), -1, g_font,
		PointF((REAL)x, (REAL)y),
		&format, g_yellowBrush);
}


void MainLoop(int elapseT)
{
	g_flameSrc = GetAnimationRect(elapseT);

	for (int i=0; i < g_dust.size(); ++i)
	{
		g_dust[ i].X -= (elapseT * 0.1f);
		if (g_dust[ i].X < 0)
		{
			g_dust[ i].X = 1024;
			g_dust[ i].Y = (rand() % 640);
		}
	}
	

	g_enemyPosF.X -= (elapseT * 0.001f);
	g_enemyPos = Point((int)g_enemyPosF.X, (int)g_enemyPosF.Y);

	for (int i=0; i < g_enemy.size(); ++i)
	{
		if (g_enemy[ i].type == 1)
		{
			g_enemy[ i].t += elapseT;
			if (g_enemy[ i].t > 600)
			{
				g_enemy[ i].t = 0;
				Point dst = GetBlockPos(g_enemy[ i], g_enemyPos);
				Point target = GetMostNearBlockPos(dst, g_ship, g_shipPos);
				g_enemy[ i].target = target;

				Bullet bullet;
				bullet.owner = 1;					 
				bullet.x = dst.X;
				bullet.y = dst.Y;
				const int dx = target.X - dst.X;
				const int dy = target.Y - dst.Y;
				const int len = sqrt((double)(dx*dx + dy*dy));
				float xx = (float)dx/(float)len;
				float yy = (float)dy/(float)len;					

				bullet.vx = xx * 0.1f;
				bullet.vy = yy * 0.1f;
				SetBullet(bullet);
			}
		}
	}

	// ���ΰ��� �Ѿ� �߻�.
	for (int i=0; i < g_ship.size(); ++i)
	{
		if (g_ship[ i].type == 1)
		{
			g_ship[ i].t += elapseT;
			if (g_ship[ i].t > 600)
			{
				g_ship[ i].t = 0;
				Point dst = GetBlockPos(g_ship[ i], g_shipPos);
				Point target = GetMostNearBlockPos(dst, g_enemy, g_enemyPos);
				g_ship[ i].target = target;

				Bullet bullet;
				bullet.owner = 0;
				bullet.x = dst.X;
				bullet.y = dst.Y;
				const int dx = target.X - dst.X;
				const int dy = target.Y - dst.Y;
				const int len = sqrt((double)(dx*dx + dy*dy));
				float xx = (float)dx/(float)len;
				float yy = (float)dy/(float)len;					

				bullet.vx = xx * 0.1f;
				bullet.vy = yy * 0.1f;
				SetBullet(bullet);
			}
		}
	}



	// �Ѿ� �̵�.
	for (int i=0; i < g_bullets.size(); ++i)
	{
		if (g_bullets[ i].use)
		{
			g_bullets[ i].x += (elapseT * g_bullets[ i].vx);
			g_bullets[ i].y += (elapseT * g_bullets[ i].vy);
		}
	}




	g_scrollPos += (elapseT * 0.02f);
	g_scrollDstRect1.X = 0;
	g_scrollDstRect1.Y = 0;
	g_scrollDstRect1.Width = min(1600-g_scrollPos, 1024);
	g_scrollDstRect1.Height = 640;

	g_scrollSrcRect1.X = g_scrollPos;
	g_scrollSrcRect1.Y = 0;
	g_scrollSrcRect1.Width = min(1600-g_scrollPos, 1024);
	g_scrollSrcRect1.Height = 640;


	g_scrollDstRect2.X = min(1600-g_scrollPos, 1024);
	g_scrollDstRect2.Y = 0;
	g_scrollDstRect2.Width = 1024-min(1600-g_scrollPos, 1024);
	g_scrollDstRect2.Height = 640;

	g_scrollSrcRect2.X = 0;
	g_scrollSrcRect2.Y = 0;
	g_scrollSrcRect2.Width = 1024-min(1600-g_scrollPos, 1024);
	g_scrollSrcRect2.Height = 640;

	if (g_scrollPos > 1600)
	{
		g_scrollPos = 0;
	}


	// ������ ��� �Ѿ��� ����.
	for (int i=0; i < g_bullets.size(); ++i)
	{
		if (g_bullets[ i].use)
		{
			if (g_bullets[ i].x > 1024)
			{
				g_bullets[ i].use = false;
			}
			if (g_bullets[ i].x < 0)
			{
				g_bullets[ i].use = false;
			}
		}
	}


	// �浹�׽�Ʈ
	for (int i=0; i < g_bullets.size(); ++i)
	{

	}


	++frame;
	frameT += elapseT;
	if (frameT > 1000)
	{
		std::wstringstream ss;
		ss << frame;
		frameStr = ss.str();
		frame = 0;
		frameT = 0;
	}

	::InvalidateRect(g_hWnd, NULL, FALSE);
}


void DrawHexagon(Graphics *graphic, const int x, const int y, 
	Brush *brush, Pen *pen, const bool isFill)
{
	GraphicsPath path;
	float radius = cellSize;
	float angle = pi/2.f;
	Point offset(x,y);
	Point points[6];
	for (int i=0; i < 6; ++i)
	{
		Point pos;
		pos.X = (int)(radius * cos(angle));
		pos.Y = (int)(radius * sin(angle));
		points[ i] = pos + offset;
		angle += pi/3.f;
	}
	path.AddPolygon(points, 6);
	if (isFill)
		graphic->FillPath(brush, &path);
	else
		graphic->DrawPath(pen, &path);
}


void DrawHexagonGun(Graphics *graphic, const int x, const int y, 
	Block block, Brush *brush, Pen *pen )
{
	GraphicsPath path;
	Point points[4];
	points[ 0] = Point(0, -cellSize/2);
	points[ 1] = Point(cellSize, -cellSize/2);
	points[ 2] = Point(cellSize, cellSize/2);
	points[ 3] = Point(0, cellSize/2);
	for (int i=0; i < 4; ++i)
		points[ i] = points[ i];

	path.AddPolygon(points, 4);

	Point diff = block.target - Point(x,y);
	float rad = atan((float)diff.Y/(float)diff.X);
	int angle = (int)(180.f/pi * rad);
	if (diff.X < 0)
		angle += 180;

	Matrix R;
	R.Rotate(angle);
	Matrix T;
	T.Translate(x, y);
	T.Multiply(&R);
	path.Transform( &T );


	graphic->DrawPath(pen, &path);
	graphic->FillPath(brush, &path);

	{ // �ѱ�.
		const float length = cellSize;
		GraphicsPath path;
		Point points[4];
		points[ 0] = Point(cellSize, -cellSize/6);
		points[ 1] = Point(cellSize+length, -cellSize/6);
		points[ 2] = Point(cellSize+length, cellSize/6);
		points[ 3] = Point(cellSize, cellSize/6);
		for (int i=0; i < 4; ++i)
			points[ i] = points[ i];//+ Point(x,y);

		path.AddPolygon(points, 4);
		path.Transform( &T );
		graphic->DrawPath(pen, &path);
		graphic->FillPath(brush, &path);
	}

}


void DrawHexagonEngine( Graphics *graphic, const int x, const int y, 
	Brush *brush, Pen *pen, Image *image, int type )
{
	GraphicsPath path;
	Point points[4];
	points[ 0] = Point(0, -cellSize/2);
	points[ 1] = Point(-cellSize, -cellSize/2);
	points[ 2] = Point(-cellSize, cellSize/2);
	points[ 3] = Point(0, cellSize/2);
	for (int i=0; i < 4; ++i)
		points[ i] = points[ i] + Point(x,y);

	path.AddPolygon(points, 4);
	graphic->DrawPath(pen, &path);
	graphic->FillPath(brush, &path);

	Rect dest;
	if (2 == type)
	{
		dest = Rect(x-64, y-32, 64, 64);
	}
	else
	{
		dest = Rect(x, y-32, 64, 64);
	}

	graphic->DrawImage(image, dest, 
		g_flameSrc.X, g_flameSrc.Y, g_flameSrc.Width, g_flameSrc.Height,
		UnitPixel);
}


void DrawShipBlock( Graphics *graphic, 
	Point shipPos, Block block, Brush *brush, Pen *pen)
{
	Point dst = GetBlockPos(block, shipPos);
	DrawHexagon(graphic, dst.X, dst.Y, brush, pen, true);
	DrawHexagon(graphic, dst.X, dst.Y, brush, pen, false);
}


void DrawShipBlockOption( Graphics *graphic, 
	Point shipPos, Block block, Brush *brush, Pen *pen)
{
	Point dst = GetBlockPos(block, shipPos);

	if (block.type == 1)
	{
		DrawHexagonGun(graphic, dst.X, dst.Y, block, g_greenBrush, g_blackPen);
	}
	else if (block.type == 2)
	{
		DrawHexagonEngine(graphic, dst.X, dst.Y, g_blueBrush, g_blackPen,
			g_flameImage, 2);
	}
	else if (block.type == 3)
	{
		DrawHexagonEngine(graphic, dst.X, dst.Y, g_blueBrush, g_blackPen,
			g_enemyflameImage, 3);
	}
}


void DrawShip( Graphics *graphic, std::vector<Block> &ship, Point pos,
	Brush *brush, Pen *pen)
{
	const int width = (int)(cos(pi/6.f) * cellSize);
	const int height = (int)(cos(pi/3.f) * cellSize);

	for (int i=ship.size()-1; i >= 0; --i)
	{
		DrawShipBlock(graphic, pos, ship[ i],
			brush, pen);
	}

	for (int i=ship.size()-1; i >= 0; --i)
	{
		DrawShipBlockOption(graphic, pos, ship[ i],
			brush, pen);
	}
}


Block& GetBlockInPos( std::vector<Block> &ship, Point shipPos, Point pos)
{
	const int width = (int)(cos(pi/6.f) * cellSize);
	const int height = (int)(cos(pi/3.f) * cellSize);

	for (int i=0; i < (int)ship.size(); ++i)
	{
		int x = (int)(ship[ i].x * width*2.f) + shipPos.X;
		int y = (int)(ship[ i].y * (cellSize+height)) + shipPos.Y;
		if ((ship[ i].y % 2) == 1)
		{
			x += width;
		}

		Point diff = Point(x,y) - pos;
		const float len = (float)sqrt((double)(diff.X*diff.X + diff.Y*diff.Y));
		if (cellSize > len)
		{
			return ship[ i];
		}
	}

	static Block emptyBlock(-1,-1);
	return emptyBlock;
}


Rect GetAnimationRect(int elapseT)
{
	static int t = 0;
	static int idx = 0;
	t += elapseT;

	if (t > 25)
	{
		++idx;
		if (idx >= 16)
			idx = 0;
	}

	const int width = 64;
	const int height = 64;

	return Rect((idx%4)*width, (idx/4)*height, width, height);
}


Point GetBlockPos(Block block, Point shipPos)
{
	const int width = (int)(cos(pi/6.f) * cellSize);
	const int height = (int)(cos(pi/3.f) * cellSize);

	int dstX = (int)(block.x * width*2.f) + shipPos.X;
	int dstY = (int)(block.y * (cellSize+height)) + shipPos.Y;
	if ((block.y % 2) == 1)
	{
		dstX += width;
	}

	return Point(dstX, dstY);
}


Point GetMostNearBlockPos(Point originPos, std::vector<Block> &ship, Point shipPos)
{
	Point nearPos;
	int nearLen = 10000000;
	for (int i=0; i < ship.size(); ++i)
	{
		Point pos = GetBlockPos(ship[ i], shipPos);
		Point p = originPos - pos;
		const int len = p.X * p.X + p.Y * p.Y;
		if (nearLen > len)
		{
			nearLen = len;
			nearPos = pos;
		}		
	}

	return nearPos;
}


void SetBullet( Bullet bullet )
{
	for (int i=0; i < g_bullets.size(); ++i)
	{
		if (!g_bullets[ i].use)
		{
			g_bullets[ i] = bullet;
			g_bullets[ i].use = true;
			break;
		}
	}
}
