
#include <iostream>
using namespace std;

void main()
{
	// �迭 ����.
	int array[ 10];

	int i = 0;
	while (i < 10)
	{
		array[ i] = i;
		++i;
	}

	// ���
	i = 0;
	while (i < 10)
	{
		cout << array[ i];
		++i;
	}
	cout << endl;

	// ���������� ȸ��
	i = 8;
	int k = array[ 9];
	while (i >= 0)
	{
		array[ i+1] = array[ i];
		--i;
	}
	array[ 0] = k;

	// ���
	i = 0;
	while (i < 10)
	{
		cout << array[ i];
		++i;
	}
	cout << endl;

}
