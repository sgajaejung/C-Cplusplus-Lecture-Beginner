#include <iostream>
#include <string>
using namespace std;

// �Լ�1 ����
void function1( int n )
{
	cout << n << endl;
}

// �Լ�2 ����
void function2( string str )
{
	cout << str << endl;
}

// �Լ�3 ����
void function3( int n, float f, string str)
{
	cout << n << ", " << f << ", " << str << endl;
}

void main()
{
	int num = 100;
	function1( num ); // �Լ�1 ȣ��
	function1( 100 ); // �Լ�1 ȣ��
	
	string name = "Lee";
	function2(name); // �Լ�2 ȣ��
	function2("Lee"); // �Լ�2 ȣ��

	float value = 0.001f;
	function3(num, value, name); // �Լ�3 ȣ��
	function3(100, 0.001f, "Lee"); // �Լ�3 ȣ��
}

