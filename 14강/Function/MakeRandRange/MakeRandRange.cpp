#include <iostream>
#include <time.h>
using namespace std;

// Ư������ ������ �������� ���� ä���ִ� ���α׷�.
void MakeRandRange(int data[ 10], int min0, int max0)
{
	const int range = max0 - min0;

	for (int i=0; i < 10; ++i)
	{
		data[ i] = (rand() % range) + min0;
	}
}

void main()
{
	srand((int)time(NULL)); // ���� ���尪 �ʱ�ȭ.

	int values[10];
	MakeRandRange(values, 100, 200); // �������� ���� ä���.

	// �迭��ҵ� ���
	for (int i=0; i < 10; ++i)
	{
		cout << values[ i] << endl;
	}
}
