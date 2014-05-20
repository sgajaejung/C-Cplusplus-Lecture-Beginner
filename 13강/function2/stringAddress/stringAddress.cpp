#include <iostream>
#include <string>
using namespace std;

string addressBook[ 10];
int size = 0;

void addName( string name )
{
	addressBook[ size] = name;
	++size;
}

void displayAddress()
{
	cout << "display" << endl;
	for (int i=0; i < size; ++i)
	{
		cout << addressBook[ i] << endl;
	}
}

void sortAddress()
{
	for (int k=0; k < size; ++k)
	{
		for (int i=k+1; i < size; ++i)
		{
			if (addressBook[ i] < addressBook[ k]) 
			{
				string tmp = addressBook[ k]; // Swap
				addressBook[ k] = addressBook[ i];
				addressBook[ i] = tmp;
			}
		}
	}
}

void main()
{
	addName( "CCC" );
	addName( "AAA" );
	addName( "EEE" );
	addName( "BBB" );
	addName( "DDD" );
	displayAddress();
	sortAddress();
	displayAddress();
}
