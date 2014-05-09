
#include <iostream>
using namespace std;


void main()
{
	int var1;
	float var2;

	var1 = 1;
	cout << var1 << endl;

	var1 = 2;
	cout << var1 << endl;

	var2 = 1.f;
	cout << var2 << endl;
	
	var2 = 2.f;
	cout << var2 << endl;
	
	//1 = 2; // ?

	cout << 'a' << endl;

	// 형변환
	int n1 = 10;
	int n2 = 3;
	float f = (float)n1 / (float)n2;
	cout << "f = " << f << endl;

	// warning 사라짐)
	float f1 = 10.f;
	float f2 = 3.f;
	int n = (int)(f1 / f2);
	cout << "n = " << n << endl;


	// 원둘레 프로그램 과제.

	// 상수 처리
	const int var3 = 1;
	//var3 = 2;

	//int rr = rand();
	
	int var10 = 0;
	int var20 = 1;

	if (var10 < var20)
	{
		cout << "var10 greater than var20 " << endl;
	}
	else
	{
		cout << "var10 less than var20 " << endl;
	}
	
	cout << (var10 > var20) << endl;

	bool result;
	result = true;
	result = false;
	result = var10 > var20;
	cout << result << endl;

	bool flag = true;

	result = !flag; // toggle

	if (result)
	{
		cout << "true" << endl;
	}
	else
	{
		cout << "false" << endl;
	}


	int vv1 = 10;
	int vv2 = 10;
	if (vv1 == vv2)
		cout << "vv1 == vv2" << endl;

	float ff1 = 21.3f;
	float ff2 = 21.3f;
	float ff3 = 21.29f;
	if (ff1 == ff2)
		cout << "ff1 == ff2" << endl;
	
	if (ff1 == ff3)
		cout << "ff1 == ff3" << endl;
	

	float rr = 0.008f;
	rr += 0.005f;
	rr *= 100.f;
	int nn = (int)rr;
	rr = (float)nn / 100.f;
	cout << "rr = " << rr << endl;


	float ff6 = 0.998f;
	int nn6 = (int)ff6;
	cout << nn6 << endl;

	ff6 = 0.9999998f;
	nn6 = (int)ff6;
	cout << nn6 << endl;


	int a=10, b=12, c=100;
	if (a > b)
	{
		if (a > c)
		{
			cout << a << endl;
		}
		else
		{
			cout << c << endl;
		}
	}
	else
	{
		if (b > c)
		{
			cout << b << endl;
		}
		else
		{
			cout  << c << endl;
		}
	}


}
