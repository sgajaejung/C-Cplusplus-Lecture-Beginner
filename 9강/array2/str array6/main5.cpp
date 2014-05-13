#include <iostream>
#include <string>
using namespace std;

void main()
{
	// 문자열 테이블 생성
	string strArray[3] = { "apple", "banana", "melon" };

	// 키 입력
	string input;
	cin >> input;

	// 문자열 테이블에 있는 단어라면 메세지를 출력한다.
	bool isFind = false;
	for (int i=0; i < 3; ++i)
	{
		if (input == strArray[ i])
		{
			cout << input << " find !!!" << endl;
			isFind = true;
			break;
		}
	}

	// 못찾았으면 메세지 출력.
	if (!isFind)
	{
		cout << "nothing~" << endl;
	}
}
