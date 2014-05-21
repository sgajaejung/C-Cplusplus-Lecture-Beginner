#include <iostream>
#include <time.h>
using namespace std;

// �ζ� ���� �̾Ƴ���.
void Lotto(int data[ 6])
{
	// 1~45 ���� �������� �̴´�.
	// �ߺ��� ���ڴ� ����� �Ѵ�.
	bool check[ 45] = {false, };
	int count = 0;
	while (count < 6)
	{
		const int number = rand() % 45;
		if (check[ number])
			continue;

		data[ count] = number;
		check[ count] = true;
		++count;
	}
}

void main()
{
	srand((int)time(NULL)); // ���� ���尪 �ʱ�ȭ.

	int values[6];
	Lotto(values); // �ζ� ������.

	// �迭��ҵ� ���
	for (int i=0; i < 6; ++i)
	{
		cout << values[ i]+1 << endl;
	}
}
