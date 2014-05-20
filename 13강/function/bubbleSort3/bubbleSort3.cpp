#include <iostream>
#include <time.h>
using namespace std;

void bubbleSort(float data[10])
{
	for (int i=0; i < 10; ++i)
	{
		for (int k=i+1; k < 10; ++k)
		{
			if (data[ i] < data[ k]) // �迭 �߿� ���� ū ���� �ִٸ�
			{											// numbers �迭�� ù��°�� �����Ѵ�.
				float tmp = data[ k]; // Swap
				data[ k] = data[ i];
				data[ i] = tmp;
			}
		}
	}
}

void display(float data[10])
{
	for (int i=0; i < 10; ++i)
	{
		cout << data[ i] << endl;
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

	// ���� ū ���� ����Ѵ�.
	cout << "**************" << endl;
	display(numbers);
}
