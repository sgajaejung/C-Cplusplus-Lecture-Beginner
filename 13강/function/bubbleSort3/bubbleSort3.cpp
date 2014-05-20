#include <iostream>
#include <time.h>
using namespace std;

void bubbleSort(float data[10])
{
	for (int i=0; i < 10; ++i)
	{
		for (int k=i+1; k < 10; ++k)
		{
			if (data[ i] < data[ k]) // 배열 중에 가장 큰 값이 있다면
			{											// numbers 배열의 첫번째에 저장한다.
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
	float numbers[10]; // 실수값을 저장할 배열.
	for (int i=0; i < 10; ++i)
	{ // 실수 -1 ~ 0 ~ +1 값을 랜덤하게 생성해서 저장한다.
		numbers[ i] = ((rand() % 201) - 100) / 100.f;
		cout << numbers[ i] << endl; // 랜덤값을 출력한다.
	}

	bubbleSort(numbers);

	// 가장 큰 값을 출력한다.
	cout << "**************" << endl;
	display(numbers);
}
