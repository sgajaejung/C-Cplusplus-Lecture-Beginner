#include <iostream>
#include <string>
#include <time.h>
using namespace std;

void main()
{
	srand((int)time(NULL));
	float numbers[10]; // �Ǽ����� ������ �迭.
	for (int i=0; i < 10; ++i)
	{ // �Ǽ� -1 ~ 0 ~ +1 ���� �����ϰ� �����ؼ� �����Ѵ�.
		numbers[ i] = ((rand() % 201) - 100) / 100.f;
		cout << numbers[ i] << endl; // �������� ����Ѵ�.
	}

	for (int i=1; i < 10; ++i)
	{
		if (numbers[ i] > numbers[ 0]) // �迭 �߿� ���� ū ���� �ִٸ�
		{											// numbers �迭�� ù��°�� �����Ѵ�.
			float tmp = numbers[ 0]; // Swap
			numbers[ 0] = numbers[ i];
			numbers[ i] = tmp;
		}
	}
	// ���� ū ���� ����Ѵ�.
	cout << "**************" << endl;
	cout << "maximum value = " << numbers[ 0] << endl;
	cout << "**************" << endl;
	for (int i=0; i < 10; ++i)
	{
		cout << numbers[ i] << endl;
	}
}
