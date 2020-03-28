#include <iostream>
#include <stdio.h>

//template <class Type>// forma general

//using namespace std;
typedef int Type;// forma particular

class list {

private:
    
    static const Type DIGIT_COUNTS = 9;
    size_t realSize() const;
    size_t numberOfDigits() const;
    void del();// se elimina el vector
public:
    Type* vector = nullptr;// array que 
    size_t size;//cantidad de bloques
    Type length;//cantidad de digitos en cada bloque

    // Constructores
    list();
    list(const size_t size);
    list(const size_t size, Type load);
    list(const list& rvalue);
    // destructor
    ~list() {
        size = 0;
        length = 0;
        delete[] vector;
        vector = nullptr;
    }
    // Operadores
    const list& operator=(const list&& rvalue);
    void operator=(const list& rvalue);
    void operator<<(int num);
    void operator>>(int num);
    list operator+(const list& rhs);
    Type& operator[]  (const size_t& index);
    const Type operator[]  (const size_t& index) const;

    Type* expand(const size_t& size) {
        Type* new_ptr = new Type[size];
        memset(new_ptr + this->size, 0, (size - this->size) * sizeof(Type));
        while (this->size--) {
            new_ptr[this->size] = vector[this->size];
        }
        delete[] vector;
        this->size = size;
        return vector = new_ptr;
    }
};

//Constructor

inline list::list() {
    vector = nullptr;
    size = 0;
    length = 0;
}

inline list::list(size_t size) {
    this->size = size;
    this->length = 0;
    vector = new Type[size];
}

inline list::list(size_t size, Type load) {
    this->size = size;
    vector = new Type[size];
    memset(vector, load, size * sizeof(Type));
    this->length = load == 0 ? 0 : this->numberOfDigits();
}

inline list::list(const list& rvalue) {
    delete[] vector;
    this->size = rvalue.size;
    this->length = rvalue.length;
    vector = new Type[rvalue.size];
    memcpy(vector, rvalue.vector, sizeof(Type) * rvalue.size);
}

// Operadores

inline const list& list::operator= (const list&& rvalue) {
    delete[] vector;
    this->size = rvalue.size;
    vector = new Type[rvalue.size];
    memcpy(vector, rvalue.vector, sizeof(Type) * rvalue.size);
    return *this;
}

inline void list::operator=(const list& rvalue) {
    delete[] vector;
    this->size = rvalue.size;
    vector = new Type[rvalue.size];
    memcpy(vector, rvalue.vector, sizeof(Type) * rvalue.size);
}

inline list list::operator+(const list& rhs) {
    list aux(this->size);
    memcpy(aux.vector, this->vector, sizeof(int) * this->size);
    memcpy(aux.vector+ this->size, rhs.vector, sizeof(int) * rhs.size);
    return aux;
}

inline void list::operator<<(int num)
{
    list aux(this->size + num, 0);
    memcpy(aux.vector, this->vector, sizeof(int) * this->size);
    *this = aux;
}

inline void list::operator>>(int num)
{
    list aux(this->size + num, 0);
    memcpy(aux.vector + num, this->vector, sizeof(int) * this->size);
    *this = aux;
}

inline Type& list::operator[]  (const size_t& index) {
    if (index >= size) {
        expand(index + 1);
    }
    return vector[index];
}
inline const Type list::operator[]  (const size_t& index) const {
    if (index >= size) {
        return 0;
    }
    return vector[index];
}

// Funciones privadas

inline void list::del() {
    delete[] vector;
    size = 0;
    length = 0;
    vector = nullptr;
}

inline size_t list::realSize() const {
    size_t i = size - 1;
    while (vector[i] == 0 && i > 0)i--;
    return i;
}

inline size_t list::numberOfDigits() const
{
    return (size - 1) * DIGIT_COUNTS +
        (vector[size - 1] > 99999 ? (vector[size - 1] > 9999999 ? (vector[size - 1] > 99999999 ? 9 : 8) : (vector[size - 1] > 999999 ? 7 : 6)) :
        (vector[size - 1] > 999 ? (vector[size - 1] > 9999 ? 5 : 4) : (vector[size - 1] > 99 ? 3 : (vector[size - 1] > 9 ? 2 : 1))));// busqueda binaria
}





