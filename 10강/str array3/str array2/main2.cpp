#include <iostream>
#include <string>
using namespace std;
void main()
{
	string strArray[5]; // ���ڿ� ���� ����.
	int count = 0;
	for (int i=0; i < 5; ++i, ++count) // �ִ� 5���� �ܾ �Է¹޴´�.
	{
		string str; // �Է��� ���� ������ �����Ѵ�.
		cin >> str; // �ܾ� �Է�
		if (str == "end") // end �� �Է��ϸ� ������ �����Ѵ�.
			break;
		strArray[ i] = str; // �ܾ� ����.
	}
	if (count <= 0) // �Էµ� �ܾ ���ٸ� �����Ѵ�.
		return;
	const string table[ 10] = { "�ں���", "�����", "���ٿ�", "�ڽ���",
		"�ż���", "������", "��ƶ�", "����", "������",
		"��ä��" };
	// �Է��� ������ ��� ���̺� ���ԵǾ��ٸ� �޼����� ����Ѵ�.
	for (int i=0; i < count; ++i)
	{
		bool isFind = false;
		for (int k=0; k < 10; ++k)
		{
			if (table[ k] == strArray[ i]) // �ܾ ��ġ�ߴٸ� ���� �ܾ�� �Ѿ��.
			{
				isFind = true;
				break;
			}
		}
		if (!isFind)
		{
			cout << "��ġ���� ���� �̸��� �ֽ��ϴ�. " << strArray[ i] << endl;
			return;
		}
	}
	cout << "��� �̸��� ��ġ�Ͽ����ϴ�. " << endl;
}
