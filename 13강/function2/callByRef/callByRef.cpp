#include <iostream>
#include <string>
using namespace std;


void func1( string str )
{
	str = "hello";
}

void func2( string strs[ 10] )
{
	strs[ 0] = "hello";
}

void main()
{
	string str1;
	func1(str1);
	cout << str1 << endl;

	string str2[10];
	func2(str2);
	cout << str2[0] << endl;
}
