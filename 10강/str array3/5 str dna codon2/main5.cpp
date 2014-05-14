#include <iostream>
#include <string>
#include <time.h>
using namespace std;

void main()
{
	srand((int)time(NULL));
	float numbers[10]; // 실수값을 저장할 배열.
	for (int i=0; i < 10; ++i)
	{ // 실수 -1 ~ 0 ~ +1 값을 랜덤하게 생성해서 저장한다.
		numbers[ i] = ((rand() % 201) - 100) / 100.f;
		cout << numbers[ i] << endl; // 랜덤값을 출력한다.
	}

	float maxValue = -1.f; // 가장 큰 값을 저장할 변수.
	for (int i=0; i < 10; ++i)
	{
		if (numbers[ i] > maxValue) // 배열 중에 가장 큰 값이 있다면
		{											// maxValue 변수에 저장한다.
			maxValue = numbers[ i];
		}
	}
	// 가장 큰 값을 출력한다.
	cout << "**************" << endl;
	cout << "maximum value = " << maxValue << endl;
}
