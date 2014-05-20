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
	float numbers[10]; // 실수값을 저장할 배열.
	for (int i=0; i < 10; ++i)
	{ // 실수 -1 ~ 0 ~ +1 값을 랜덤하게 생성해서 저장한다.
		numbers[ i] = ((rand() % 201) - 100) / 100.f;
		cout << numbers[ i] << endl; // 랜덤값을 출력한다.
	}

	bubbleSort(numbers);

	cout << "**************" << endl;
	for (int i=0; i < 10; ++i)
	{
		cout << numbers[ i] << endl;
	}
}
