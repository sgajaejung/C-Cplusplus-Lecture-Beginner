#include <iostream>
#include <string>
using namespace std;

void main()
{
	const string strDNA = "GCAGAGAT"; // DNA ��Ʈ��
	string strArray[5]; // DNA�� �Է� ���� ���� ����.
	int count = 0;
	for (int i=0; i < 5; ++i, ++count) // �ִ� 5���� DNA�� �Է¹޴´�.
	{
		string str; // �Է��� ���� ������ �����Ѵ�.
		cin >> str; // DNA �Է�
		if (str == "end") // end �� �Է��ϸ� ������ �����Ѵ�.
			break;
		strArray[ i] = str; // DNA ����.
	}
	if (count <= 0) // �Էµ� DNA�� ���ٸ� �����Ѵ�.
		return;

	// �����DNA�� �ִٸ� ����Ѵ�.
	for (int i=0; i < count; ++i)
	{ // for1
		string str = strArray[ i]; // �Է¹��� DNA �ӽú����� �Ҵ�.
		for (int k=0; k < (int)strDNA.length(); ++k)			
		{ // for2
			if (str[ 0] == strDNA[ k])
			{
				bool isFind = true;
				for (int m=0; m < (int)str.length(); ++m)
				{ // for3
					if (strDNA[k + m] != str[ m])
					{
						isFind = false;
						break;
					}
				} // ~for3

				if (isFind)
				{
					cout << "find DNA = " << str << endl;
					break;
				}
			}
		} // ~for2
	} // ~for1
}
