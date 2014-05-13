#include <iostream>
#include <string>
using namespace std;

void main()
{
	string strDna = "GCAGAGAG"; // DNA 스트링
	string str;
	cin >> str; // 문자열을 입력받는다.
	if (str.empty())
		return; // 문자열이 비어있다면 종료.

	// 공통된DNA가 있다면 출력한다.
	for (int k=0; k < strDna.length(); ++k)			
	{
		if (str[ 0] == strDna[ k])
		{


		}
	}

}
