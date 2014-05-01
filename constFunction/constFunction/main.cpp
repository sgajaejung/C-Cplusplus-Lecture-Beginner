
#include <iostream>
using namespace std;

class cA
{
public:
	cA() {} // �⺻ ������.
	cA(int n) : m_n(n) {} // n�� �ʱ�ȭ �ϴ� ������

	// m_n �� ����ϴ� �Լ�
	// ��� ������ �ٲ��� �ʱ� ������ const �Լ��� ������ �� �ִ�.
	void display() const {
		cout << m_n << endl;
	}

	// m_n �� �����ϴ� �Լ�
	// ��� ������ �ٲ�� ������ const �Լ��� ���� �� ����.
	void setN(int n) {
		m_n = n;
	}

	// m_n�� �����ϴ� �Լ�
	// ��� ������ �ٲ��� �ʱ� ������ const �Լ��� �����ϴ�.
	int getN() const { 
		return m_n;
	}

	// m_n ����.
	void process() {
		++m_n;
	}

private:
	int m_n;
};


void func( const cA &a )
{
//	a.process(); // Error!! const �������� const �Լ��� �ƴ� ���� ȣ���ϰ� �ִ�.
}

void main()
{
	cA a(10); // cA �ν��Ͻ� ����
	a.display();  // ���
	a.setN(2); // ��� ���� ����
	a.display(); // ���.

	func(a);
}
