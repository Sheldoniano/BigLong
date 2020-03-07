// BigLong.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//
#include <iostream>
#include <conio.h>
#include <iomanip>  // std::setfill, std::setw
#include <chrono>
#include "dynamicArray.cpp"

using namespace std;

#ifdef _WIN32
#define LONG_LONG_MIN LLONG_MIN
#define LONG_LONG_MAX LLONG_MAX
#define ULONG_LONG_MAX ULLONG_MAX
#endif

typedef int ELEM_TYPE;
typedef long long PRODUCT_TYPE;
static const ELEM_TYPE BASE = 1000000000;
static const ELEM_TYPE UPPER_BOUND = 999999999;
static const ELEM_TYPE DIGIT_COUNT = 9;
static const ELEM_TYPE powersOfTen[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000 };

// unsigned long long max_number = 18446744073709551615
// long long max_number = 9223372036854775807 
// long max_number = 2147483647
// int max_number = 2147483647
// unsigned int max_number = 4294967295

/**********Function operations*************///3 times mores speed than div,ldiv,lldiv(int num,int den) in <cstdlib>

inline static div_t my_div2(int num, int denom)// Divide entre dos
{
	div_t result;
	result.quot = num << denom;
	result.rem = num - denom * result.quot;
	return result;
}

inline static div_t my_div(int num, int denom)
{
	div_t result;
	result.quot = num / denom;
	result.rem = num - denom * result.quot;
	return result;
}

inline static ldiv_t my_ldiv(long num, long denom)
{
	ldiv_t result;
	result.quot = num / denom;
	result.rem = num - denom * result.quot;
	return result;
}

inline static lldiv_t my_lldiv(long long num, long long denom)
{
	lldiv_t result;
	result.quot = num / denom;
	result.rem = num - denom * result.quot;
	return result;
}

/******************Class Creations***********************/
class Biglong {

	friend std::ostream& operator<<(std::ostream& s, const Biglong& n);
	friend std::istream& operator>>(std::istream& s, Biglong& val);

private:
	bool pos; // true if number is positive
	ELEM_TYPE *vector;
	//int length;// indica la cantidad de digitos que tiene el numero
	//int quantum;// indica la cantidad de paquetes de elementos de 9 digitos
	//int precision;
	//int header;
	void fromstring(const std::string& s);
	void correct(bool justCheckLeadingZeros = false, bool hasValidSign = false);
	void truncateToBase();
	bool equalizeSigns();
	void removeLeadingZeros();

public:
	int length;// indica la cantidad de digitos que tiene el numero
	int quantum;// indica la cantidad de paquetes de elementos de 9 digitos

	~Biglong() {
		// destructor
		delete[] this->vector;
		this->quantum = 0;
		this->length = 0;
		this->vector = nullptr;
	} // destructor
	/* constructors */
	Biglong();
	Biglong(const char* c);
	Biglong(const std::string& s);
	Biglong(int l);
	Biglong(long l);
	Biglong(long long l);
	Biglong(unsigned int l);
	Biglong(unsigned long l);
	Biglong(unsigned long long l);
	Biglong(const Biglong& l);
	/* assignment operators */
	const Biglong& operator=(const std::string& s);
	const Biglong& operator=(const char* c);
	const Biglong& operator=(int l);
	const Biglong& operator=(long l);
	const Biglong& operator=(long long l);
	const Biglong& operator=(unsigned int l);
	const Biglong& operator=(unsigned long l);
	const Biglong& operator=(unsigned long long l);
	const Biglong& operator=(const Biglong& l);
	/* digit operations */
	size_t numberOfDigits() const;
	/* operations */
	Biglong operator-() const;
	Biglong operator+(const Biglong& rhs) const;
	Biglong operator-(const Biglong& rhs) const;
	Biglong operator*(const Biglong& rhs) const;
	Biglong operator^(const Biglong& rhs) const;
	Biglong operator/(const Biglong& rhs) const; // throw
	Biglong operator%(const Biglong& rhs) const; // throw
	/* unary increment/decrement operators */
	const Biglong& operator++();
	const Biglong& operator--();
	Biglong operator++(int);
	Biglong operator--(int);
	/* asignaciones operacionales */
	const Biglong& operator*=(const Biglong& rhs);

	/* relational operations */
	bool operator==(const Biglong& rhs) const;
	bool operator!=(const Biglong& rhs) const;
	bool operator<(const Biglong& rhs) const;
	bool operator<=(const Biglong& rhs) const;
	bool operator>(const Biglong& rhs) const;
	bool operator>=(const Biglong& rhs) const;


};
/****************** Definir clases en funciones ***********************/
/* constructors */
inline Biglong::Biglong() : pos(true) // el constructor 1, inicializo los parametros pos=true;length=1 y vector =0
{
	//ELEM_TYPE* vector = new ELEM_TYPE[1];
	vector = nullptr;
	length = 0;
	quantum = 0;
}

