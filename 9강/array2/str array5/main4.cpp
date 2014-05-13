#include <iostream>
#include <string>
using namespace std;

void main()
{
	// 상수 초기화.
	const int N = 3;
	string strArray[ N];

	 //strArray = { "hello", " ", "world" };

	 strArray[ 0] = "Hello";
	 strArray[ 1] = " ";
	 strArray[ 2] = "World";

	// 스트링 배열 출력
	cout << strArray[ 0];
	cout << strArray[ 1];
	cout << strArray[ 2];
	cout << endl;
}
