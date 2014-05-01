
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;


// 유저리스트 클래스.
class cUserList
{
public:
	cUserList() {} // 생성자.
	cUserList(const cUserList &rhs) { // 복사 생성자.
		operator=(rhs); // 할당 연산자로 초기화.
	}
	virtual ~cUserList() {} // 소멸자.


	// 출력.
	void display()
	{
		cout << "display" << endl;
		vector<string>::iterator it = m_users.begin();
		while (m_users.end() != it)
		{
			cout << *it << endl;
			++it;
		}
	}


	// 유저 추가.
	void AddUser(const string &name) {
		m_users.push_back(name);
	}


	// 유저 제거.
	void RemoveUser( const string &name) {
		vector<string>::iterator it = find(m_users.begin(), m_users.end(), name);
		if (m_users.end() != it)
			m_users.erase(it);
	}


	// 유저를 추가해서 리턴.
	cUserList operator + (const string &name) {
		cUserList users = *this;
		users.AddUser(name);
		return users;
	}


	// 유저를 제거해서 리턴.
	cUserList operator - (const string &name) {
		cUserList users = *this;
		users.RemoveUser(name);
		return users;
	}


	// 인덱스 접근
	// 유저를 리턴한다.
	string operator[] (const int index) {
		return m_users[ index];
	}


	// 할당 연산자.
	const cUserList& operator = (const cUserList &rhs) {
		if (this != &rhs)
		{
			m_users = rhs.m_users;
		}
		return *this;
	}


private:
	vector<string> m_users;
};


void main()
{
	cUserList userList;
	userList.AddUser( "name 1" );
	userList.AddUser( "name 2" );
	userList.AddUser( "name 3" );
	userList.AddUser( "name 4" );
	userList.display();

	cUserList users2 = userList - "name 1";
	users2.display();

	// 첨자 접근.
	cout << users2[ 2].c_str() << endl;
}