//inline Biglong::Biglong(int _presicion) : pos(true) // el constructor 2, inicializo los parametros pos=true;length=1 y vector =0
//{
//	unsigned long long *vector = 0;
//	length = 1;
//	precision = _presicion;
//}


inline Biglong::Biglong(const char* c)
{
	fromstring(c);
}

inline Biglong::Biglong(const std::string& s) 
{
	fromstring(s);
}

inline Biglong::Biglong(int l) : pos(l >= 0)
{
	bool restauno = false;

	if (l == INT_MIN)
	{
		restauno = true;
		++l;
	}

	if (!pos)
	{
		l = -l;
	}
	div_t dt = my_div(l, BASE);
	if (dt.quot > 0){
		vector = new ELEM_TYPE[2];
		vector[0]= (ELEM_TYPE)dt.rem;
		vector[1] = (ELEM_TYPE)dt.quot;
		quantum = 2;
		length = this->numberOfDigits();
	}
	else {
		vector = new ELEM_TYPE[1];
		*vector= (ELEM_TYPE)dt.rem;
		quantum = 1;
		length = this->numberOfDigits();
	}

	if (restauno)
	{
		--*this;
	}
}

inline Biglong::Biglong(long l) : pos(l >= 0)
{
	//PROFINY_SCOPE
	bool restauno = false;
	if (l == LONG_MIN)
	{
		restauno = true;
		++l;
	}

	if (!pos)
	{
		l = -l;
	}
	ldiv_t dt = my_ldiv(l, BASE);
	if (dt.quot > 0) {
		vector = new ELEM_TYPE[2];
		vector[0] = (ELEM_TYPE)dt.rem;
		vector[1] = (ELEM_TYPE)dt.quot;
		quantum = 2;
		length = this->numberOfDigits();
	}
	else {
		vector = new ELEM_TYPE[1];
		*vector = (ELEM_TYPE)dt.rem;
		quantum = 1;
		length = this->numberOfDigits();
	}

	if (restauno)
	{
		--* this;
	}
}

inline Biglong::Biglong(long long l) : pos(l >= 0)
{
	int base;
	//PROFINY_SCOPE
	bool restauno = false;
	if (l == LONG_LONG_MIN)
	{
		restauno = true;
		++l;
	}

	if (!pos)
	{
		l = -l;
	}

	lldiv_t dt = my_lldiv(l, BASE);
	if (dt.quot == 0) {// los elementos no se pasan de la base
		vector = new ELEM_TYPE[1];
		*vector = (ELEM_TYPE)dt.rem;
		quantum = 1;
		length = this->numberOfDigits();
	}
	else {
		base= (ELEM_TYPE)dt.rem;
		l = dt.quot;
		lldiv_t dt = my_lldiv(l, BASE);
		if (dt.quot == 0){
			vector = new ELEM_TYPE[2];
			vector[0] = (ELEM_TYPE)base;
			vector[1] = (ELEM_TYPE)dt.rem;
			quantum = 2;
			length = this->numberOfDigits();
		}
		else {
			vector = new ELEM_TYPE[3];
			vector[0] = (ELEM_TYPE)base;
			vector[1] = (ELEM_TYPE)dt.rem;
			vector[2] = (ELEM_TYPE)dt.quot;
			quantum = 3;
			length = this->numberOfDigits();
		}
	}

	if (restauno)
	{
		--* this;
	}
}

inline Biglong::Biglong(unsigned int l) : pos(true)
{
	//PROFINY_SCOPE
	unsigned int quot, rem;

	quot = l / BASE;
	rem = l - BASE*quot;

	if (quot == 0) {
		vector = new ELEM_TYPE[1];
		*vector = (ELEM_TYPE)rem;
		quantum = 1;
		length = this->numberOfDigits();
	}
	else {
		vector = new ELEM_TYPE[2];
		vector[0] = (ELEM_TYPE)rem;
		vector[1] = (ELEM_TYPE)quot;
		quantum = 2;
		length = this->numberOfDigits();
	}

}

inline Biglong::Biglong(unsigned long l) : pos(true)
{
	//PROFINY_SCOPE
	unsigned long quot, rem;

	quot = l / BASE;
	rem = l - BASE * quot;

	if (quot == 0) {
		vector = new ELEM_TYPE[1];
		*vector = (ELEM_TYPE)rem;
		quantum = 1;
		length = this->numberOfDigits();
	}
	else {
		vector = new ELEM_TYPE[2];
		vector[0] = (ELEM_TYPE)rem;
		vector[1] = (ELEM_TYPE)quot;
		quantum = 2;
		length = this->numberOfDigits();
	}
}

