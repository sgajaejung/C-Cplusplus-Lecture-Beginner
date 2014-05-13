
#include <iostream>
using namespace std;


void main()
{
	int array[2][3] = {1, 2, 3, 4,5};
	cout << array[ 1][ 1] << endl;

	int array2[][3] = {1, 2, 3, 4,5};
	cout << array2[ 1][ 1] << endl;

	int count = 0;
	int inventory[10];
	bool loop = true;
	while (loop)
	{
		cout << "(1) »ç±â" << endl;
		cout << "(2) ÆÈ±â" << endl;
		cout << "(3) ³ª°¡±â" << endl;
		int sel;
		cin >> sel;

		switch (sel)
		{
		case 1:
			{
				cout << "(1) ÀÎÇü" << endl;
				cout << "(2) ²É" << endl;
				cout << "(3) ¾È»ì²¨¾ß" << endl;
				int sel2;
				cin >> sel2;

				switch (sel2)
				{
				case 1:
					inventory[ count] = 1;
					count++;
					break;
				case 2:
					inventory[ count] = 2;
					count++;
					break;
				case 3:
					break;
				}
			}
			break;

		case 2:
			{

			}
			break;

		case 3:
			{
				cout << "»óÁ¡À» ³ª°©´Ï´Ù." << endl;
				loop = false;
			}
			break;
		}

	}

}

