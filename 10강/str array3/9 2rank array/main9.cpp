#include <iostream>
#include <string>
using namespace std;

void main()
{
	const string strDNA = "GCAGAGAT"; // DNA 스트링
	string strArray[5]; // DNA를 입력 받을 변수 선언.
	int count = 0;
	for (int i=0; i < 5; ++i, ++count) // 최대 5개의 DNA를 입력받는다.
	{
		string str; // 입력을 받을 변수를 선언한다.
		cin >> str; // DNA 입력
		if (str == "end") // end 를 입력하면 루프를 종료한다.
			break;
		strArray[ i] = str; // DNA 저장.
	}
	if (count <= 0) // 입력된 DNA가 없다면 종료한다.
		return;

	// 공통된DNA가 있다면 출력한다.
	for (int i=0; i < count; ++i)
	{ // for1
		for (int k=0; k < (int)strDNA.length(); ++k)			
		{ // for2
			if (strArray[ i][ 0] == strDNA[ k])
			{
				bool isFind = true;
				for (int m=0; m < (int)strArray[ i].length(); ++m)
				{ // for3
					if (strDNA[k + m] != strArray[ i][ m])
					{
						isFind = false;
						break;
					}
				} // ~for3

				if (isFind)
				{
					cout << "find DNA = " << strArray[ i] << endl;
					break;
				}
			}
		} // ~for2
	} // ~for1
}