inline Biglong::Biglong(unsigned long long l) : pos(true)
{
	//PROFINY_SCOPE
	unsigned long long quot, rem;
	int base;

	quot = l / BASE;
	rem = l - BASE * quot;

	if (quot == 0) {// los elementos no se pasan de la base
		vector = new ELEM_TYPE[1];
		*vector = (ELEM_TYPE)rem;
		quantum = 1;
		length = this->numberOfDigits();
	}
	else {
		base = (ELEM_TYPE)rem;
		l = quot;
		quot = l / BASE;
		rem = l - BASE * quot;
		if (quot == 0) {
			vector = new ELEM_TYPE[2];
			vector[0] = (ELEM_TYPE)base;
			vector[1] = (ELEM_TYPE)rem;
			quantum = 2;
			length = this->numberOfDigits();
		}
		else {
			vector = new ELEM_TYPE[3];
			vector[0] = (ELEM_TYPE)base;
			vector[1] = (ELEM_TYPE)rem;
			vector[2] = (ELEM_TYPE)quot;
			quantum = 3;
			length = this->numberOfDigits();
		}
	}
}

inline Biglong::Biglong(const Biglong& l) : pos(l.pos),length(l.length)//, vector(l.vector)
{
	// se precinde de delete[] debido a que esta es una variable auxiliar es el paso intermedio para la igualación
	this->pos = l.pos;
	this->quantum = l.quantum;
	this->length = l.length;
	this->vector = new ELEM_TYPE[this->quantum];
	memcpy(this->vector, l.vector, sizeof(ELEM_TYPE) * l.quantum);
}

inline void Biglong::fromstring(const std::string& s) {
	int cont = 0;// el puntero del vector
	pos = true;
	length = 0;
	for (int k = 0; s[k] != '\0'; ++k)// length of string of char array
		++length;
	quantum = (length % DIGIT_COUNT != 0) ? (length / DIGIT_COUNT + 1) : (length / DIGIT_COUNT);// where DIGIT_COUNT is 9 and it's for reserve memory dynamic
	vector = new ELEM_TYPE[quantum];
	int i = length;
	for (; i > DIGIT_COUNT; i -= DIGIT_COUNT)// se extraen todos los valores en bloques de 9
	{
		vector[cont] = 0;
		for (int j = i - DIGIT_COUNT; j < i ; ++j)
		{
			vector[cont] = 10 * vector[cont] + (int)s[j] - 48;
		}
		++cont;
	}

	/***************** Se complementan las cifras más significativas si length / DIGIT_COUNT no es exacto ******************/
	vector[cont] = 0;
	int j = 0;
	if (s[0] == '-')
	{
		j = 1;
		--length;
		pos = false;
	}
	for (; j < i; ++j)
	{
		vector[cont] = 10 * vector[cont] + (int)s[j] - 48;
	}
	//correct(true);// se considera que el algoritmo de guardado de datos es suficientemente preciso como para prescindir de esta función

} // verificar la parte de correct(true)

/**************************************************** Igualdades matematicas *********************************************************************/

inline const Biglong& Biglong::operator=(int l)
{
	//PROFINY_SCOPE
	bool restauno = false;
	if (l == INT_MIN)
	{
		restauno = true;
		++l;
	}
	pos = l >= 0;

	if (!pos)
	{
		l = -l;
	}
	div_t dt = my_div(l, BASE);
	if (dt.quot > 0) {
		vector = new ELEM_TYPE[2];
		vector[0] = (ELEM_TYPE)dt.rem;
		vector[1] = (ELEM_TYPE)dt.quot;
		quantum = 2;
		length = this->numberOfDigits();
	}
	else {
		vector = new ELEM_TYPE[1];
		*vector = (ELEM_TYPE)dt.rem;
		quantum = 1;
		length = this->numberOfDigits();
	}

	return restauno ? -- * this : *this;
}

inline const Biglong& Biglong::operator=(const char* c)
{
	//PROFINY_SCOPE
	fromstring(c);
	return *this;
}

inline const Biglong& Biglong::operator=(const std::string& s)
{
	//PROFINY_SCOPE
	fromstring(s);
	return *this;
}

inline const Biglong& Biglong::operator=(long l)
{
	//PROFINY_SCOPE
	bool restauno = false;
	if (l == INT_MIN)
	{
		restauno = true;
		++l;
	}

	pos = l >= 0;

	//*vector = 0;

	if (!pos)
	{
		l = -l;
	}
	ldiv_t dt = my_ldiv(l, BASE);
	if (dt.quot > 0) {
		vector = new ELEM_TYPE[2];
		vector[0] = (ELEM_TYPE)dt.rem;
		vector[1] = (ELEM_TYPE)dt.quot;
		quantum = 2;
		length = this->numberOfDigits();
	}
	else {
		vector = new ELEM_TYPE[1];
		*vector = (ELEM_TYPE)dt.rem;
		quantum = 1;
		length = this->numberOfDigits();
	}

	return restauno ? -- * this : *this;
}

