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

	for (int i=0; i < 10; ++i)
	{
		for (int k=i+1; k < 10; ++k)
		{
			if (numbers[ k] > numbers[ i]) // �迭 �߿� ���� ū ���� �ִٸ�
			{											// numbers �迭�� ù ��°�� �����Ѵ�.
				float tmp = numbers[ i]; // Swap
				numbers[ i] = numbers[ k];
				numbers[ k] = tmp;
			}
		}
	}

	cout << "**************" << endl;
	for (int i=0; i < 10; ++i)
	{
		cout << numbers[ i] << endl;
	}
}
