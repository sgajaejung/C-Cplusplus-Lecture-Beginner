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

	//오른쪽으로 회전
	int k = array[ 9];
	for (int i=8; i >= 0; --i)
	{
		array[ i+1] = array[ i];
	}
	array[ 0] = k;

	// 출력
	for (int i=0; i < 10; ++i)
	{
		cout << array[ i];
	}
	cout << endl;

}
