
#include <iostream>
using namespace std;

void main()
{
	// int 형 배열 선언
	// 5개의 int 타입의 값을 저장할 수 있다.
	int array[ 5];

	array[ 0] = 1; // 배열 첫 번째 요소 설정
	array[ 1] = 2; // 배열 두 번째 요소 설정
	array[ 2] = 3; // 배열 세 번째 요소 설정
	array[ 3] = 4; // 배열 네 번째 요소 설정
	array[ 4] = 5; // 배열 다섯 번째 요소 설정

	cout << array[ 0] << endl; // 배열 첫 번째 요소 출력
	cout << array[ 1] << endl; // 배열 두 번째 요소 출력
	cout << array[ 2] << endl; // 배열 세 번째 요소 출력
	cout << array[ 3] << endl; // 배열 네 번째 요소 출력
	cout << array[ 4] << endl; // 배열 다섯 번째 요소 출력

	// 배열 재 설정.
	array[ 0] = 10;
	array[ 1] = 11;
	array[ 2] = 12;
	array[ 3] = 13;
	array[ 4] = 14;

	// 배열 출력
	cout << array[ 0] << endl;
	cout << array[ 1] << endl;
	cout << array[ 2] << endl;
	cout << array[ 3] << endl;
	cout << array[ 4] << endl;

	// 값 입력
	cin >> array[ 0]; // 배열 첫 번째 요소에 값 저장
	cin >> array[ 1]; // 배열 두 번째 요소에 값 저장
	cin >> array[ 2]; // 배열 세 번째 요소에 값 저장
	cin >> array[ 3]; // 배열 네 번째 요소에 값 저장
	cin >> array[ 4]; // 배열 다섯 번째 요소에 값 저장

	// 출력.
	cout << array[ 0] * 5 << endl;
	cout << array[ 1] * 5 << endl;
	cout << array[ 2] * 5 << endl;
	cout << array[ 3] * 5 << endl;
	cout << array[ 4] * 5 << endl;

}
