
#include <iostream>
using namespace std;

void main()
{
	// int �� �迭 ����
	// 5���� int Ÿ���� ���� ������ �� �ִ�.
	int array[ 5];

	array[ 0] = 1; // �迭 ù ��° ��� ����
	array[ 1] = 2; // �迭 �� ��° ��� ����
	array[ 2] = 3; // �迭 �� ��° ��� ����
	array[ 3] = 4; // �迭 �� ��° ��� ����
	array[ 4] = 5; // �迭 �ټ� ��° ��� ����

	cout << array[ 0] << endl; // �迭 ù ��° ��� ���
	cout << array[ 1] << endl; // �迭 �� ��° ��� ���
	cout << array[ 2] << endl; // �迭 �� ��° ��� ���
	cout << array[ 3] << endl; // �迭 �� ��° ��� ���
	cout << array[ 4] << endl; // �迭 �ټ� ��° ��� ���

	// �迭 �� ����.
	array[ 0] = 10;
	array[ 1] = 11;
	array[ 2] = 12;
	array[ 3] = 13;
	array[ 4] = 14;

	// �迭 ���
	cout << array[ 0] << endl;
	cout << array[ 1] << endl;
	cout << array[ 2] << endl;
	cout << array[ 3] << endl;
	cout << array[ 4] << endl;

	// �� �Է�
	cin >> array[ 0]; // �迭 ù ��° ��ҿ� �� ����
	cin >> array[ 1]; // �迭 �� ��° ��ҿ� �� ����
	cin >> array[ 2]; // �迭 �� ��° ��ҿ� �� ����
	cin >> array[ 3]; // �迭 �� ��° ��ҿ� �� ����
	cin >> array[ 4]; // �迭 �ټ� ��° ��ҿ� �� ����

	// ���.
	cout << array[ 0] * 5 << endl;
	cout << array[ 1] * 5 << endl;
	cout << array[ 2] * 5 << endl;
	cout << array[ 3] * 5 << endl;
	cout << array[ 4] * 5 << endl;

}
