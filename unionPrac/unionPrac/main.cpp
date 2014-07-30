
#include <iostream>
using namespace std;

struct sTT
{
	int dd;
	int ee;
	int ff;
};

struct sAA
{
	union
	{
		int a;
		int b;
		int c;
		sTT d;
	};
};

struct sLogin
{
	union
	{
		char packet[ 128];

		struct
		{
			char id[8];
			char passwd[ 8];
			BOOL num1 : 1;
			BOOL num2 : 1;
			BOOL num3 : 1;
			BOOL num4 : 1;
			int key;

			BOOL n;
			bool n2;
		};
	};
};


void main()
{
	sAA a;
	a.a = 10;
	a.b = 11;
	a.c = 12;
	a.d.dd = 13;
	a.d.ee = 14;

	cout << "sizeof sAA = " << sizeof(sAA) << endl;

	cout << a.a << endl;
	cout << a.b << endl;
	cout << a.c << endl;
	cout << a.d.dd << endl;
	cout << a.d.ee << endl;

	cout << &a.a << endl;
	cout << &a.b << endl;
	cout << &a.c << endl;


	sLogin login;
	strcpy(login.id, "name" );
	login.num = 10;

	//send( login.packet, ~~ );


}
