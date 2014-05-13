#include <iostream>
#include <string>
#include <sstream>
using namespace std;

void main()
{
	// 정수 변수 값 설정.
	int n1 = 100;
	int n2 = 200;

	// 정수형 타입을 문자열 타입으로 변환한다.
	stringstream ss1;
	ss1 << n1;

	stringstream ss2;
	ss2 << n2;

	// 문자열로 바뀐 숫자를 출력하고, 계산한 결과도 출력한다.
	cout << n1 << " + " << n2 << " = " << n1 + n2 << endl;
}
