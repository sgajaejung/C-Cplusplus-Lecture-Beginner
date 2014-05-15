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

	// 수식을 입력하세요.
	cin >> first >> op >> second;

	if (op == "+")
	{
		cout << plus(first, second)	<< endl;
	}
	else if (op == "-")
	{
		cout << minus(first, second)	<< endl;
	}
}
