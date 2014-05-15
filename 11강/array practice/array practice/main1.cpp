#include <iostream>
#include <string>
using namespace std;

void main()
{
	// 스트링 배열 선언
	string strArray[5] = { "abcde", "aabcd", "aaabcd", "aacdf", "ccefef" };

	for (int i=1; i < 5; ++i)
	{
		if (strArray[ 0] > strArray[ i]) // 가장 작은 스트링을 찾는다.
		{ // 스왑
			string tmp = strArray[ 0];
			strArray[ 0] = strArray[ i];
			strArray[ i] = tmp;
		}
	}
	// 가장 작은 스트링을 출력한다.
	cout <<"minimum string = " << strArray[ 0] << endl;
}
