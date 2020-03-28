#include <iostream>
#include <conio.h>
#include <iomanip>  // std::setfill, std::setw
#include "BigLong.cpp"

using namespace std;

//Biglong factorial(Biglong n)
//{
//	if (n == 1)return 1;
//	return n * factorial(n - 1);
//}

Biglong factorial(Biglong n)
{
	Biglong aux=1, i;
	for (i = 1; i <= n; ++i)
	{
		aux *= i;
	}
	return aux;
}

int main() {

	int aux[101] = { 0 };

	cout << aux[0];
	//list a;
	//Biglong a1;

	//for( int x : squares )
	//for (auto x : squares)

	//list a;


	//int a[] = { 1,2,3,4,5 };
	//int b[] = { 0,0,0,0,0,0,0,0,0,0 };
	//cout << factorial("50000");

	cout << "\n|||Fin||||" << endl;
	_getch();
	system("CLS");
	//goto inicio;
	return 0;
}
