#include <iostream>
#include <time.h>
using namespace std;

void bubbleSort(float data[10])
{
	for (int k=0; k < 10; ++k)
	{
		for (int i=k+1; i < 10; ++i)
		{
			if (data[ i] > data[ k]) 
			{
				float tmp = data[ k]; // Swap
				data[ k] = data[ i];
				data[ i] = tmp;
			}
		}
	}
}

void main()
{
	srand((int)time(NULL));
	float numbers[10]; // �Ǽ����� ������ �迭.
	for (int i=0; i < 10; ++i)
	{ // �Ǽ� -1 ~ 0 ~ +1 ���� �����ϰ� �����ؼ� �����Ѵ�.
		numbers[ i] = ((rand() % 201) - 100) / 100.f;
		cout << numbers[ i] << endl; // �������� ����Ѵ�.
	}

	bubbleSort(numbers);

	cout << "**************" << endl;
	for (int i=0; i < 10; ++i)
	{
		cout << numbers[ i] << endl;
	}
}
