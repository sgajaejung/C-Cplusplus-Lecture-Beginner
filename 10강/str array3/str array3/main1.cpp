#include <iostream>
#include <string>
using namespace std;

void main()
{
	// 문자열 테이블 생성
	string strArray[5] = { "김태희", "유인나", "성유리", "손예진",  "강소라" };
	string input; 
	cin >> input; // 키입력
	// 문자열 테이블에 있는 단어라면 메세지를 출력한다.
	bool isFind = false;
	for (int i=0; i < 5; ++i)
	{
		if (input == strArray[ i])
		{
			cout << input << " find !!!" << endl;
			isFind = true;
			break;
		}
	}
	if (!isFind) // 못찾았으면 메세지 출력.
	{
		cout << "nothing~" << endl;
	}
}
