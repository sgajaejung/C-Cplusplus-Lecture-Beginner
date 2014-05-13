#include <iostream>
#include <string>
using namespace std;

void main()
{
	cout << "더하기 계산기 " << endl << "숫자를 입력하세요" << endl;

	// 첫 번째 문자열 입력
	string first;
	cin >> first;

	cout <<  " + " << endl;

	// 두 번째 문자열 입력
	string second;
	cin >> second;

	cout << " = " << endl;

	// 문자열을 숫자로 변환해서 계산한다.
	int n1 = atoi(first.c_str());
	int n2 = atoi(second.c_str());

	// 계산 결과 출력.
	cout << n1 + n2 << endl;
}
