#include <iostream>
using namespace std;

// 절대값을 리턴하는 함수.
int myAbs(int n)
{
	return (n > 0)? n : -n;
}

void main()
{
	int input;
	cin >> input;

	// 절대값을 출력한다.
	cout << myAbs(input) << endl;
}

