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

	float maxValue = -1.f; // ���� ū ���� ������ ����.
	for (int i=0; i < 10; ++i)
	{
		if (numbers[ i] > maxValue) // �迭 �߿� ���� ū ���� �ִٸ�
		{											// maxValue ������ �����Ѵ�.
			maxValue = numbers[ i];
		}
	}
	// ���� ū ���� ����Ѵ�.
	cout << "**************" << endl;
	cout << "maximum value = " << maxValue << endl;
}
