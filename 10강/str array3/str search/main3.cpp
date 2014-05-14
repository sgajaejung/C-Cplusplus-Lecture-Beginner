#include <iostream>
#include <string>
using namespace std;

void main()
{
	const string strDNA = "GCAGAGAG"; // DNA 스트링
	string str;
	cin >> str; // DNA 문자열을 입력받는다.
	if (str.empty())
		return; // DNA 문자열이 비어있다면 종료.

	// 공통된DNA가 있다면 출력한다.
	for (int k=0; k < (int)strDNA.length(); ++k)			
	{
		if (str[ 0] == strDNA[ k])
		{
			bool isFind = true;
			for (int i=0; i < (int)str.length(); ++i)
			{
				if (strDNA[k + i] != str[ i])
				{
					isFind = false;
					break;
				}
			}

			if (isFind)
			{
				cout << "find DNA = " << str << endl;
				break;
			}
		}
	}
}
