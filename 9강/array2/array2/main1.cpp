#include <iostream>
#include <string>
using namespace std;

void main()
{
	// ��Ʈ�� �迭 ����.
	string strArray[ 3];

	strArray[ 0] = "Hello";
	strArray[ 1] = " ";
	strArray[ 2] = "World";

	// ���
	cout << strArray[ 0];
	cout << strArray[ 1];
	cout << strArray[ 2];
	cout << endl;

	// ��Ʈ�� �迭 �缳��.
	strArray[ 0] = "I ";
	strArray[ 1] = "am ";
	strArray[ 2] = "a Boy";

	// ���
	cout << strArray[ 0];
	cout << strArray[ 1];
	cout << strArray[ 2];
	cout << endl;
}
