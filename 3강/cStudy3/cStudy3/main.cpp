
#include <iostream>
using namespace std;

void main()
{
	int n1 = 100;
	int n2 = 200;

	bool flag1 = n1 > n2;
	bool flag2 = n1 >= n2;
	bool flag3 = n1 < n2;
	bool flag4 = n1 <= n2;
	bool flag5 = n1 == n2;
	bool flag6 = n1 != n2;

	cout << "n1 = " << n1 << endl;
	cout << "n2 = " << n2 << endl;
	cout << "n1 > n2 -> " << flag1 << endl;
	cout << "n1 >= n2 -> " << flag2 << endl;
	cout << flag3 << endl;
	cout << flag4 << endl;
	cout << flag5 << endl;
	cout << flag6 << endl;


	if (n1 > n2)
	{
		cout << "n1 > n2" << endl;
	}
	else
	{
		cout << "n1 <= n2" << endl;
	}

	cout << ((n1 > n2)? "n1 > n2" : "n1 <= n2") << endl;
	

	cout << "abs(n1) = " << ((n1 > 0)? n1 : -n1) << endl;


	cout << 100 / 3 << endl;
	cout << 100 % 3 << endl;
	cout << 12345 % 100 << endl;


	int bin = 1001;
	int b1 = (bin / 1000) * 8;
	int b2 = ((bin / 100) % 10) * 4;
	int b3 = ((bin / 10) % 10) * 2;
	int b4 = (bin % 10) * 1; 
	cout << "binary = " << bin << " to digit = " << b1 + b2 + b3 + b4 << endl;


	bool cond1 = true;
	bool cond2 = false;
	bool flag10 = cond1 && cond2;
	bool flag11 = cond1 || cond2;
	bool flag12 = !cond1;

	cout << "cond1 = " << cond1 << endl;
	cout << "cond2 = " << cond2 << endl;
	cout << "cond1 && cond2 = " << flag10 << endl;
	cout << "cond1 || cond2 = " << flag11 << endl;
	cout << "!cond1 = " << flag12 << endl;
	


	int count = 0;
	while (count < 10)
	{
		cout << count << endl;
		count = count + 1;
	}


	count = 1;
	while (count < 10)
	{
		cout << count * 2 << endl;
		count = count + 1;
	}


	int sum = 0;
	count = 0;
	while (count <= 10)
	{
		sum = sum + count;
		count = count + 1;
	}
	cout << "sum = " << sum << endl;



	int nn = 100;

	nn += 3;
	nn -= 2;

}
