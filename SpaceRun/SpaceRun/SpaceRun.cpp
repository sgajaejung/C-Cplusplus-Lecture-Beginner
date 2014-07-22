
#include <windows.h>
#include <math.h>
#include <string>
#include <vector>
#include <objidl.h>
#include <gdiplus.h> 
#include <sstream>
#include <time.h>
#pragma comment( lib, "gdiplus.lib" ) 
#pragma comment(lib, "winmm.lib") 
using namespace std;
using namespace Gdiplus;


// Gdiplus 용
GdiplusStartupInput g_gdiplusStartupInput; 
ULONG_PTR g_gdiplusToken;
Graphics *g_graphics;
Font *g_font;
Pen *g_pen; // 펜 객체.
Pen *g_whitePen; // 펜 객체.
Pen *g_blackPen; // 펜 객체.
Pen *g_greenPen; // 펜 객체.
Pen *g_yellowPen; // 펜 객체.
Pen *g_pinkPen; // 펜 객체.
Brush *g_blackBrush; // 브러쉬 객체.
Brush *g_whiteBrush; // 브러쉬 객체.
Brush *g_greyBrush; // 브러쉬 객체.
Brush *g_greenBrush;
Brush *g_blueBrush;
Brush *g_brush; // 브러쉬 객체.
Brush *g_yellowBrush;
Image *g_image;
Image *g_image2;
Image *g_flameImage;
Image *g_enemyflameImage;
Image *g_explosionImage;
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
	int life;
	Point target;

	Block():life(100) {}
	Block(int x0, int y0):x(x0), y(y0), type(0), t(0), life(100) {}
	Block(int x0, int y0, int type0):x(x0), y(y0), type(type0), t(0), life(100) {}
};
std::vector<Block> g_ship;
Point g_shipPos(100,100);

struct Enemy
{
	int owner;
	std::vector<Block> blocks;
	Point pos;
	PointF posF;
	float vx;
};
std::vector<Enemy> g_enemys;
std::vector<Enemy> g_heros;



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


struct Explosion
{
	bool use;
	int owner;
	int t;
	int idx;
	float x;
	float y;
	Rect srcRect;
};
vector<Explosion> g_explosions(50);


// 콜백 프로시져 함수 프로토 타입
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, 
	WPARAM wParam, LPARAM lParam );
void InitGdiPlus(HWND hWnd);
void ReleaseGdiPlus();
void DrawString( Graphics *graphics, int x, int y, const wstring &str);
void MainLoop(int elapseT);
Rect GetAnimationRect(int elapseT);
Rect GetAnimationRect2(int elapseT);
Rect GetAnimationRectExplosion(int &t, int &idx);

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
Point GetMostNearEnemyBlockPos(Point originPos, std::vector<Enemy> &fights);
void SetBullet( Bullet bullet );
void SetExplosion( int owner, int x, int y );

void DrawEnemy(Graphics *graphic, std::vector<Enemy> &fights);
void MoveEnemy(std::vector<Enemy> &fights, int elapseT);
void EnemyShoot(std::vector<Enemy> &targets, Enemy &enemy, const int elapseT);
bool CheckEnemyCollision( std::vector<Enemy> &ships, int bulletX, int bulletY );
void spawnBossEnemy(Enemy &enemy);


