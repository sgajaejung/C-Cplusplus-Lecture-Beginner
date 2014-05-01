
#include <iostream>
#include <string>
using namespace std;

class cA
{
public:
	cA() {} // �⺻ ������
	cA(const string &name) : m_name(name) {} // name ���� ������.
	cA(const cA &rhs) {
		m_name = rhs.m_name;
		data1 = rhs.data1;
		data2 = rhs.data2;
	}

	// ����Լ�
	void display() const {
		cout << m_name.c_str() << endl;
	}

private:
	string m_name; // �̸�
	int data1; // ����Ÿ
	int data2;
};


// ���۷����� ���ڷ� �ϴ� �Լ�
// ȣ�� �� ���� ���� �� �Լ� ���ο��� ����ϴ� a �� ���� ��ü�� ����Ų��.
// ��, �ּҰ� ����.
void func1(const cA &a)
{
	cout << "func1" << endl;
	cout << "address of func argument a = " << &a << endl;
	a.display();
}


// cA ��ü�� ���ڷ� �ϴ� �Լ�
// �Լ��� ȣ�� �� �� a ��ü�� ����ȴ�. �׷��� �ּҰ� �ٸ���.
void func2(const cA a)
{
	cout << "func2" << endl;
	cout << "address of func argument a = " << &a << endl;
	a.display();
}


// cA ��ü�� ���ڷ� �޾Ƽ�, �ٽ� �����ϴ� �Լ�.
// ���ϵǴ� ret ��ü�� �Լ��� ����Ǹ鼭 ����ȴ�.
// ��, ���ο��� ����ϴ� ret �� �ּҿ� ���ϵ� ��ü�� �ּҴ� �ٸ���.
cA func3(const cA a)
{
	cout << "func3" << endl;
	cA ret = a;
	cout << "address of func local variable ret = " << &ret << endl;
	return ret;
}


void main()
{
	cA a("hello world cA string");
	a.display();
	cout << "address of a = " << &a << endl;

	func1(a);
	func2(a);
	cA b = func3(a);
	cout << "address of b = " << &b << endl;
}
