#include <iostream>
#include <string>
using namespace std;

void main()
{
	// ��Ʈ�� �迭 ����
	string strArray[5] = { "abcde", "aabcd", "aaabcd", "aacdf", "ccefef" };

	for (int i=1; i < 5; ++i)
	{
		if (strArray[ 0] > strArray[ i]) // ���� ���� ��Ʈ���� ã�´�.
		{ // ����
			string tmp = strArray[ 0];
			strArray[ 0] = strArray[ i];
			strArray[ i] = tmp;
		}
	}
	// ���� ���� ��Ʈ���� ����Ѵ�.
	cout <<"minimum string = " << strArray[ 0] << endl;
}
