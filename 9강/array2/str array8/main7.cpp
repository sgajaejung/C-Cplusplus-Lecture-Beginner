#include <iostream>
#include <string>
#include <sstream>
using namespace std;

void main()
{
	// ���� ���� �� ����.
	int n1 = 100;
	int n2 = 200;

	// ������ Ÿ���� ���ڿ� Ÿ������ ��ȯ�Ѵ�.
	stringstream ss1;
	ss1 << n1;

	stringstream ss2;
	ss2 << n2;

	// ���ڿ��� �ٲ� ���ڸ� ����ϰ�, ����� ����� ����Ѵ�.
	cout << n1 << " + " << n2 << " = " << n1 + n2 << endl;
}
