#include <iostream>
#include <string>
using namespace  std;


bool FindStr(string str)
{
	const string strArray[5] = { "김태희", "유인나", "성유리", "손예진",  "강소라" };
	// 문자열 테이블에 있는 단어라면 메세지를 출력한다.
	for (int i=0; i < 5; ++i)
	{
		if (strArray[ i] == str)
		{
			cout << str << " find !!!" << endl;
			return true;
		}
	}
	return false;
}

void main()
{
	string input; 
	cin >> input; // 키입력

	if ( FindStr(input) )
	{
		cout << "nothing~" << endl;
	}
}
