#include <iostream>
using namespace std;

void main()
{
	// �迭 ����.
	int array[ 10];

	for (int i=0; i < 10; ++i)
	{
		array[ i] = i;
	}

	// ���
	for (int i=0; i < 10; ++i)
	{
		cout << array[ i];
	}
	cout << endl;

	//���������� ȸ��
	int k = array[ 9];
	for (int i=8; i >= 0; --i)
	{
		array[ i+1] = array[ i];
	}
	array[ 0] = k;

	// ���
	for (int i=0; i < 10; ++i)
	{
		cout << array[ i];
	}
	cout << endl;

}
