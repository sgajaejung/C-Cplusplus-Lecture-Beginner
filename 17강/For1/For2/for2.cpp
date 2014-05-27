#include <iostream>
using namespace std;

void main()
{
	for (int i=0; i < 10; ++i)
	{
		for (int k=9-i; k >= 0; --k)
		{
			cout << "*";
		}
		cout << endl;
	}
}
