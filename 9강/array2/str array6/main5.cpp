#include <iostream>
#include <string>
using namespace std;

void main()
{
	// ���ڿ� ���̺� ����
	string strArray[3] = { "apple", "banana", "melon" };

	// Ű �Է�
	string input;
	cin >> input;

	// ���ڿ� ���̺� �ִ� �ܾ��� �޼����� ����Ѵ�.
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

	// ��ã������ �޼��� ���.
	if (!isFind)
	{
		cout << "nothing~" << endl;
	}
}
