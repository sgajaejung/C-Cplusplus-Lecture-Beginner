#include <iostream>
using namespace std;

// ���밪�� �����ϴ� �Լ�.
int myAbs(int n)
{
	return (n > 0)? n : -n;
}

void main()
{
	int input;
	cin >> input;

	// ���밪�� ����Ѵ�.
	cout << myAbs(input) << endl;
}