inline const Biglong& Biglong::operator=(long long l)
{
	int base;
	//PROFINY_SCOPE
	bool restauno = false;
	if (l == LONG_LONG_MIN)
	{
		restauno = true;
		++l;
	}

	pos = l >= 0;

	if (!pos)
	{
		l = -l;
	}

	lldiv_t dt = my_lldiv(l, BASE);
	if (dt.quot == 0) {// los elementos no se pasan de la base
		vector = new ELEM_TYPE[1];
		*vector = (ELEM_TYPE)dt.rem;
		quantum = 1;
		length = this->numberOfDigits();
	}
	else {
		base = (ELEM_TYPE)dt.rem;
		l = dt.quot;
		lldiv_t dt = my_lldiv(l, BASE);
		if (dt.quot == 0) {
			vector = new ELEM_TYPE[2];
			vector[0] = (ELEM_TYPE)base;
			vector[1] = (ELEM_TYPE)dt.rem;
			quantum = 2;
			length = this->numberOfDigits();
		}
		else {
			vector = new ELEM_TYPE[3];
			vector[0] = (ELEM_TYPE)base;
			vector[1] = (ELEM_TYPE)dt.rem;
			vector[2] = (ELEM_TYPE)dt.quot;
			quantum = 3;
			length = this->numberOfDigits();
		}
	}

	return restauno ? -- * this : *this;
}

inline const Biglong& Biglong::operator=(unsigned int l)
{
	pos = l >= 0;

	unsigned int quot, rem;

	quot = l / BASE;
	rem = l - BASE * quot;

	if (quot == 0) {
		vector = new ELEM_TYPE[1];
		*vector = (ELEM_TYPE)rem;
		quantum = 1;
		length = this->numberOfDigits();
	}
	else {
		vector = new ELEM_TYPE[2];
		vector[0] = (ELEM_TYPE)rem;
		vector[1] = (ELEM_TYPE)quot;
		quantum = 2;
		length = this->numberOfDigits();
	}
	return *this;
}

inline const Biglong& Biglong::operator=(unsigned long l)
{
	pos = l >= 0;
	unsigned long quot, rem;

	quot = l / BASE;
	rem = l - BASE * quot;

	if (quot == 0) {
		vector = new ELEM_TYPE[1];
		*vector = (ELEM_TYPE)rem;
		quantum = 1;
		length = this->numberOfDigits();
	}
	else {
		vector = new ELEM_TYPE[2];
		vector[0] = (ELEM_TYPE)rem;
		vector[1] = (ELEM_TYPE)quot;
		quantum = 2;
		length = this->numberOfDigits();
	}
	return *this;
}

inline const Biglong& Biglong::operator=(unsigned long long l)
{
	pos = l >= 0;
	unsigned long long quot, rem;
	int base;

	quot = l / BASE;
	rem = l - BASE * quot;

	if (quot == 0) {// los elementos no se pasan de la base
		vector = new ELEM_TYPE[1];
		*vector = (ELEM_TYPE)rem;
		quantum = 1;
		length = this->numberOfDigits();
	}
	else {
		base = (ELEM_TYPE)rem;
		l = quot;
		quot = l / BASE;
		rem = l - BASE * quot;
		if (quot == 0) {
			vector = new ELEM_TYPE[2];
			vector[0] = (ELEM_TYPE)base;
			vector[1] = (ELEM_TYPE)rem;
			quantum = 2;
			length = this->numberOfDigits();
		}
		else {
			vector = new ELEM_TYPE[3];
			vector[0] = (ELEM_TYPE)base;
			vector[1] = (ELEM_TYPE)rem;
			vector[2] = (ELEM_TYPE)quot;
			quantum = 3;
			length = this->numberOfDigits();
		}
	}
	return *this;
}

inline const Biglong& Biglong::operator=(const Biglong& l)
{
	//PROFINY_SCOPE
	delete[] this->vector;
	this->pos = l.pos;
	this->quantum = l.quantum;
	this->length = l.length;
	this->vector = new ELEM_TYPE[this->quantum];
	memcpy(this->vector, l.vector, sizeof(ELEM_TYPE) * l.quantum);
	return *this;
}

/******************** digit operations **************************************/
inline size_t Biglong::numberOfDigits() const
{
	return (quantum - 1) * DIGIT_COUNT +
		(vector[quantum - 1] > 99999 ? (vector[quantum - 1] > 9999999 ? (vector[quantum - 1] > 99999999 ? 9 : 8) : (vector[quantum - 1] > 999999 ? 7 : 6)) :
		(vector[quantum - 1] > 999 ? (vector[quantum - 1] > 9999 ? 5 : 4) : (vector[quantum - 1] > 99 ? 3 : (vector[quantum - 1] > 9 ? 2 : 1))));// busqueda binaria
}

