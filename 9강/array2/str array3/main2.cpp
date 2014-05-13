#include <iostream>
#include <string>
using namespace std;

void main()
{
	// 배열 선언.
	string strArray[ 3];

	// 유저 입력
	cin >> strArray[0];
	cin >> strArray[1];
	cin >> strArray[2];

	// 입력된 내용 출력.
	cout << "input1 = " << strArray[ 0] << endl;
	cout << "input2 = " << strArray[ 1] << endl;
	cout << "input3 = " << strArray[ 2] << endl;
}
