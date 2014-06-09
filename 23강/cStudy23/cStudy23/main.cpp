
#include <iostream>
using namespace std;

void main()
{
	int numbers1[7] = {1,3,4,6,30,100,200};
	int numbers2[7] = {3,5,6,10,20,21,22};
	int numbers3[14]; //1,3,3,4,5,6,6,10,20,30

	int m=0, n=0, k=0;
	for (int i=0; i < 14; ++i)
	{
		if (numbers1[ m] < numbers2[n])
		{
			numbers3[ k] = numbers1[ m];
			++k;
			++m;
		}
		else
		{
			numbers3[ k] = numbers2[ n];
			++k;
			++n;
		}

		if (m >= 7)
		{
			for (int q=n; q < 7; ++q)
			{
				numbers3[ k] = numbers2[ q];
				++k;
			}
			break;
		}
		if (n >= 7)
		{
			for (int q=m; q < 7; ++q)
			{
				numbers3[ k] = numbers1[ q];
				++k;
			}
			break;
		}
	}
	
	for (int i=0; i < 14; ++i)
		cout << numbers3[ i] << endl;

}
