#include <iostream>
#include <string>
using namespace std;

void main()
{
	cout << "���ϱ� ���� " << endl << "���ڸ� �Է��ϼ���" << endl;

	// ù ��° ���ڿ� �Է�
	string first;
	cin >> first;

	cout <<  " + " << endl;

	// �� ��° ���ڿ� �Է�
	string second;
	cin >> second;

	cout << " = " << endl;

	// ���ڿ��� ���ڷ� ��ȯ�ؼ� ����Ѵ�.
	int n1 = atoi(first.c_str());
	int n2 = atoi(second.c_str());

	// ��� ��� ���.
	cout << n1 + n2 << endl;
}
