#include <iostream>
#include <string>
using namespace std;

void main()
{
	// ���ڿ� ���̺� ����
	string strArray[5] = { "������", "���γ�", "������", "�տ���",  "���Ҷ�" };
	string input; 
	cin >> input; // Ű�Է�
	// ���ڿ� ���̺� �ִ� �ܾ��� �޼����� ����Ѵ�.
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
	if (!isFind) // ��ã������ �޼��� ���.
	{
		cout << "nothing~" << endl;
	}
}
