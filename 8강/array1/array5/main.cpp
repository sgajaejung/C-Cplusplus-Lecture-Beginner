
#include <iostream>
using namespace std;


void main()
{
	int array[10];
	
	int count = 0;
	while (count < 10)
	{
		array[ count] = rand();
		++count;
	}


	int i=0; 
	while (i < 10)
	{
		cout << array[ i] << endl;
		++i;
	}
}
