#include <iostream>
#include <string>
using namespace std;
void main()
{
	string strArray[5]; // 문자열 변수 선언.
	int count = 0;
	for (int i=0; i < 5; ++i, ++count) // 최대 5개의 단어를 입력받는다.
	{
		string str; // 입력을 받을 변수를 선언한다.
		cin >> str; // 단어 입력
		if (str == "end") // end 를 입력하면 루프를 종료한다.
			break;
		strArray[ i] = str; // 단어 저장.
	}
	if (count <= 0) // 입력된 단어가 없다면 종료한다.
		return;
	const string table[ 10] = { "박보영", "김소은", "문근영", "박신혜",
		"신세경", "백진희", "고아라", "윤아", "진세연",
		"문채원" };
	// 입력한 문장이 모두 테이블에 포함되었다면 메세지를 출력한다.
	for (int i=0; i < count; ++i)
	{
		bool isFind = false;
		for (int k=0; k < 10; ++k)
		{
			if (table[ k] == strArray[ i]) // 단어가 일치했다면 다음 단어로 넘어간다.
			{
				isFind = true;
				break;
			}
		}
		if (!isFind)
		{
			cout << "일치하지 않은 이름이 있습니다. " << strArray[ i] << endl;
			return;
		}
	}
	cout << "모든 이름이 일치하였습니다. " << endl;
}
