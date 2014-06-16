// ���Ǳ� ���α׷�
//  2014-06-13

#include <iostream>
#include <string>
#include <time.h>
#include <windows.h>
using namespace std;

string merchandise[10] = {
	"�巡�� ����Ʈ",
	"���̳� ��Ÿ��7",
	"���۸�����",
	"�Ҵ�",
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
	cout << "--\t\t���� ���Ǳ�\t\t--" << endl;
	for (int i=0; i < maxMerchandise; ++i)
	{
		cout << "-- (" << i+1 << ") " << merchandise[ i];
		cout << " - " << price[ i] << endl;
	}
//	cout << "-- (" << maxMerchandise+1 << ") ����" << endl;
	cout << "-------------------------" << endl;
}


void printCart()
{
	cout << "-------------------------" << endl;
	cout << "��ٱ���" << endl;
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
	cout << "   / �Ѥ� @" << endl;
	cout << "   /   " << endl;
	Sleep(800);
	system("cls");

	cout << "    ||" << endl;
	cout << "    O" << endl;
	cout << " -- | --" << endl;
	cout << "   / ��|   @" << endl;
	cout << "   /   " << endl;
	Sleep(800);
	system("cls");

	cout << "    ||" << endl;
	cout << "    O" << endl;
	cout << " -- | --" << endl;
	cout << "   / ��|   @" << endl;
	cout << "   /   " << endl;
	Sleep(800);
	system("cls");

	cout << "    ||" << endl;
	cout << "    O" << endl;
	cout << " -- | --" << endl;
	cout << "   / ��|    @" << endl;
	cout << "   /   " << endl;
	Sleep(800);
	system("cls");

	cout << "    ||" << endl;
	cout << "    O" << endl;
	cout << " -- | --" << endl;
	cout << "   / ��|     @" << endl;
	cout << "   /   " << endl;
	Sleep(800);
	system("cls");

	cout << "    ||" << endl;
	cout << "    O" << endl;
	cout << " -- | --" << endl;
	cout << "   / ��|      @" << endl;
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
		cout << "�ܾ� : " << totalMoney << endl;
		cout << "�ݾ��� �Է��ϼ���" << endl;

		int money;
		cin >> money;

		cout << money << " ���� �Ա� �Ǿ����ϴ�." << endl;
		if (money <= 0)
		{
			cout << "�ٽ� �־��ּ���." << endl;
			continue;
		}

		totalMoney += money;
		cout << "�ܾ� : " << totalMoney << endl;

		cout << "��ǰ�� �����ϼ���" << endl;
		int select;
		cin >> select;
		
		if ((select <= 0) || (select > maxMerchandise))
		{
			cout << "��ǰ�� �� �� �����ϼ̽��ϴ�." << endl;
			continue;			
		}

//		if (maxMerchandise == select)
//		{
//			cout << "���Ǳ⸦ �����մϴ�." << endl;
//			break;
//		}

		select -= 1;
		if (price[ select] > totalMoney)
		{
			cout << "�ܾ��� �����մϴ�." << endl;
			Sleep(1000);
			system("cls");
			continue;
		}

		cout << merchandise[ select] << " �� �����ϼ̽��ϴ�." << endl;

		cart[ cartCount++] = select;
		totalMoney -= price[ select];

		Sleep(1000);
		system("cls");
	}

}