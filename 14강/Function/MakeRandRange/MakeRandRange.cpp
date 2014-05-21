#include <iostream>
#include <time.h>
using namespace std;

// 특정범위 내에서 랜덤으로 값을 채워주는 프로그램.
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
	srand((int)time(NULL)); // 랜덤 씨드값 초기화.

	int values[10];
	MakeRandRange(values, 100, 200); // 랜덤으로 값을 채운다.

	// 배열요소들 출력
	for (int i=0; i < 10; ++i)
	{
		cout << values[ i] << endl;
	}
}
