#include <iostream>
using namespace std;

void main()
{
	// 배열 선언.
	int array[ 10];

	for (int i=0; i < 10; ++i)
	{
		array[ i] = i;
	}

	// 출력
	for (int i=0; i < 10; ++i)
	{
		cout << array[ i];
	}
	cout << endl;

	// 왼쪽으로 회전
	int k = array[ 0];
	for (int i=0; i < 9; ++i)
	{
		array[ i] = array[ i+1];
	}
	array[ 9] = k;

	// 출력
	for (int i=0; i < 10; ++i)
	{
		cout << array[ i];
	}
	cout << endl;

}
