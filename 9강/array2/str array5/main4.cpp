#include <iostream>
#include <string>
using namespace std;

void main()
{
	// ��� �ʱ�ȭ.
	const int N = 3;
	string strArray[ N];

	 //strArray = { "hello", " ", "world" };

	 strArray[ 0] = "Hello";
	 strArray[ 1] = " ";
	 strArray[ 2] = "World";

	// ��Ʈ�� �迭 ���
	cout << strArray[ 0];
	cout << strArray[ 1];
	cout << strArray[ 2];
	cout << endl;
}
