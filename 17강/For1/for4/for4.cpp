#include <iostream>
using namespace std;

void main()
{
	for (int i=20-1; i>= 0; i-=2)
	{
		int n = (20 - i)/2;
		for (int k=0; k < n; ++k)
		{
			cout << " ";
		}

		for (int k=0; k < i; ++k)
		{
			cout << "*";
		}

		for (int k=0; k < n; ++k)
		{
			cout << " ";
		}

		cout << endl;
	}
}

