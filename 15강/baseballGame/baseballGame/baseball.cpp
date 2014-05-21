#include <iostream>
#include <time.h>
using namespace std;

int randNumber( int numbers[3] )
{
	int check[10] = {false,};
	int count = 0;

	while (count < 3)
	{
		int n = rand() % 10;
		if (check[n])
			continue;

		numbers[ count] = n;
		check[ n] = true;
		++count;
	}

	return numbers[2]*100 + numbers[1]*10 + numbers[0];
}


int GetStrikeCount(int numbers[3], int n3, int n2, int n1)
{
	int strike = 0;
	if (numbers[ 2] == n3)
		++strike;
	if (numbers[ 1] == n2)
		++strike;
	if (numbers[ 0] == n1)
		++strike;
	return strike;
}

int GetBallCount(int numbers[3], int n3, int n2, int n1)
{
	int ball = 0;
	if (numbers[ 2] != n3)
	{
		if (numbers[ 1] == n3)
			++ball;
		if (numbers[ 0] == n3)
			++ball;
	}

	if (numbers[ 1] != n2)
	{
		if (numbers[ 2] == n2)
			++ball;
		if (numbers[ 0] == n2)
			++ball;
	}

	if (numbers[ 0] != n1)
	{
		if (numbers[ 2] == n1)
			++ball;
		if (numbers[ 1] == n1)
			++ball;
	}
	return ball;
}

void main()
{
	srand((int)NULL);
	int number[3];
	randNumber( number );

	while (1)
	{
		cout << "숫자를 입력하세요~ " << endl;
		int value;
		cin >> value;

		const int n3 = value / 100;
		const int n2 = (value / 10) % 10;
		const int n1 = value % 10;

		const int strike = GetStrikeCount(number, n3, n2, n1);
		const int ball = GetBallCount(number, n3, n2, n1);

		if (strike == 3)
		{
			cout << value << " 정답!!! " << endl;
			break;
		}
		else
		{
			cout << strike << " 스트라이크, " << ball << " 볼" << endl;
		}
	}
}
