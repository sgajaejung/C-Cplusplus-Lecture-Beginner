#include <iostream>
#include <string>
using namespace std;

void main()
{
	// 문자열 변수를 선언하면서 초기화 한다.
	string str = "0123456789";

	// 문자열 출력.
	cout << str << endl;

	// 문자열 왼쪽으로 회전.
	char k = str[ 0];
	for (int i=0; i < 10; ++i)
	{
		str[ i] = str[ i+1];
	}
	str[ 9] = k;

	// 회전된 문자열을 출력한다.
	cout << str << endl;
}
