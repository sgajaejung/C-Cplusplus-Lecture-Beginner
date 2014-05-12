
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

	// 오른쪽으로 회전
	i = 8;
	int k = array[ 9];
	while (i >= 0)
	{
		array[ i+1] = array[ i];
		--i;
	}
	array[ 0] = k;

	// 출력
	i = 0;
	while (i < 10)
	{
		cout << array[ i];
		++i;
	}
	cout << endl;

}
