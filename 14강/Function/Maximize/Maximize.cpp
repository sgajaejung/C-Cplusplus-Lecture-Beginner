#include <iostream>
using namespace std;

// ������ ���� �� �߿� ���� ū ���� �����Ѵ�.
int Maximize(int a, int b, int c)
{
	if (a > b)
	{
		if (a > c)
			return a;
		else
			return c;
	}
	else
	{
		if (b > c)
			return b;
		else
			return c;
	}	
}

void main()
{
	int input1, input2, input3;
	cin >> input1 >> input2 >> input3;

	// ���� ���� ����Ѵ�.
	cout << Maximize(input1, input2, input3) << endl;
}
