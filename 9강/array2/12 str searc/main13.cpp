#include <iostream>
#include <string>
using namespace std;

void main()
{
	string strDna = "GCAGAGAG"; // DNA ��Ʈ��
	string str;
	cin >> str; // ���ڿ��� �Է¹޴´�.
	if (str.empty())
		return; // ���ڿ��� ����ִٸ� ����.

	// �����DNA�� �ִٸ� ����Ѵ�.
	for (int k=0; k < strDna.length(); ++k)			
	{
		if (str[ 0] == strDna[ k])
		{
			bool isFind = true;
			for (int i=0; i < str.length(); ++i)
			{
				if (strDna[k + i] != str[ i])
				{
					isFind = false;
					break;
				}
			}

			if (isFind)
			{
				cout << "find DNA " << str << endl;
				break;
			}
		}
	}
}
