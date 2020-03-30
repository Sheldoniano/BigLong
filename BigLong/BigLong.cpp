// BigLong.cpp :
//
#include <iostream>
#include <conio.h>
#include <iomanip>  // std::setfill, std::setw
#include "list.cpp"

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
	void fromstring(const std::string& s);

public:
	list vector;
	~Biglong() {
		// destructor
		vector.del();
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
inline Biglong::Biglong() // el constructor 1, inicializo los parametros pos=true;length=1 y vector =0
{
	vector.resize(1,0);
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

inline Biglong::Biglong(int l)
{
	vector.pos = l >= 0;
	bool restauno = false;
	if (l == INT_MIN)
	{
		restauno = true;
		++l;
	}

	if (!this->vector.pos)
	{
		l = -l;
	}
	div_t dt = my_div(l, BASE);
	if (dt.quot > 0){
		
		vector.resize(2);
		vector.pointer[0]= (ELEM_TYPE)dt.rem;
		vector.pointer[1] = (ELEM_TYPE)dt.quot;
		vector.length = vector.numberOfDigits();
	}
	else {
		vector.resize(1);
		vector.pointer[0]=(ELEM_TYPE)dt.rem;
		vector.length = vector.numberOfDigits();
	}

	if (restauno)
	{
		--*this;
	}
}

inline Biglong::Biglong(long l)
{
	vector.pos = l >= 0;
	bool restauno = false;
	if (l == LONG_MIN)
	{
		restauno = true;
		++l;
	}

	if (!vector.pos)
	{
		l = -l;
	}
	ldiv_t dt = my_ldiv(l, BASE);
	if (dt.quot > 0) {
		vector.resize(2);
		vector.pointer[0] = (ELEM_TYPE)dt.rem;
		vector.pointer[1] = (ELEM_TYPE)dt.quot;
		vector.length = vector.numberOfDigits();
	}
	else {
		vector.resize(1);
		vector.pointer[0] = (ELEM_TYPE)dt.rem;
		vector.length = vector.numberOfDigits();
	}

	if (restauno)
	{
		--* this;
	}
}

inline Biglong::Biglong(long long l)
{
	vector.pos = l >= 0;
	int base;
	bool restauno = false;
	if (l == LONG_LONG_MIN)
	{
		restauno = true;
		++l;
	}

	if (!vector.pos)
	{
		l = -l;
	}

	lldiv_t dt = my_lldiv(l, BASE);
	if (dt.quot == 0) {// los elementos no se pasan de la base
		vector.resize(1);
		vector.pointer[0] = (ELEM_TYPE)dt.rem;
		vector.length = vector.numberOfDigits();
	}
	else {
		base= (ELEM_TYPE)dt.rem;
		l = dt.quot;
		lldiv_t dt = my_lldiv(l, BASE);
		if (dt.quot == 0){
			vector.resize(2);
			vector.pointer[0] = (ELEM_TYPE)dt.rem;
			vector.pointer[1] = (ELEM_TYPE)dt.quot;
			vector.length = vector.numberOfDigits();
		}
		else {
			vector.resize(3);
			vector.pointer[0] = (ELEM_TYPE)base;
			vector.pointer[1] = (ELEM_TYPE)dt.rem;
			vector.pointer[2] = (ELEM_TYPE)dt.quot;
			vector.length = vector.numberOfDigits();
		}
	}

	if (restauno)
	{
		--* this;
	}
}

inline Biglong::Biglong(unsigned int l)
{
	vector.pos = true;
	unsigned int quot, rem;

	quot = l / BASE;
	rem = l - BASE*quot;

	if (quot == 0) {
		vector.resize(1);
		vector.pointer[0] = (ELEM_TYPE)rem;
		vector.length = vector.numberOfDigits();
	}
	else {
		vector.resize(2);
		vector.pointer[0] = (ELEM_TYPE)rem;
		vector.pointer[1] = (ELEM_TYPE)quot;
		vector.length = vector.numberOfDigits();
	}

}

inline Biglong::Biglong(unsigned long l)
{
	vector.pos = true;
	unsigned long quot, rem;
	quot = l / BASE;
	rem = l - BASE * quot;

	if (quot == 0) {
		vector.resize(1);
		vector.pointer[0] = (ELEM_TYPE)rem;
		vector.length = vector.numberOfDigits();
	}
	else {
		vector.resize(2);
		vector.pointer[0] = (ELEM_TYPE)rem;
		vector.pointer[1] = (ELEM_TYPE)quot;
		vector.length = vector.numberOfDigits();
	}
}

inline Biglong::Biglong(unsigned long long l)
{
	vector.pos = true;
	unsigned long long quot, rem;
	int base;
	quot = l / BASE;
	rem = l - BASE * quot;

	if (quot == 0) {// los elementos no se pasan de la base
		vector.resize(1);
		vector.pointer[0] = (ELEM_TYPE)rem;
		vector.length = vector.numberOfDigits();
	}
	else {
		base = (ELEM_TYPE)rem;
		l = quot;
		quot = l / BASE;
		rem = l - BASE * quot;
		if (quot == 0) {
			vector.resize(2);
			vector.pointer[0] = (ELEM_TYPE)base;
			vector.pointer[1] = (ELEM_TYPE)rem;
			vector.length = vector.numberOfDigits();
		}
		else {
			vector.resize(3);
			vector.pointer[0] = (ELEM_TYPE)base;
			vector.pointer[1] = (ELEM_TYPE)rem;
			vector.pointer[2] = (ELEM_TYPE)quot;
			vector.length = vector.numberOfDigits();
		}
	}
}

inline Biglong::Biglong(const Biglong& l) :vector(l.vector) {}

inline void Biglong::fromstring(const std::string& s) {
	int cont = 0;// el puntero del vector
	vector.pos = true;
	vector.length = 0;
	for (; s[vector.length] != '\0'; ++vector.length);// length of string of char array
	vector.size = (vector.length % DIGIT_COUNT != 0) ? (vector.length / DIGIT_COUNT + 1) : (vector.length / DIGIT_COUNT);// where DIGIT_COUNT is 9 and it's for reserve memory dynamic
	vector.pointer = new ELEM_TYPE[vector.size];
	int i = vector.length;
	for (; i > DIGIT_COUNT; i -= DIGIT_COUNT)// se extraen todos los valores en bloques de 9
	{
		vector.pointer[cont] = 0;
		for (int j = i - DIGIT_COUNT; j < i ; ++j)
		{
			vector.pointer[cont] = 10 * vector.pointer[cont] + (int)s[j] - 48;
		}
		++cont;
	}
	/***************** Se complementan las cifras más significativas si length / DIGIT_COUNT no es exacto ******************/
	vector.pointer[cont] = 0;
	int j = 0;
	if (s[0] == '-')
	{
		j = 1;
		--vector.length;
		vector.pos = false;
	}
	for (; j < i; ++j)
	{
		vector.pointer[cont] = 10 * vector.pointer[cont] + (int)s[j] - 48;
	}
	//correct(true);// se considera que el algoritmo de guardado de datos es suficientemente preciso como para prescindir de esta función

} // verificar la parte de correct(true)

/**************************************************** Igualdades matematicas *********************************************************************/

inline const Biglong& Biglong::operator=(int l)
{
	bool restauno = false;
	if (l == INT_MIN)
	{
		restauno = true;
		++l;
	}
	vector.pos = l >= 0;

	if (!vector.pos)
	{
		l = -l;
	}
	div_t dt = my_div(l, BASE);
	if (dt.quot > 0) {
		vector.resize(2);
		vector.pointer[0] = (ELEM_TYPE)dt.rem;
		vector.pointer[1] = (ELEM_TYPE)dt.quot;
		vector.length = vector.numberOfDigits();
	}
	else {
		vector.resize(1);
		vector.pointer[0] = (ELEM_TYPE)dt.rem;
		vector.length = vector.numberOfDigits();
	}

	return restauno ? -- * this : *this;
}

inline const Biglong& Biglong::operator=(const char* c)
{
	fromstring(c);
	return *this;
}

inline const Biglong& Biglong::operator=(const std::string& s)
{
	fromstring(s);
	return *this;
}

inline const Biglong& Biglong::operator=(long l)
{
	bool restauno = false;
	if (l == INT_MIN)
	{
		restauno = true;
		++l;
	}
	vector.pos = l >= 0;
	if (!vector.pos)
	{
		l = -l;
	}
	ldiv_t dt = my_ldiv(l, BASE);
	if (dt.quot > 0) {
		vector.resize(2);
		vector.pointer[0] = (ELEM_TYPE)dt.rem;
		vector.pointer[1] = (ELEM_TYPE)dt.quot;
		vector.length = vector.numberOfDigits();
	}
	else {
		vector.resize(1);
		vector.pointer[0] = (ELEM_TYPE)dt.rem;
		vector.length = vector.numberOfDigits();
	}

	return restauno ? -- * this : *this;
}

inline const Biglong& Biglong::operator=(long long l)
{
	int base;
	
	bool restauno = false;
	if (l == LONG_LONG_MIN)
	{
		restauno = true;
		++l;
	}

	vector.pos = l >= 0;

	if (!vector.pos)
	{
		l = -l;
	}

	lldiv_t dt = my_lldiv(l, BASE);
	if (dt.quot == 0) {// los elementos no se pasan de la base
		vector.resize(1);
		vector.pointer[0]=(ELEM_TYPE)dt.rem;
		vector.length = vector.numberOfDigits();
	}
	else {
		base = (ELEM_TYPE)dt.rem;
		l = dt.quot;
		lldiv_t dt = my_lldiv(l, BASE);
		if (dt.quot == 0) {
			vector.resize(2);
			vector.pointer[0] = (ELEM_TYPE)base;
			vector.pointer[1] = (ELEM_TYPE)dt.rem;
			vector.length = vector.numberOfDigits();
		}
		else {
			vector.resize(3);
			vector.pointer[0] = (ELEM_TYPE)base;
			vector.pointer[1] = (ELEM_TYPE)dt.rem;
			vector.pointer[2] = (ELEM_TYPE)dt.quot;
			vector.length = vector.numberOfDigits();
		}
	}

	return restauno ? -- * this : *this;
}

inline const Biglong& Biglong::operator=(unsigned int l)
{
	vector.pos = l >= 0;

	unsigned int quot, rem;

	quot = l / BASE;
	rem = l - BASE * quot;

	if (quot == 0) {
		vector.resize(1);
		vector.pointer[0] = (ELEM_TYPE)rem;
		vector.length = vector.numberOfDigits();
	}
	else {
		vector.resize(2);
		vector.pointer[0] = (ELEM_TYPE)rem;
		vector.pointer[1] = (ELEM_TYPE)quot;
		vector.length = vector.numberOfDigits();
	}
	return *this;
}

inline const Biglong& Biglong::operator=(unsigned long l)
{
	vector.pos = l >= 0;
	unsigned long quot, rem;

	quot = l / BASE;
	rem = l - BASE * quot;

	if (quot == 0) {
		vector.resize(1);
		vector.pointer[0] = (ELEM_TYPE)rem;
		vector.length = vector.numberOfDigits();
	}
	else {
		vector.resize(2);
		vector.pointer[0] = (ELEM_TYPE)rem;
		vector.pointer[1] = (ELEM_TYPE)quot;
		vector.length = vector.numberOfDigits();
	}
	return *this;
}

inline const Biglong& Biglong::operator=(unsigned long long l)
{
	vector.pos = l >= 0;
	unsigned long long quot, rem;
	int base;

	quot = l / BASE;
	rem = l - BASE * quot;

	if (quot == 0) {// los elementos no se pasan de la base
		vector.resize(1);
		vector.pointer[0] = (ELEM_TYPE)rem;
		vector.length = vector.numberOfDigits();
	}
	else {
		base = (ELEM_TYPE)rem;
		l = quot;
		quot = l / BASE;
		rem = l - BASE * quot;
		if (quot == 0) {
			vector.resize(2);
			vector.pointer[0] = (ELEM_TYPE)base;
			vector.pointer[1] = (ELEM_TYPE)rem;
			vector.length = vector.numberOfDigits();
		}
		else {
			vector.resize(3);
			vector.pointer[0] = (ELEM_TYPE)base;
			vector.pointer[1] = (ELEM_TYPE)rem;
			vector.pointer[2] = (ELEM_TYPE)quot;
			vector.length = vector.numberOfDigits();
		}
	}
	return *this;
}

inline const Biglong& Biglong::operator=(const Biglong& l)
{
	this->vector = l.vector;
	return *this;
}

/****************************************************** Operadores matemáticos ************************************************************/

inline Biglong Biglong::operator-() const
{
	Biglong result = *this;
	result.vector.pos = !vector.pos;
	return result;
}

inline Biglong Biglong::operator+(const Biglong& rhs) const
{
	Biglong result;
	result.vector.length = vector.length > rhs.vector.length ? vector.length +1: rhs.vector.length+1;
	result.vector.size = result.vector.length / DIGIT_COUNT + 1;
	result.vector.pointer = new ELEM_TYPE[result.vector.size];
	memset(result.vector.pointer, 0, result.vector.size * sizeof(ELEM_TYPE));
	for (size_t i = 0; i < vector.size || i < rhs.vector.size; ++i)
	{
		result.vector.pointer[i] = (i < vector.size ? (vector.pos ? vector.pointer[i] : -vector.pointer[i]) : 0) + (i < rhs.vector.size ? (rhs.vector.pos ? rhs.vector.pointer[i] : -rhs.vector.pointer[i]) : 0);
	}
	result.vector.correct();
	return result;
}

inline Biglong Biglong::operator-(const Biglong& rhs) const
{
	
	Biglong result;
	result.vector.length = vector.length > rhs.vector.length ? vector.length + 1 : rhs.vector.length + 1;
	result.vector.size = result.vector.length / DIGIT_COUNT + 1;
	result.vector.pointer = new ELEM_TYPE[result.vector.size];
	memset(result.vector.pointer, 0, result.vector.size * sizeof(ELEM_TYPE));
	for (size_t i = 0; i < vector.size || i < rhs.vector.size; ++i)
	{
		result.vector.pointer[i] = (i < vector.size ? (vector.pos ? vector.pointer[i] : -vector.pointer[i]) : 0) - (i < rhs.vector.size ? (rhs.vector.pos ? rhs.vector.pointer[i] : -rhs.vector.pointer[i]) : 0);
	}
	result.vector.correct();
	return result;
}

inline Biglong Biglong::operator*(const Biglong& rhs) const
{
	Biglong result;
	result.vector.resize((vector.length + rhs.vector.length) / DIGIT_COUNT + 1,0, vector.length + rhs.vector.length);//se coloca size,set fill,length
	PRODUCT_TYPE carry = 0;
	size_t digit = 0;
	for (;; ++digit)
	{
		lldiv_t dt = my_lldiv(carry, BASE);
		carry = dt.quot;
		if(digit< result.vector.size)result.vector.pointer[digit] = (ELEM_TYPE)dt.rem;// se añade un valor fuera del rango permitido y eso ocaciona un error
		bool found = false;
		for (size_t i = digit < rhs.vector.size ? 0 : digit - rhs.vector.size + 1; i < vector.size && i <= digit; ++i)
		{
			PRODUCT_TYPE pval = result.vector.pointer[digit] + vector.pointer[i] * (PRODUCT_TYPE)rhs.vector.pointer[digit - i];
			if (pval >= BASE || pval <= -BASE)
			{
				lldiv_t dt = my_lldiv(pval, BASE);
				carry += dt.quot;
				pval = dt.rem;
			}
			result.vector.pointer[digit] = (ELEM_TYPE)pval;
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
		result.vector.pointer[digit] = (ELEM_TYPE)dt.rem;
		carry = dt.quot;
	}
	result.vector.RemoveHeadZeros();
	result.vector.pos = (result.vector.size == 1 && result.vector.pointer[0] == 0) ? true : (vector.pos == rhs.vector.pos);//toma al cero como numero positivo
	return result;
}

inline Biglong Biglong::operator^ (const Biglong& rhs) const
{
	Biglong a(*this);
	for (Biglong i = 0; i < rhs - 1; ++i)
	{
		a *= *this;
	}
	return a;
}


/* Operadores matematicos unitarios*/

inline const Biglong& Biglong::operator--() 
{
	vector.pointer[0] -= (vector.pos ? 1 : -1);
	this->vector.correct(false, true);
	return *this;
}

inline Biglong Biglong::operator--(int)
{
	Biglong result = *this;
	vector.pointer[0] -= (vector.pos ? 1 : -1);
	this->vector.correct(false, true);
	return result;
}

inline Biglong Biglong::operator++(int)
{
	Biglong result = *this;
	vector.pointer[0] += (vector.pos ? 1 : -1);
	this->vector.correct(false, true);
	return result;
}

inline const Biglong& Biglong::operator++()
{
	vector.pointer[0] += (vector.pos ? 1 : -1);
	this->vector.correct(false, true);
	return *this;
}

inline const Biglong& Biglong::operator*=(const Biglong& rhs)
{
	// TODO: optimize (do not use operator*)
	*this = *this * rhs;
	return *this;
}


/**************************************************** operadores relacionales *********************************************************************/

inline bool Biglong::operator==(const Biglong& rhs) const
{
	if (vector.pos != rhs.vector.pos || vector.size != rhs.vector.size || vector.length != rhs.vector.length)
	{
		return false;
	}
	for (int i = (int)vector.size - 1; i >= 0; --i)
	{
		if (vector.pointer[i] != rhs.vector.pointer[i])
		{
			return false;
		}
	}
	return true;
}

inline bool Biglong::operator!=(const Biglong& rhs) const
{
	
	if (vector.pos != rhs.vector.pos || vector.size != rhs.vector.size || vector.length != rhs.vector.length)
	{
		return true;
	}
	for (int i = (int)vector.size - 1; i >= 0; --i)
	{
		if (vector.pointer[i] != rhs.vector.pointer[i])
		{
			return true;
		}
	}
	return false;
}

inline bool Biglong::operator<(const Biglong& rhs) const
{
	if (vector.pos && !rhs.vector.pos)
	{
		return false;
	}
	if (!vector.pos && rhs.vector.pos)
	{
		return true;
	}
	if (vector.size > rhs.vector.size)
	{
		return vector.pos ? false : true;
	}
	if (vector.size < rhs.vector.size)
	{
		return vector.pos ? true : false;
	}
	for (int i = (int)vector.size - 1; i >= 0; --i)
	{
		if (vector.pointer[i] < rhs.vector.pointer[i])
		{
			return vector.pos ? true : false;
		}
		if (vector.pointer[i] > rhs.vector.pointer[i])
		{
			return vector.pos ? false : true;
		}
	}
	return false;
}

inline bool Biglong::operator>(const Biglong& rhs) const
{
	if (vector.pos && !rhs.vector.pos)
	{
		return true;
	}
	if (!vector.pos && rhs.vector.pos)
	{
		return false;
	}
	if (vector.size > rhs.vector.size)
	{
		return vector.pos ? true : false;
	}
	if (vector.size < rhs.vector.size)
	{
		return vector.pos ? false : true;
	}
	for (int i = (int)vector.size - 1; i >= 0; --i)
	{
		if (vector.pointer[i] < rhs.vector.pointer[i])
		{
			return vector.pos ? false : true;
		}
		if (vector.pointer[i] > rhs.vector.pointer[i])
		{
			return vector.pos ? true : false;
		}
	}
	return false;
}

inline bool Biglong::operator<=(const Biglong& rhs) const
{
	if (vector.pos && !rhs.vector.pos)
	{
		return false;
	}
	if (!vector.pos && rhs.vector.pos)
	{
		return true;
	}
	if (vector.size > rhs.vector.size)
	{
		return vector.pos ? false : true;
	}
	if (vector.size < rhs.vector.size)
	{
		return vector.pos ? true : false;
	}
	for (int i = (int)vector.size - 1; i >= 0; --i)
	{
		if (vector.pointer[i] < rhs.vector.pointer[i])
		{
			return vector.pos ? true : false;
		}
		if (vector.pointer[i] > rhs.vector.pointer[i])
		{
			return vector.pos ? false : true;
		}
	}
	return true;
}

inline bool Biglong::operator>=(const Biglong& rhs) const
{
	if (vector.pos && !rhs.vector.pos)
	{
		return true;
	}
	if (!vector.pos && rhs.vector.pos)
	{
		return false;
	}
	if (vector.size > rhs.vector.size)
	{
		return vector.pos ? true : false;
	}
	if (vector.size < rhs.vector.size)
	{
		return vector.pos ? false : true;
	}
	for (int i = (int)vector.size - 1; i >= 0; --i)
	{
		if (vector.pointer[i] < rhs.vector.pointer[i])
		{
			return vector.pos ? false : true;
		}
		if (vector.pointer[i] > rhs.vector.pointer[i])
		{
			return vector.pos ? true : false;
		}
	}
	return true;
}

/**************************************************************/
/******************** NON-MEMBER OPERATORS ********************/
/**************************************************************/

inline istream& operator>>(std::istream& s, Biglong& n)
{
	
	std::string str;
	s >> str;
	n.fromstring(str);
	return s;
}

inline ostream& operator<<(std::ostream& s, const Biglong& n)
{	
	if (!n.vector.pos)
	{
		s << '-';
	}
	bool first = true;
	// cout << "Resultado" << endl;//n.vector[0] << endl;
	for (int i = (int)n.vector.size - 1; i >= 0; --i)
	{
		if (first)
		{
			s << n.vector.pointer[i];
			//s << "[" << n.vector[i] << "]";
			first = false;
		}
		else
		{
			s << std::setfill('0') << std::setw(DIGIT_COUNT) << n.vector.pointer[i];
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
