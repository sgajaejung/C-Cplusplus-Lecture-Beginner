#include <iostream>
using namespace std;

// �ΰ��� ���� �߿� ���� ���� �����ϴ� �Լ�.
int myMin(int a, int b)
{
	return (a < b)? a : b;
}

void main()
{
	int input1, input2;
	cin >> input1 >> input2;

	// ���� ���� ����Ѵ�.
	cout << myMin(input1, input2) << endl;
}