/****************************************************** Operadores matemáticos ************************************************************/

inline Biglong Biglong::operator-() const
{
	//PROFINY_SCOPE
	Biglong result = *this;
	result.pos = !pos;
	return result;
}

inline Biglong Biglong::operator+(const Biglong& rhs) const
{
	//PROFINY_SCOPE
	Biglong result;
	result.length = length > rhs.length ? length +1: rhs.length+1;
	result.quantum = result.length / DIGIT_COUNT + 1;
	result.vector = new ELEM_TYPE[result.quantum];
	memset(result.vector, 0, result.quantum * sizeof(ELEM_TYPE));

	for (size_t i = 0; i < quantum || i < rhs.quantum; ++i)
	{
		result.vector[i] = (i < quantum ? (pos ? vector[i] : -vector[i]) : 0) + (i < rhs.quantum ? (rhs.pos ? rhs.vector[i] : -rhs.vector[i]) : 0);
	}
	result.correct();
	return result;
}

inline Biglong Biglong::operator-(const Biglong& rhs) const
{
	//PROFINY_SCOPE
	Biglong result;
	result.length = length > rhs.length ? length + 1 : rhs.length + 1;
	result.quantum = result.length / DIGIT_COUNT + 1;
	result.vector = new ELEM_TYPE[result.quantum];
	memset(result.vector, 0, result.quantum * sizeof(ELEM_TYPE));
	for (size_t i = 0; i < quantum || i < rhs.quantum; ++i)
	{
		result.vector[i] = (i < quantum ? (pos ? vector[i] : -vector[i]) : 0) - (i < rhs.quantum ? (rhs.pos ? rhs.vector[i] : -rhs.vector[i]) : 0);
	}
	result.correct();
	return result;
}

inline Biglong Biglong::operator*(const Biglong& rhs) const
{
	//PROFINY_SCOPE
	Biglong result;
	result.length = length + rhs.length;
	result.quantum = result.length / DIGIT_COUNT + 1;//+1 // el valor ideal
	result.vector = new ELEM_TYPE[result.quantum];// se define el nuevo vector  (verificar)
	memset(result.vector, 0, result.quantum * sizeof(ELEM_TYPE));// setea todos los valores del vector con 0
	//result.vector[result.quantum - 1] = 0;
	PRODUCT_TYPE carry = 0;
	size_t digit = 0;
	for (;; ++digit)
	{
		lldiv_t dt = my_lldiv(carry, BASE);
		carry = dt.quot;
		if(digit< result.quantum)result.vector[digit] = (ELEM_TYPE)dt.rem;// se añade un valor fuera del rango permitido y eso ocaciona un error
		bool found = false;
		for (size_t i = digit < rhs.quantum ? 0 : digit - rhs.quantum + 1; i < quantum && i <= digit; ++i)
		{
			PRODUCT_TYPE pval = result.vector[digit] + vector[i] * (PRODUCT_TYPE)rhs.vector[digit - i];
			if (pval >= BASE || pval <= -BASE)
			{
				lldiv_t dt = my_lldiv(pval, BASE);
				carry += dt.quot;
				pval = dt.rem;
			}
			result.vector[digit] = (ELEM_TYPE)pval;
			found = true;
		}
		if (!found)
		{
			break;
		}
	}
	for (; carry > 0; ++digit)
	{
		lldiv_t dt = my_lldiv(carry, BASE);
		result.vector[digit] = (ELEM_TYPE)dt.rem;
		carry = dt.quot;
	}
	//result.correct();
	result.removeLeadingZeros();
	result.pos = (result.quantum == 1 && result.vector[0] == 0) ? true : (pos == rhs.pos);//toma al cero como numero positivo
	return result;
}

Biglong Biglong::operator^ (const Biglong& rhs) const
{
	Biglong a(*this);
	for (Biglong i = 0; i < rhs - 1; ++i)
	{
		a *=*this;
	}
	return a;
}



/* Operadores matematicos unitarios*/

inline const Biglong& Biglong::operator--() 
{
	// es equivalente a solo restar sin devolver algun elemento
	cout << "const Biglong& Biglong::operator--()" << endl;/*1*/
	//PROFINY_SCOPE
	vector[0] -= (pos ? 1 : -1);
	this->correct(false, true);
	return *this;
}

inline Biglong Biglong::operator--(int)
{
	cout << "Biglong Biglong::operator--(int)" << endl;/*1*/
	//PROFINY_SCOPE
	Biglong result = *this;
	vector[0] -= (pos ? 1 : -1);
	this->correct(false, true);
	return result;
}

inline Biglong Biglong::operator++(int)
{
	//PROFINY_SCOPE
	Biglong result = *this;
	vector[0] += (pos ? 1 : -1);
	this->correct(false, true);
	return result;
}

