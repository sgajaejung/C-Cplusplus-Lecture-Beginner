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

	// �������� ȸ��
	int k = array[ 0];
	for (int i=0; i < 9; ++i)
	{
		array[ i] = array[ i+1];
	}
	array[ 9] = k;

	// ���
	for (int i=0; i < 10; ++i)
	{
		cout << array[ i];
	}
	cout << endl;

}
