
#include <iostream>
using namespace std;

void main()
{
	// ���� �ʱ�ȭ
	int a = 100;
	int b = 200;
	int c = 300;

	// ȸ��
	int temp = c;
	c = b;
	b = a;
	a = temp;
}