inline const Biglong& Biglong::operator++()
{
	//PROFINY_SCOPE
	vector[0] += (pos ? 1 : -1);
	this->correct(false, true);
	return *this;
}

inline const Biglong& Biglong::operator*=(const Biglong& rhs)
{
	//PROFINY_SCOPE
	// TODO: optimize (do not use operator*)
	*this = *this * rhs;
	return *this;
}


/**************************************************** operadores relacionales *********************************************************************/

inline bool Biglong::operator==(const Biglong& rhs) const
{
	//PROFINY_SCOPE
	if (pos != rhs.pos || quantum != rhs.quantum || length != rhs.length)
	{
		return false;
	}
	for (int i = (int)quantum - 1; i >= 0; --i)
	{
		if (vector[i] != rhs.vector[i])
		{
			return false;
		}
	}
	return true;
}

inline bool Biglong::operator!=(const Biglong& rhs) const
{
	//PROFINY_SCOPE
	if (pos != rhs.pos || quantum != rhs.quantum || length != rhs.length)
	{
		return true;
	}
	for (int i = (int)quantum - 1; i >= 0; --i)
	{
		if (vector[i] != rhs.vector[i])
		{
			return true;
		}
	}
	return false;
}

inline bool Biglong::operator<(const Biglong& rhs) const
{
	//PROFINY_SCOPE
	if (pos && !rhs.pos)
	{
		return false;
	}
	if (!pos && rhs.pos)
	{
		return true;
	}
	if (quantum > rhs.quantum)
	{
		return pos ? false : true;
	}
	if (quantum < rhs.quantum)
	{
		return pos ? true : false;
	}
	for (int i = (int)quantum - 1; i >= 0; --i)
	{
		if (vector[i] < rhs.vector[i])
		{
			return pos ? true : false;
		}
		if (vector[i] > rhs.vector[i])
		{
			return pos ? false : true;
		}
	}
	return false;
}

inline bool Biglong::operator>(const Biglong& rhs) const
{
	//PROFINY_SCOPE
	if (pos && !rhs.pos)
	{
		return true;
	}
	if (!pos && rhs.pos)
	{
		return false;
	}
	if (quantum > rhs.quantum)
	{
		return pos ? true : false;
	}
	if (quantum < rhs.quantum)
	{
		return pos ? false : true;
	}
	for (int i = (int)quantum - 1; i >= 0; --i)
	{
		if (vector[i] < rhs.vector[i])
		{
			return pos ? false : true;
		}
		if (vector[i] > rhs.vector[i])
		{
			return pos ? true : false;
		}
	}
	return false;
}

inline bool Biglong::operator<=(const Biglong& rhs) const
{
	//PROFINY_SCOPE
	if (pos && !rhs.pos)
	{
		return false;
	}
	if (!pos && rhs.pos)
	{
		return true;
	}
	if (quantum > rhs.quantum)
	{
		return pos ? false : true;
	}
	if (quantum < rhs.quantum)
	{
		return pos ? true : false;
	}
	for (int i = (int)quantum - 1; i >= 0; --i)
	{
		if (vector[i] < rhs.vector[i])
		{
			return pos ? true : false;
		}
		if (vector[i] > rhs.vector[i])
		{
			return pos ? false : true;
		}
	}
	return true;
}

inline bool Biglong::operator>=(const Biglong& rhs) const
{
	//PROFINY_SCOPE
	if (pos && !rhs.pos)
	{
		return true;
	}
	if (!pos && rhs.pos)
	{
		return false;
	}
	if (quantum > rhs.quantum)
	{
		return pos ? true : false;
	}
	if (quantum < rhs.quantum)
	{
		return pos ? false : true;
	}
	for (int i = (int)quantum - 1; i >= 0; --i)
	{
		if (vector[i] < rhs.vector[i])
		{
			return pos ? false : true;
		}
		if (vector[i] > rhs.vector[i])
		{
			return pos ? true : false;
		}
	}
	return true;
}

/**************************************************** funciones de verificación *********************************************************************/

inline void Biglong::correct(bool justCheckLeadingZeros, bool hasValidSign)
{
	//PROFINY_SCOPE
	if (!justCheckLeadingZeros)
	{
		truncateToBase();

		if (equalizeSigns())
		{
			pos = ((quantum == 1 && vector[0] == 0) || !hasValidSign) ? true : pos;
		}
		else
		{
			pos = hasValidSign ? !pos : false;
			for (size_t i = 0; i < quantum; ++i )
			{
				vector[i] = abs(vector[i]);
			}
		}
	}

	removeLeadingZeros();
}

