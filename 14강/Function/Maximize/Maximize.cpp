#include <iostream>
using namespace std;

// 세개의 인자 값 중에 가장 큰 값을 리턴한다.
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

	// 작은 값을 출력한다.
	cout << Maximize(input1, input2, input3) << endl;
}
