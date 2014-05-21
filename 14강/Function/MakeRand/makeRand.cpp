#include <iostream>
#include <time.h>
using namespace std;

// 랜덤으로 값을 채워주는 프로그램.
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
	MakeRand(values); // 랜덤으로 값을 채운다.

	// 배열요소들 출력
	for (int i=0; i < 10; ++i)
	{
		cout << values[ i] << endl;
	}
}
