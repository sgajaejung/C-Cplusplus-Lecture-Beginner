// 자판기 프로그램
//  2014-06-13

#include <iostream>
#include <string>
#include <time.h>
#include <windows.h>
using namespace std;

string merchandise[10] = {
	"드래곤 퀘스트",
	"파이날 판타지7",
	"슈퍼마리오",
	"소닉",
};

int price[10] = {
	100000,
	150000,
	80000,
	65000,
};

int maxMerchandise = 4;

int totalMoney = 0;
int cart[10];
int cartCount = 0;


void printDisplay()
{
	cout << "-------------------------" << endl;
	cout << "--\t\t게임 자판기\t\t--" << endl;
	for (int i=0; i < maxMerchandise; ++i)
	{
		cout << "-- (" << i+1 << ") " << merchandise[ i];
		cout << " - " << price[ i] << endl;
	}
//	cout << "-- (" << maxMerchandise+1 << ") 종료" << endl;
	cout << "-------------------------" << endl;
}


void printCart()
{
	cout << "-------------------------" << endl;
	cout << "장바구니" << endl;
	for (int i=0; i < cartCount; ++i)
	{
		cout << " - " << merchandise[ cart[ i]] << endl;
	}
	cout << "-------------------------" << endl;
}

void printSoccer()
{
	cout << "    ||" << endl;
	cout << "    O" << endl;
	cout << " -- | --" << endl;
	cout << "   / /" << endl;
	cout << "   / / @" << endl;
	Sleep(800);
	system("cls");

	cout << "    ||" << endl;
	cout << "    O" << endl;
	cout << " -- | --" << endl;
	cout << "   / ㅡㅡ @" << endl;
	cout << "   /   " << endl;
	Sleep(800);
	system("cls");

	cout << "    ||" << endl;
	cout << "    O" << endl;
	cout << " -- | --" << endl;
	cout << "   / ㅡ|   @" << endl;
	cout << "   /   " << endl;
	Sleep(800);
	system("cls");

	cout << "    ||" << endl;
	cout << "    O" << endl;
	cout << " -- | --" << endl;
	cout << "   / ㅡ|   @" << endl;
	cout << "   /   " << endl;
	Sleep(800);
	system("cls");

	cout << "    ||" << endl;
	cout << "    O" << endl;
	cout << " -- | --" << endl;
	cout << "   / ㅡ|    @" << endl;
	cout << "   /   " << endl;
	Sleep(800);
	system("cls");

	cout << "    ||" << endl;
	cout << "    O" << endl;
	cout << " -- | --" << endl;
	cout << "   / ㅡ|     @" << endl;
	cout << "   /   " << endl;
	Sleep(800);
	system("cls");

	cout << "    ||" << endl;
	cout << "    O" << endl;
	cout << " -- | --" << endl;
	cout << "   / ㅡ|      @" << endl;
	cout << "   /   " << endl;
	Sleep(800);
	system("cls");


}

void main()
{
	while (1)
	{
		printSoccer();

		printDisplay();
		printCart();
		cout << "잔액 : " << totalMoney << endl;
		cout << "금액을 입력하세요" << endl;

		int money;
		cin >> money;

		cout << money << " 원이 입금 되었습니다." << endl;
		if (money <= 0)
		{
			cout << "다시 넣어주세요." << endl;
			continue;
		}

		totalMoney += money;
		cout << "잔액 : " << totalMoney << endl;

		cout << "상품을 선택하세요" << endl;
		int select;
		cin >> select;
		
		if ((select <= 0) || (select > maxMerchandise))
		{
			cout << "상품을 잘 못 선택하셨습니다." << endl;
			continue;			
		}

//		if (maxMerchandise == select)
//		{
//			cout << "자판기를 종료합니다." << endl;
//			break;
//		}

		select -= 1;
		if (price[ select] > totalMoney)
		{
			cout << "잔액이 부족합니다." << endl;
			Sleep(1000);
			system("cls");
			continue;
		}

		cout << merchandise[ select] << " 를 구매하셨습니다." << endl;

		cart[ cartCount++] = select;
		totalMoney -= price[ select];

		Sleep(1000);
		system("cls");
	}

}