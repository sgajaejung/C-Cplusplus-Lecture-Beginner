#include <iostream>
using namespace std;

void main()
{
	int m=1;
   for (int i=10; i >= 0; --i, m+=2)
   {
	   for (int k=0; k < i; ++k)
		   cout << " ";
	   for (int k=0; k < 3; ++k)
		   cout << "*";
	   for (int k=0; k < m; ++k)
		   cout << " ";
	   for (int k=0; k < 3; ++k)
		   cout << "*";
	   cout << endl;
   }
}
