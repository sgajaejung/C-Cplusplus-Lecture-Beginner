#include <iostream>
#include <string>
using namespace std;

void main()
{
	// 스트링 배열 선언.
	string strArray[ 3];

	strArray[ 0] = "Hello";
	strArray[ 1] = " ";
	strArray[ 2] = "World";

	// 출력
	cout << strArray[ 0];
	cout << strArray[ 1];
	cout << strArray[ 2];
	cout << endl;

	// 스트링 배열 재설정.
	strArray[ 0] = "I ";
	strArray[ 1] = "am ";
	strArray[ 2] = "a Boy";

	// 출력
	cout << strArray[ 0];
	cout << strArray[ 1];
	cout << strArray[ 2];
	cout << endl;
}
