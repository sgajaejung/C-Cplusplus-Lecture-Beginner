
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;


// ��������Ʈ Ŭ����.
class cUserList
{
public:
	cUserList() {} // ������.
	cUserList(const cUserList &rhs) { // ���� ������.
		operator=(rhs); // �Ҵ� �����ڷ� �ʱ�ȭ.
	}
	virtual ~cUserList() {} // �Ҹ���.


	// ���.
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


	// ���� �߰�.
	void AddUser(const string &name) {
		m_users.push_back(name);
	}


	// ���� ����.
	void RemoveUser( const string &name) {
		vector<string>::iterator it = find(m_users.begin(), m_users.end(), name);
		if (m_users.end() != it)
			m_users.erase(it);
	}


	// ������ �߰��ؼ� ����.
	cUserList operator + (const string &name) {
		cUserList users = *this;
		users.AddUser(name);
		return users;
	}


	// ������ �����ؼ� ����.
	cUserList operator - (const string &name) {
		cUserList users = *this;
		users.RemoveUser(name);
		return users;
	}


	// �ε��� ����
	// ������ �����Ѵ�.
	string operator[] (const int index) {
		return m_users[ index];
	}


	// �Ҵ� ������.
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

	// ÷�� ����.
	cout << users2[ 2].c_str() << endl;
}
