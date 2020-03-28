// Lista diseñada para enteros

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
public:
    Type* pointer = nullptr;// puntero se inicializa con cero
    size_t size;//cantidad de bloques
    Type length;//cantidad de digitos en cada bloque

    // Constructores
    list();
    list(const size_t size);
    list(const size_t size, Type load);
    list(const size_t size, Type vector[]);
    list(const list& rvalue);
    // destructor
    ~list() {
        size = 0;
        length = 0;
        delete[] pointer;
        pointer = nullptr;
    }
    // Operadores
    const list& operator=(const list&& rvalue);
    void operator=(const list& rvalue);
    void operator<<(int num);
    void operator>>(int num);
    list operator+(const list& rhs);
    Type& operator[]  (const size_t& index);
    const Type operator[]  (const size_t& index) const;

    // Funciones genericas

    void del();// se elimina el pointer
    void zero();// se elimina el pointer
    void resize(size_t _size);// se cambia el tamaño del vector
    void resize(size_t _size, int fill);// se cambia el tamaño del vector y se rellena con un valor definido
    void removeLeadingZeros();
    void correct(bool justCheckLeadingZeros = false, bool hasValidSign = false);
    void truncateToBase();
    bool equalizeSigns();
    Type* expand(const size_t& size) {
        Type* new_ptr = new Type[size];
        memset(new_ptr + this->size, 0, (size - this->size) * sizeof(Type));
        while (this->size--) {
            new_ptr[this->size] = pointer[this->size];
        }
        delete[] pointer;
        this->size = size;
        return pointer = new_ptr;
    }
};

//Constructor

inline list::list() {
    pointer = nullptr;
    size = 0;
    length = 0;
}

inline list::list(size_t size) {
    this->size = size;
    this->length = 0;
    pointer = new Type[size];
}

inline list::list(size_t size, Type load) {
    this->size = size;
    pointer = new Type[size];
    memset(pointer, load, size * sizeof(Type));
    this->length = load == 0 ? 0 : this->numberOfDigits();
}

//inline list::list(const size_t size, Type vec[]) {
//    this->size = size;
//    //for (Type x : vec) {
//
//    //}
//}

inline list::list(const list& rvalue) {
    delete[] pointer;
    this->size = rvalue.size;
    this->length = rvalue.length;
    pointer = new Type[rvalue.size];
    memcpy(pointer, rvalue.pointer, sizeof(Type) * rvalue.size);
}

// Operadores

inline const list& list::operator= (const list&& rvalue) {
    delete[] pointer;
    this->size = rvalue.size;
    pointer = new Type[rvalue.size];
    memcpy(pointer, rvalue.pointer, sizeof(Type) * rvalue.size);
    return *this;
}

inline void list::operator=(const list& rvalue) {
    delete[] pointer;
    this->size = rvalue.size;
    pointer = new Type[rvalue.size];
    memcpy(pointer, rvalue.pointer, sizeof(Type) * rvalue.size);
}

inline list list::operator+(const list& rhs) {
    list aux(this->size);
    memcpy(aux.pointer, this->pointer, sizeof(int) * this->size);
    memcpy(aux.pointer+ this->size, rhs.pointer, sizeof(int) * rhs.size);
    return aux;
}

inline void list::operator<<(int num)
{

    if (num > 0) {
        list aux(this->size + num, 0);
        memcpy(aux.pointer, this->pointer, sizeof(int) * this->size);
        *this = aux;
    }
    else if (num < 0)
    {
        if ((this->size + num) > 0)
        {
        list aux(this->size + num);
        memcpy(aux.pointer, this->pointer, sizeof(int) * (this->size + num));
        *this = aux;
        }
        else { std::cout << "Error se está tratando de quitar más elementos de los que tiene"; }
    }

}

inline void list::operator>>(int num)
{
    // cuando es positivo se crea más 
    if (num > 0) {
        list aux(this->size + num, 0);
        memcpy(aux.pointer + num, this->pointer, sizeof(int) * this->size);
        *this = aux;
    }
    else if (num < 0)
    {
        if ((this->size + num) > 0)
        {
            list aux(this->size + num);
            memcpy(aux.pointer, this->pointer - num, sizeof(int) * (this->size + num));
            *this = aux;
        }
        else { std::cout << "Error se está tratando de quitar más elementos de los que tiene"; }
    }
}

inline Type& list::operator[]  (const size_t& index) {
    if (index >= size) {
        expand(index + 1);
    }
    return pointer[index];
}
inline const Type list::operator[]  (const size_t& index) const {
    if (index >= size) {
        return 0;
    }
    return pointer[index];
}

// Funciones publicas

inline void list::del() {
    delete[] pointer;
    size = 0;
    length = 0;
    pointer = nullptr;
}

inline void list::zero() {
    delete[] pointer;
    size = 0;
    length = 1;
    pointer = new Type[1];
    pointer[0] = 0;
}

inline void list::resize(size_t _size) {
    delete[] pointer;
    size = _size;
    length = 1;
    pointer = new Type[_size];
}

inline void list::resize(size_t _size, int fill) {
    delete[] pointer;
    size = _size;
    pointer = new Type[_size];
    memset(pointer, fill, _size * sizeof(Type));
    length = _size == 0 ? 1 : this->numberOfDigits();
}

// Funciones privadas

inline size_t list::realSize() const {
    size_t i = size - 1;
    while (pointer[i] == 0 && i > 0)i--;
    return i;
}

inline size_t list::numberOfDigits() const
{
    return (size - 1) * DIGIT_COUNTS +
        (pointer[size - 1] > 99999 ? (pointer[size - 1] > 9999999 ? (pointer[size - 1] > 99999999 ? 9 : 8) : (pointer[size - 1] > 999999 ? 7 : 6)) :
        (pointer[size - 1] > 999 ? (pointer[size - 1] > 9999 ? 5 : 4) : (pointer[size - 1] > 99 ? 3 : (pointer[size - 1] > 9 ? 2 : 1))));// busqueda binaria
}

inline void list::removeLeadingZeros()
{
    //PROFINY_SCOPE
    *this >> (1 + this->realSize() - size);
    length= this->numberOfDigits();
    
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



