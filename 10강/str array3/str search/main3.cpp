#include <iostream>
#include <string>
using namespace std;

void main()
{
	const string strDNA = "GCAGAGAG"; // DNA ��Ʈ��
	string str;
	cin >> str; // DNA ���ڿ��� �Է¹޴´�.
	if (str.empty())
		return; // DNA ���ڿ��� ����ִٸ� ����.

	// �����DNA�� �ִٸ� ����Ѵ�.
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