inline bool Biglong::equalizeSigns()
{
	//PROFINY_SCOPE
	bool isPositive = true;
	int i = (int)((quantum)) - 1;
	for (; i >= 0; --i)
	{
		if (vector[i] != 0)
		{
			isPositive = vector[i--] > 0;
			break;
		}
	}

	if (isPositive)
	{
		for (; i >= 0; --i)
		{
			if (vector[i] < 0)
			{
				int k = 0, index = i + 1;
				for (; (size_t)(index) < quantum && vector[index] == 0; ++k, ++index)
				{ // number on the left is positive
					vector[index] -= 1;
					vector[i] += BASE;
					for (; k > 0; --k)
					{
						vector[i + k] = UPPER_BOUND;
					}
				}
			}
		}
	}
	else
	{
		for (; i >= 0; --i)
		{
			if (vector[i] > 0)
			{
				int k = 0, index = i + 1;
				for (; (size_t)(index) < quantum && vector[index] == 0; ++k, ++index)
				{
					// number on the left is negative
					vector[index] += 1;
					vector[i] -= BASE;
					for (; k > 0; --k)
					{
						vector[i + k] = -UPPER_BOUND;
					}
				}
			}
		}
	}

	return isPositive;
}

inline void Biglong::truncateToBase()
{
	//PROFINY_SCOPE
	for (size_t i = 0; i < quantum; ++i) // truncate each
	{
		if (vector[i] >= BASE || vector[i] <= -BASE)
		{
			div_t dt = my_div(vector[i], BASE);
			vector[i] = dt.rem;
			if (i + 1 == quantum)//>=
			{
				ELEM_TYPE* aux = new ELEM_TYPE[quantum+1];
				copy(vector, vector + quantum, aux);//Función presente en el estandar de c++ que copia valores desde la posición de memoria de un array a otro
				delete[] vector;
				aux[quantum] = dt.quot;
				vector = aux;
				aux = nullptr;
				++length;
			}
			else
			{
				vector[i + 1] += dt.quot;
			}
		}
	}
}

inline void Biglong::removeLeadingZeros()
{
	//PROFINY_SCOPE
	if (vector[quantum - 1] != 0)
	{
		length = this->numberOfDigits();
		return;
	}
	--quantum;
	for (int i = (int)(quantum) - 1; i >= 0; --i) // remove leading 0's
	{
		if (vector[i] != 0)
		{
			quantum = i+1;
			ELEM_TYPE* aux = new ELEM_TYPE[quantum];
			copy(vector, vector + quantum, aux);
			delete[] vector;
			vector = aux;
			aux = nullptr;
			length = this->numberOfDigits();
			return;
		}
		//--quantum;
	}
	// si se llegá hasta acá implica que el vector es nulo
	quantum = quantum <= 0 ? 1 : quantum;
	ELEM_TYPE* aux = new ELEM_TYPE[quantum];
	copy(vector, vector + quantum, aux);
	delete[] vector;//si ocurre un error del tipo HEAP CORRUPTION DETECTED: es porque se está escribiendo fuera de los limites del vector,entonces al llamar a la función delete[] se presenta dicho error
	vector = nullptr;
	length = 1;
	quantum = 1;
	vector = new ELEM_TYPE[quantum];
	vector[0] = 0;
}


/**************************************************************/
/******************** NON-MEMBER OPERATORS ********************/
/**************************************************************/

inline istream& operator>>(std::istream& s, Biglong& n)
{
	//PROFINY_SCOPE
	std::string str;
	s >> str;
	n.fromstring(str);
	return s;
}

inline ostream& operator<<(std::ostream& s, const Biglong& n)
{
	// cout << "ostream& operator<<(std::ostream& s, const Biglong& n)" << endl;
	//PROFINY_SCOPE
	if (!n.pos)
	{
		s << '-';
	}
	bool first = true;
	// cout << "Resultado" << endl;//n.vector[0] << endl;
	for (int i = (int)n.quantum - 1; i >= 0; --i)
	{
		if (first)
		{
			s << n.vector[i];
			//s << "[" << n.vector[i] << "]";
			first = false;
		}
		else
		{
			s << std::setfill('0') << std::setw(DIGIT_COUNT) << n.vector[i];
			//s <<"["<< std::setfill('0') << std::setw(DIGIT_COUNT) << n.vector[i] << "]";
		}
	}
	return s;
}



/* // codigo para poder identificar si existe algun error por memoria insuficiente
bits = new (std::nothrow) char[((int *) copyMe->bits)[0]];
if (bits == NULL)
{
	cout << "ERROR Not enough memory.\n";
	exit(1);
}
*/

//
//int binaryLength(unsigned long long n)
//{
//	int i = 0; // the minimum number of bits required.
//	if (n >= 0x7FFFFFFF) { n >>= 32; i += 32; }
//	if (n >= 0x7FFF) { n >>= 16; i += 16; }
//	if (n >= 0x7F) { n >>= 8; i += 8; }
//	if (n >= 0x7) { n >>= 4; i += 4; }
//	if (n >= 0x3) { n >>= 2; i += 2; }
//	if (n >= 0x1) { n >>= 1; i += 1; }
//	return i+n;
//}

