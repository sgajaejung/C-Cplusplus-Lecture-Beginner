#include <iostream>
#include <string>
using namespace std;

void main()
{
	// ���ڿ� ������ �����ϸ鼭 �ʱ�ȭ �Ѵ�.
	string str = "0123456789";

	// ���ڿ� ���.
	cout << str << endl;

	// ���ڿ� ���������� ȸ��.
	char k = str[ 9];
	for (int i=8; i >= 0; --i)
	{
		str[ i+1] = str[ i];
	}
	str[ 0] = k;

	// ȸ���� ���ڿ��� ����Ѵ�.
	cout << str << endl;
}
