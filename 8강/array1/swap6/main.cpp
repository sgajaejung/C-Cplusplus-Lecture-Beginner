
#include <iostream>
using namespace std;

void main()
{
	// ���� �ʱ�ȭ
	int a = 100;
	int b = 200;
	int c = 300;
	int d = 10000;

	// ȸ��
	int temp = d;
	d = c;
	c = b;
	b = a;
	a = temp;
}
