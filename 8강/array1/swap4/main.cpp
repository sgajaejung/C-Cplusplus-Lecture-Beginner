
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

	// �������� ȸ��
	i = 0;
	int k = array[ 0];
	while (i < 9)
	{
		array[ i] = array[ i+1];
		++i;
	}
	array[ 9] = k;

	// ���
	i = 0;
	while (i < 10)
	{
		cout << array[ i];
		++i;
	}
	cout << endl;

}
