
#include <iostream>
using namespace std;

void main()
{
	int array[10];

	int count = 0;
	while (count < 10)
	{
		array[ count] = rand();
		cout << array[ count] << endl;
		++count;
	}


	int i = 0;
	int k = 0;
	while (i < 10)
	{
		if (array[ i] > k)
		{
			k = array[ i];
		}
		++i;
	}

	cout << "******" << endl;
	cout << k << endl;
}
