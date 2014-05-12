
#include <iostream>
using namespace std;

void main()
{
	// 배열 선언.
	int array[ 10];

	int i = 0;
	while (i < 10)
	{
		array[ i] = i;
		++i;
	}

	// 출력
	i = 0;
	while (i < 10)
	{
		cout << array[ i];
		++i;
	}
	cout << endl;

	// 왼쪽으로 회전
	i = 0;
	int k = array[ 0];
	while (i < 9)
	{
		array[ i] = array[ i+1];
		++i;
	}
	array[ 9] = k;

	// 출력
	i = 0;
	while (i < 10)
	{
		cout << array[ i];
		++i;
	}
	cout << endl;

}
