#include <iostream>
#include <time.h>
using namespace std;

// 로또 값을 뽑아낸다.
void Lotto(int data[ 6])
{
	// 1~45 값을 랜덤으로 뽑는다.
	// 중복된 숫자는 없어야 한다.
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
	srand((int)time(NULL)); // 랜덤 씨드값 초기화.

	int values[6];
	Lotto(values); // 로또 돌리기.

	// 배열요소들 출력
	for (int i=0; i < 6; ++i)
	{
		cout << values[ i]+1 << endl;
	}
}
