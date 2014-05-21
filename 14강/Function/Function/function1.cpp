#include <iostream>
#include <string>
using namespace std;

// 함수1 정의
void function1( int n )
{
	cout << n << endl;
}

// 함수2 정의
void function2( string str )
{
	cout << str << endl;
}

// 함수3 정의
void function3( int n, float f, string str)
{
	cout << n << ", " << f << ", " << str << endl;
}

void main()
{
	int num = 100;
	function1( num ); // 함수1 호출
	function1( 100 ); // 함수1 호출
	
	string name = "Lee";
	function2(name); // 함수2 호출
	function2("Lee"); // 함수2 호출

	float value = 0.001f;
	function3(num, value, name); // 함수3 호출
	function3(100, 0.001f, "Lee"); // 함수3 호출
}

