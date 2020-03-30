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
	Biglong aux=1;
	int i;
	for (i = 1; n >= i; ++i)
	{
		aux *= i;
	}
	return aux;
}

int main() {

	cout << factorial(25000);

	cout << "\n|||Fin||||" << endl;
	_getch();
	system("CLS");
	//goto inicio;
	return 0;
}
