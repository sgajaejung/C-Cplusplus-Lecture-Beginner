#include <iostream>
#include <string>
using namespace std;

void main()
{
	// ���ڿ� ������ �����ϸ鼭 �ʱ�ȭ �Ѵ�.
	string str = "0123456789";

	// ���ڿ� ���.
	cout << str << endl;

	// ���ڿ� �������� ȸ��.
	char k = str[ 0];
	for (int i=0; i < 10; ++i)
	{
		str[ i] = str[ i+1];
	}
	str[ 9] = k;

	// ȸ���� ���ڿ��� ����Ѵ�.
	cout << str << endl;
}