int APIENTRY WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	wchar_t className[32] = L"Space Run";
	wchar_t windowName[32] = L"Space Run";

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
	RECT rcClient = { 0, 0, 1024, 640};
	AdjustWindowRect( &rcClient, WS_OVERLAPPEDWINDOW, FALSE );	//rcClient 크기를 작업 영영으로 할 윈도우 크기를 rcClient 에 대입되어 나온다.

	//윈도우 크기와 윈도우 위치를 바꾸어준다.
	SetWindowPos( hWnd, NULL, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, 
		SWP_NOZORDER | SWP_NOMOVE );

	g_hWnd = hWnd;

	// 윈도우를 화면에 띄운다.
	ShowWindow( hWnd, nCmdShow );

	srand( (int)time(NULL) );

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

			
			//DrawShip(graph, g_ship, g_shipPos, g_whiteBrush, g_blackPen);
			DrawEnemy(graph, g_heros);

			Block selBlock = GetBlockInPos(g_ship, g_shipPos, g_mousePos);
			if (selBlock.x >= 0 && selBlock.y >= 0)
			{
				DrawShipBlock(graph, g_shipPos, selBlock, 
					g_yellowBrush, g_blackPen);
			}


			DrawEnemy(graph, g_enemys);


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


			// explosion
			for (int i=0; i < g_explosions.size(); ++i)
			{
				if (!g_explosions[ i].use)
					continue;

				graph->DrawImage(g_explosionImage, 
					Rect(g_explosions[ i].x, g_explosions[ i].y, 96, 96),
					g_explosions[ i].srcRect.X, g_explosions[ i].srcRect.Y, 
					g_explosions[ i].srcRect.Width, g_explosions[ i].srcRect.Height, UnitPixel);
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

			for (int i=0; i < g_heros.size(); ++i)
			{
				Block &selBlock = GetBlockInPos(g_heros[i].blocks, g_heros[ i].pos, g_mouseClickPos);
				if (selBlock.x >= 0 && selBlock.y >= 0)
				{
					selBlock.type = 1;
				}
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

		case VK_SPACE:
			sndPlaySoundA("ambience_rain_outside.wav", SND_ASYNC);
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
	Gdiplus::GdiplusStartup(&g_gdiplusToken, 
		&g_gdiplusStartupInput, NULL); 
	g_graphics = new Graphics(hWnd);
	g_pen = new Pen(Color::Red);
	g_whitePen = new Pen(Color::White);
	g_blackPen = new Pen(Color::Black);
	g_greenPen = new Pen(Color::Green);
	g_yellowPen = new Pen(Color::Yellow);
	g_pinkPen = new Pen(Color::Linen);
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

	g_explosionImage = Image::FromFile(L"lvxdmali.png");

	for (int i=0; i < 30; ++i)
		g_dust.push_back( Point(rand()%1024, rand()%640) );

	RECT cr;
	GetClientRect(hWnd, &cr);
	g_bmp = new Bitmap(cr.right-cr.left, cr.bottom-cr.top);


	Enemy hero;
	hero.owner = 0;
	hero.vx = 0.001f;
	hero.pos = g_shipPos;
	hero.posF = PointF((float)g_shipPos.X, (float)g_shipPos.Y);
/*
	hero.blocks.push_back(Block(3,0));
	hero.blocks.push_back(Block(4,0));
	hero.blocks.push_back(Block(5,0));
	hero.blocks.push_back(Block(6,0));

	hero.blocks.push_back(Block(1,1));
	hero.blocks.push_back(Block(2,1));
	hero.blocks.push_back(Block(3,1));
	hero.blocks.push_back(Block(4,1));
	hero.blocks.push_back(Block(5,1));
	hero.blocks.push_back(Block(6,1));
	hero.blocks.push_back(Block(7,1));

	hero.blocks.push_back(Block(1,2));
	hero.blocks.push_back(Block(2,2));
	hero.blocks.push_back(Block(3,2));
	hero.blocks.push_back(Block(4,2));
	hero.blocks.push_back(Block(5,2));
	hero.blocks.push_back(Block(6,2));
	hero.blocks.push_back(Block(7,2));
	hero.blocks.push_back(Block(8,2));

	hero.blocks.push_back(Block(0,3));
	hero.blocks.push_back(Block(1,3));
	hero.blocks.push_back(Block(2,3));
	hero.blocks.push_back(Block(3,3));
	hero.blocks.push_back(Block(4,3));
	hero.blocks.push_back(Block(5,3));
	hero.blocks.push_back(Block(6,3));
	hero.blocks.push_back(Block(7,3));
	hero.blocks.push_back(Block(8,3));

	hero.blocks.push_back(Block(4,4));
	hero.blocks.push_back(Block(5,4));

	hero.blocks.push_back(Block(4,5));

	hero.blocks.push_back(Block(4,6));
	hero.blocks.push_back(Block(5,6));

	hero.blocks.push_back(Block(0,7));
	hero.blocks.push_back(Block(1,7));
	hero.blocks.push_back(Block(2,7));
	hero.blocks.push_back(Block(3,7));
	hero.blocks.push_back(Block(4,7));
	hero.blocks.push_back(Block(5,7));
	hero.blocks.push_back(Block(6,7));
	hero.blocks.push_back(Block(7,7));
	hero.blocks.push_back(Block(8,7));

	hero.blocks.push_back(Block(1,8));
	hero.blocks.push_back(Block(2,8));
	hero.blocks.push_back(Block(3,8));
	hero.blocks.push_back(Block(4,8));
	hero.blocks.push_back(Block(5,8));
	hero.blocks.push_back(Block(6,8));
	hero.blocks.push_back(Block(7,8));
	hero.blocks.push_back(Block(8,8));

	hero.blocks.push_back(Block(1,9));
	hero.blocks.push_back(Block(2,9));
	hero.blocks.push_back(Block(3,9));
	hero.blocks.push_back(Block(4,9));
	hero.blocks.push_back(Block(5,9));
	hero.blocks.push_back(Block(6,9));
	hero.blocks.push_back(Block(7,9));

	hero.blocks.push_back(Block(3,10));
	hero.blocks.push_back(Block(4,10));
	hero.blocks.push_back(Block(5,10));
	hero.blocks.push_back(Block(6,10));
*/
	hero.blocks.push_back(Block(1,0, 2));
	hero.blocks.push_back(Block(2,0, 1));
	hero.blocks.push_back(Block(0,1));
	hero.blocks.push_back(Block(1,1));
	hero.blocks.push_back(Block(2,1));
	hero.blocks.push_back(Block(1,2, 2));
	hero.blocks.push_back(Block(2,2, 1));

	g_heros.push_back(hero);


	Enemy enemy;
	enemy.owner = 1;
	enemy.vx = -0.001f;
	enemy.pos = Point(900,100);
	enemy.posF = PointF(900,100);
	enemy.blocks.push_back(Block(1,0, 1));
	enemy.blocks.push_back(Block(2,0, 3));
	enemy.blocks.push_back(Block(0,1));
	enemy.blocks.push_back(Block(1,1));
	enemy.blocks.push_back(Block(2,1));
	enemy.blocks.push_back(Block(1,2, 1));
	enemy.blocks.push_back(Block(2,2, 3));

	g_enemys.push_back( enemy );




	for (int i=0; i < g_explosions.size(); ++i)
	{
		if (i < (g_explosions.size()/2))
		{
			g_explosions[ i].owner = 0;
		}
		else
		{
			g_explosions[ i].owner = 1;
		}
	}


}


// Gdiplus 제거.
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
	delete g_explosionImage;
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
/*
	stringstream ss;
	ss << elapseT << endl;
	string str = ss.str();
	OutputDebugStringA( str.c_str() );
/**/

	g_flameSrc = GetAnimationRect(elapseT);

	// 우주먼지 애니메이션
	for (int i=0; i < g_dust.size(); ++i)
	{
		g_dust[ i].X -= (elapseT * 0.1f);
		if (g_dust[ i].X < 0)
		{
			g_dust[ i].X = 1024;
			g_dust[ i].Y = (rand() % 640);
		}
	}

	// 폭발 애니메이션
	for (int i=0; i < g_explosions.size(); ++i)
	{
		if (g_explosions[ i].use)
		{
			g_explosions[ i].t += elapseT;
			g_explosions[ i].srcRect = GetAnimationRectExplosion(
				g_explosions[ i].t, g_explosions[ i].idx);

			if (g_explosions[ i].idx < 0)
			{
				g_explosions[ i].use = false;
			}
		}
	}

	

//	g_enemyPosF.X -= (elapseT * 0.001f);
//	g_enemyPos = Point((int)g_enemyPosF.X, (int)g_enemyPosF.Y);
	MoveEnemy(g_enemys, elapseT);
	MoveEnemy(g_heros, elapseT);

	for (int i=0; i < g_enemys.size(); ++i)
	{
		EnemyShoot(g_heros, g_enemys[ i], elapseT);
	}

	for (int i=0; i < g_heros.size(); ++i)
	{
		EnemyShoot(g_enemys, g_heros[ i], elapseT);
	}

/*
	// 주인공편 총알 발사.
	for (int i=0; i < g_ship.size(); ++i)
	{
		if (g_ship[ i].type == 1)
		{
			g_ship[ i].t += elapseT;
			if (g_ship[ i].t > 2000)
			{
				g_ship[ i].t = 0;
				Point dst = GetBlockPos(g_ship[ i], g_shipPos);
				Point target = GetMostNearEnemyBlockPos(dst, g_enemys);
				if (target.X == -1 &&
					target.Y == -1)
					continue;

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

				bullet.vx = xx * 0.3f;
				bullet.vy = yy * 0.3f;
				SetBullet(bullet);
			}
		}
	}
/**/


	// 총알 이동.
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


	// 영역을 벗어난 총알은 제거.
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


	// 충돌테스트
	for (int i=0; i < g_bullets.size(); ++i)
	{
		if (g_bullets[ i].owner == 0)
		{
			if (CheckEnemyCollision(g_enemys, g_bullets[ i].x, g_bullets[ i].y))
			{
				Point pos(g_bullets[ i].x, g_bullets[ i].y);
				SetExplosion(0, pos.X-48, pos.Y-48);
				g_bullets[ i].use = false;
			}
		}


		// 주인공 비행기와 충돌했는지 확인.
		if (g_bullets[ i].owner == 1)
		{
			if (CheckEnemyCollision(g_heros, g_bullets[ i].x, g_bullets[ i].y))
			{
				Point pos(g_bullets[ i].x, g_bullets[ i].y);
				SetExplosion(1, pos.X-48, pos.Y-48);
				g_bullets[ i].use = false;
			}

/*
			// 적 비행기와 충돌했는지 확인.
			for (int k=0; k < g_ship.size(); ++k)
			{
				const Point pos = GetBlockPos(g_ship[ k], g_shipPos);
				const Point bulletPos = Point(g_bullets[ i].x, g_bullets[ i].y);
				const Point diff = pos - bulletPos;
				const int length = sqrt((double)(diff.X*diff.X + diff.Y*diff.Y));
				if (length < 25)
				{
					g_bullets[ i].use = false;
					g_ship[ k].life -= 1;
					if (g_ship[ k].life < 0)
					{
						g_ship.erase( g_ship.begin() + k );
					}

					SetExplosion(1, pos.X-48, pos.Y-48);
				}
			}
/**/
		}
	}


	// 적 비행기 등장.
	static int enemyApearT = 0;
	enemyApearT += elapseT;
	if (enemyApearT > 10000)
	{
		enemyApearT = 0;

		Enemy enemy;
		enemy.owner = 1;
		enemy.vx = -0.001f;

		if (rand()%3 == 0)
		{
			spawnBossEnemy(enemy);
		}
		else
		{
			const int yOffset = (rand() % 500);
			enemy.pos = Point(900,100+yOffset);
			enemy.posF = PointF(900,100+yOffset);
			enemy.blocks.push_back(Block(1,0, 1));
			enemy.blocks.push_back(Block(2,0, 3));
			enemy.blocks.push_back(Block(0,1));
			enemy.blocks.push_back(Block(1,1));
			enemy.blocks.push_back(Block(2,1));
			enemy.blocks.push_back(Block(1,2, 1));
			enemy.blocks.push_back(Block(2,2, 3));
		}

		g_enemys.push_back( enemy );
	}


	// 주인공 비행기 등장.
	static int heroApearT = 0;
	heroApearT += elapseT;
	if (heroApearT > 10000)
	{
		heroApearT = 0;

		Enemy hero;
		hero.owner = 0;
		hero.vx = 0.001f + 0.001f*(float)(rand()%5);

		const int yOffset = (rand() % 500);
		hero.pos = Point(50,100+yOffset);
		hero.posF = PointF(50,100+yOffset);
		hero.blocks.push_back(Block(1,0, 2));
		hero.blocks.push_back(Block(2,0, 1));
		hero.blocks.push_back(Block(0,1));
		hero.blocks.push_back(Block(1,1));
		hero.blocks.push_back(Block(2,1));
		hero.blocks.push_back(Block(1,2, 2));
		hero.blocks.push_back(Block(2,2, 1));

		g_heros.push_back( hero );
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

	{ // 총구.
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


Rect GetAnimationRectExplosion(int &t, int &idx)
{
	if (t > 15)
	{
		++idx;
		if (idx >= 20)
			idx = -1;
		t = 0;
	}

	const int width = 96;
	const int height = 96;

	return Rect((idx%5)*width, (idx/5)*height, width, height);
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


Point GetMostNearEnemyBlockPos(Point originPos, std::vector<Enemy> &flights)
{
	if (flights.empty())
		return Point(-1,-1);

	for (int i=0; i < flights.size(); ++i)
	{
		Point target = GetMostNearBlockPos(originPos, flights[ i].blocks, flights[ i].pos);
		if ((target.X == -1) && 
			(target.Y == -1))
			continue;
		return target;
	}

	return Point(-1,-1);
}


Point GetMostNearBlockPos(Point originPos, std::vector<Block> &ship, Point shipPos)
{
	Point nearPos(-1,-1);
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


void SetExplosion( int owner, int x, int y )
{
	for (int i=0; i < g_explosions.size(); ++i)
	{
		if ((g_explosions[ i].owner == owner) && !g_explosions[ i].use)
		{
			g_explosions[ i].use = true;
			g_explosions[ i].t = 0;
			g_explosions[ i].idx = 0;
			g_explosions[ i].x = x + rand()%30 - 15;
			g_explosions[ i].y = y + rand()%30 - 15;
			break;
		}
	}
}


void DrawEnemy(Graphics *graphic, std::vector<Enemy> &flights)
{
	for (int i=0; i < flights.size(); ++i)
	{
		DrawShip(graphic, flights[ i].blocks, flights[ i].pos, 
			g_greyBrush, g_blackPen );
	}
}


void MoveEnemy(std::vector<Enemy> &flights, int elapseT)
{
	for (int i=0; i < flights.size(); ++i)
	{
		flights[ i].posF.X += (elapseT * flights[ i].vx);
		flights[ i].pos = 
			Point((int)flights[ i].posF.X, (int)flights[ i].posF.Y);
	}
}


void EnemyShoot(std::vector<Enemy> &targets, Enemy &enemy, const int elapseT)
{
	for (int k=0; k < targets.size(); ++k)
	{

		for (int i=0; i < enemy.blocks.size(); ++i)
		{
			if (enemy.blocks[ i].type == 1)
			{
				enemy.blocks[ i].t += elapseT;
				if (enemy.blocks[ i].t > 2000)
				{
					enemy.blocks[ i].t = 0;
					Point dst = GetBlockPos(enemy.blocks[ i], enemy.pos);
					Point target = GetMostNearBlockPos(dst, targets[ k].blocks, targets[ k].pos);
					if ((target.X == -1) && 
						(target.Y == -1))
						continue;

					enemy.blocks[ i].target = target;

					Bullet bullet;
					bullet.owner = enemy.owner;
					bullet.x = dst.X;
					bullet.y = dst.Y;
					const int dx = target.X - dst.X;
					const int dy = target.Y - dst.Y;
					const int len = sqrt((double)(dx*dx + dy*dy));
					float xx = (float)dx/(float)len;
					float yy = (float)dy/(float)len;					

					bullet.vx = xx * 0.3f;
					bullet.vy = yy * 0.3f;
					SetBullet(bullet);
				}
			}
		}

	}
}

bool CheckEnemyCollision( std::vector<Enemy> &ships, int bulletX, int bulletY )
{
	for (int i=0; i < ships.size(); ++i)
	{
		// 적 비행기와 충돌했는지 확인.
		for (int k=0; k < ships[ i].blocks.size(); ++k)
		{
			const Point pos = GetBlockPos(ships[ i].blocks[ k], ships[ i].pos);
			const Point bulletPos = Point(bulletX, bulletY);
			const Point diff = pos - bulletPos;
			const int length = sqrt((double)(diff.X*diff.X + diff.Y*diff.Y));
			if (length < 30)
			{
				ships[ i].blocks[ k].life -= 1;
				if (ships[ i].blocks[ k].life < 0)
				{
					ships[ i].blocks.erase( ships[ i].blocks.begin() + k );
				}
				return true;
			}
		}
	}

	return false;
}


void spawnBossEnemy(Enemy &enemy)
{
	const int yOffset = (rand() % 500);
	enemy.pos = Point(900,100+yOffset);
	enemy.posF = PointF(900,100+yOffset);
	enemy.blocks.push_back(Block(2,0, 1));
	enemy.blocks.push_back(Block(3,0, 3));

	enemy.blocks.push_back(Block(0,1, 1));
	enemy.blocks.push_back(Block(1,1, 1));
	enemy.blocks.push_back(Block(2,1, 1));
	enemy.blocks.push_back(Block(3,1, 3));

	enemy.blocks.push_back(Block(2,2, 1));
	enemy.blocks.push_back(Block(3,2, 3));
}
