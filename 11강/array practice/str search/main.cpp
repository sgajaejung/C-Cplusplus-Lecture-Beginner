#include <iostream>
#include <string>
using namespace  std;


bool FindStr(string str)
{
	const string strArray[5] = { "������", "���γ�", "������", "�տ���",  "���Ҷ�" };
	// ���ڿ� ���̺� �ִ� �ܾ��� �޼����� ����Ѵ�.
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
	cin >> input; // Ű�Է�

	if ( FindStr(input) )
	{
		cout << "nothing~" << endl;
	}
}
