#include <iostream>
#include <time.h>
using namespace std;

void main()
{
	srand((int)time(NULL));
	float numbers[10]; // �Ǽ����� ������ �迭.
	for (int i=0; i < 10; ++i)
	{ // �Ǽ� -1 ~ 0 ~ +1 ���� �����ϰ� �����ؼ� �����Ѵ�.
		numbers[ i] = ((rand() % 201) - 100) / 100.f;
		cout << numbers[ i] << endl; // �������� ����Ѵ�.
	}

	for (int k=0; k < 10; ++k)
	{
		for (int i=k+1; i < 10; ++i)
		{
			if (numbers[ i] > numbers[ k]) 
			{
				float tmp = numbers[ k]; // Swap
				numbers[ k] = numbers[ i];
				numbers[ i] = tmp;
			}
		}
	}
	cout << "**************" << endl;
	for (int i=0; i < 10; ++i)
	{
		cout << numbers[ i] << endl;
	}
}
