
#include <iostream>
#include <string>
using namespace std;

class cA
{
public:
	cA() {} // 기본 생성자
	cA(const string &name) : m_name(name) {} // name 설정 생성자.
	cA(const cA &rhs) {
		m_name = rhs.m_name;
		data1 = rhs.data1;
		data2 = rhs.data2;
	}

	// 출력함수
	void display() const {
		cout << m_name.c_str() << endl;
	}

private:
	string m_name; // 이름
	int data1; // 데이타
	int data2;
};


// 레퍼런스를 인자로 하는 함수
// 호출 할 때의 인자 와 함수 내부에서 사용하는 a 는 같은 객체를 가르킨다.
// 즉, 주소가 같다.
void func1(const cA &a)
{
	cout << "func1" << endl;
	cout << "address of func argument a = " << &a << endl;
	a.display();
}


// cA 객체를 인자로 하는 함수
// 함수가 호출 될 때 a 객체가 복사된다. 그래서 주소가 다르다.
void func2(const cA a)
{
	cout << "func2" << endl;
	cout << "address of func argument a = " << &a << endl;
	a.display();
}


// cA 객체를 인자로 받아서, 다시 리턴하는 함수.
// 리턴되는 ret 객체는 함수가 종료되면서 복사된다.
// 즉, 내부에서 사용하는 ret 의 주소와 리턴된 객체의 주소는 다르다.
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
