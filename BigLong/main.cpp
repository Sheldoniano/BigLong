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

	//for( int x : squares )
	//for (auto x : squares)

	//list a;
	int aux[] = { 1,2,3,4,5,6,7,8,9 };
//	list a(9, { 1,2,3,4,5,6,7,8,9 });
	list a(9, aux);

	for (int i = 0; i < 10; ++i)
	{
		cout << a.vector[i] << endl;
	}
	//int a[] = { 1,2,3,4,5 };
	//int b[] = { 0,0,0,0,0,0,0,0,0,0 };
	//cout << factorial("50000");

	cout << "\n|||Fin||||" << endl;
	_getch();
	system("CLS");
	//goto inicio;
	return 0;
}
