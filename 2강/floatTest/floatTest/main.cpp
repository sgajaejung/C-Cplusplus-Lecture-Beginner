
#include <stdio.h>
#include <iostream>
using namespace std;


void main()
{
	float f1 = 10000000000.111f; // 소수점 이하 사라짐.
	float f2 = 100.111f;
	float f3 = 0.1; // warning 발생.

	printf( "%f\n", f1);
	printf( "%f\n", f2);
	printf( "%f\n", f3);

	cout << f1 << endl;
	cout << f2 << endl;
	cout << f3 << endl;

	float f4;
	cin >> f4;
	cout << f4 << endl;

	float f5;
	scanf( "%f", &f5);
	cout << f5;

}
