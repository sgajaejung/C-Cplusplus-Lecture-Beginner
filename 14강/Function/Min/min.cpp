#include <iostream>
using namespace std;

// 두개의 인자 중에 작은 값을 리턴하는 함수.
int myMin(int a, int b)
{
	return (a < b)? a : b;
}

void main()
{
	int input1, input2;
	cin >> input1 >> input2;

	// 작은 값을 출력한다.
	cout << myMin(input1, input2) << endl;
}
