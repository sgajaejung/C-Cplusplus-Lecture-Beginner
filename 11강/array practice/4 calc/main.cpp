#include <iostream>
#include <string>
using namespace std;

int plus(int a, int b)
{
	return a + b;
}

int minus(int a, int b)
{
	return a - b;
}

void main()
{
	int first, second;
	string op;
	string op2;

	// 수식을 입력하세요.
	cin >> first;
	while (1)
	{
		cin >> op;
		if (op == "=")
		{
			cout << first << endl;
			break;
		}

		cin >> second;

		if (op == "+")
		{
			first = plus(first, second);
		}
		else if (op == "-")
		{
			first = minus(first, second);
		}
	}
}
