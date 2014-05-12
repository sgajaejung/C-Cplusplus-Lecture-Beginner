
#include <iostream>
using namespace std;

void main()
{
	// 변수 초기화
	int a = 100;
	int b = 200;
	int c = 300;
	int d = 10000;

	// 회전
	int temp = a;
	a = b;
	b = c;
	c = d;
	d = temp;
}
