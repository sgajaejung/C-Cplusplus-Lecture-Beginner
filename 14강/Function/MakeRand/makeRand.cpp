#include <iostream>
#include <time.h>
using namespace std;

// �������� ���� ä���ִ� ���α׷�.
void MakeRand(int data[ 10])
{
	for (int i=0; i < 10; ++i)
	{
		data[ i] = rand();
	}
}

void main()
{
	srand((int)time(NULL));

	int values[10];
	MakeRand(values); // �������� ���� ä���.

	// �迭��ҵ� ���
	for (int i=0; i < 10; ++i)
	{
		cout << values[ i] << endl;
	}
}
