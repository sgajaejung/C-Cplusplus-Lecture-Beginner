
#include <iostream>
using namespace std;

class cA
{
public:
	cA() {} // 기본 생성자.
	cA(int n) : m_n(n) {} // n을 초기화 하는 생성자

	// m_n 을 출력하는 함수
	// 멤버 변수가 바뀌지 않기 때문에 const 함수로 구현할 수 있다.
	void display() const {
		cout << m_n << endl;
	}

	// m_n 을 설정하는 함수
	// 멤버 변수가 바뀌기 때문에 const 함수로 만들 수 없다.
	void setN(int n) {
		m_n = n;
	}

	// m_n을 리턴하는 함수
	// 멤버 변수가 바뀌지 않기 때문에 const 함수가 가능하다.
	int getN() const { 
		return m_n;
	}

	// m_n 증가.
	void process() {
		++m_n;
	}

private:
	int m_n;
};


void func( const cA &a )
{
//	a.process(); // Error!! const 변수에서 const 함수가 아닌 것을 호출하고 있다.
}

void main()
{
	cA a(10); // cA 인스턴스 생성
	a.display();  // 출력
	a.setN(2); // 멤버 변수 설정
	a.display(); // 출력.

	func(a);
}