int main() {
	cout << "**************** PRUEBAS ***************" << endl;

	Biglong a,b;
	//a="3141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117450284102701938521105559644622948954930381964428810975665933446128475648233786783165271201909145648566923460348610454326648213393607260249141273724587006606315588174881520920962829254091715364367892590360011330530548820466521384146951941511609433057270365759591953092186117381932611793105118548074462379962749567351885752724891227938183011949129833673362440656643086021394946395224737190702179860943702770539217176293176752384674818467669405132000568127145263560827785771342757789609173637178721468440901224953430146549585371050792279689258923542019956112129021960864034418159813629774771309960518707211349999998372978049951059731732816096318595024459455346908302642522308253344685035261931188171010003137838752886587533208381420617177669147303598253490428755468731159562863882353787593751957781857780532171226806613001927876611195909216420198938095257201065485863278865936153381827968230301952035301852968995773622599413891249721775283479131515574857242454150695950829533116861727855889075098381754637464939319255060400927701671139009848824012858361603563707660104710181942955596198946767";
	//b="271828182845904523536028747135266249775724709369995957496696762772407663035354759457138217852516642742746639193200305992181741359662904357290033429526059563073813232862794349076323382988075319525101901157383418793070215408914993488416750924476146066808226480016847741185374234544243710753907774499206955170276183860626133138458300075204493382656029760673711320070932870912744374704723069697720931014169283681902551510865746377211125238978442505695369677078544996996794686445490598793163688923009879312773617821542499922957635148220826989519366803318252886939849646510582093923982948879332036250944311730123819706841614039701983767932068328237646480429531180232878250981945581530175671736133206981125099618188159304169035159888851934580727386673858942287922849989208680582574927961048419844436346324496848756023362482704197862320900216099023530436994184914631409343173814364054625315209618369088870701676839642437814059271456354906130310720851038375051011574770417189861068739696552126715468895703503540212340784981933432106817012100562788023519303322474501585390473041995777709350366041699732972508868769664035557071622684471625607988265178713419512466520103059212366771943252786753985589448969709640975459185695638023637016211204774272283648961342251644507818244235294863637214174023889344124796357437026375529444833799801612549227850925778256209262264832627793338656648162772516401910590049164499828931505660472580277863186415519565324425869829469593080191529872117255634754639644791014590409058629849679128740687050489585867174798546677575732056812884592054133405392200011378630094556068816674001698420558040336379537645203040243225661352783695117788386387443966253224985065499588623428189970773327617178392803494650143455889707194258639877275471096295374152111513683506275260232648472870392076431005958411661205452970302364725492966693811513732275364509888903136020572481765851180630364428123149655070475102544650117272115551948668508003685322818315219600373562527944951582841882947876108526398139559900673764829224437528718462457803619298197139914756448826260390338144182326251509748279877799643730899703888677822713836057729788241256119071766394650706330452795466185509666618566470971134447401607046262156807174818778443714369882185596709591025968620023537185887485696522000503117343920732113908032936344797273559552773490717837934216370120500545132638354400018632399149070547977805669785335804896690629511943247309958765523681285904138324116072260299833053537087613893963917795745401613722361878936526053815584158718692553860616477983402543512843961294603529133259427949043372990857315802909586313826832914771163963370924003168945863606064584592512699465572483918656420975268508230754425459937691704197778008536273094171016343490769642372229435236612557250881477922315197477806056967253801718077636034624592787784658506560507808442115296975218908740196609066518035165017925046195";

	a = 2;

	//a = "123456789123456789";
	//b = "9876543621654"; // bota cero
	//a = 123456789;

	//a = 123456;
	//b = 654321;

	//for (int i = 0; i < 100; i++) {
	//	a = a * b;
	//}
	//cout << "\n\nsiguiente iteracion a=" << a << endl;
	//for (int i = 0; i < 23; i++)
		//a =a*2;
	a = a ^ 10000;
		//33554432
	cout << a << endl;
	cout << "length=" << a.length << " quantum=" << a.quantum << endl;
	
	//div_t a; 
	//auto t1 = chrono::high_resolution_clock::now();
	//a = my_div(5, 3);
	//auto t2 = chrono::high_resolution_clock::now();
	//cout << "se demora: " << (t2 - t1) / 1ns << endl;
	//cout << "a es igual: " << a.quot << " y tambien es igual a " << a.rem << endl;;

	//char a[] = "hola mundo";
inicio:
	//n = 0;
	//for (int i = 0; a[i] != '\0'; i++)
	//{
	//	n++;
	//}
	//cout << "prueba 2 " << n << endl;
	cout << "|||Fin||||" << endl;
	_getch();
	system("CLS");
	goto inicio;
	return 0;
}

