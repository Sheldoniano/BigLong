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
    static const Type BASE = 1000000000;
    static const Type UPPER_BOUND = 999999999;
    inline static div_t my_div(int num, int denom)
    {
        div_t result;
        result.quot = num / denom;
        result.rem = num - denom * result.quot;
        return result;
    }
public:
    bool pos;// true if number is positive
    Type* pointer = nullptr;// puntero se inicializa con cero
    size_t size;// indica la cantidad de paquetes de elementos de 9 digitos
    Type length;// indica la cantidad de digitos que tiene el numero

    // Constructores
    list();
    list(const size_t size);
    list(const size_t size, Type load);
    list(size_t size, Type load, Type len);
    list(const size_t size, Type vector[]);
    list(const list& rvalue);
    // destructor
    ~list() {
        pos = false;
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
    void resize(size_t _size, int fill, Type len);

    // funciones de verificación

    void RemoveHeadZeros();// se quitan los ceros de las cabeceras para poder para contener al numero en 
    void correct(bool justCheckLeadingZeros = false, bool hasValidSign = false);
    void truncateToBase();
    bool equalizeSigns();
    /******************** digit operations **************************************/
    size_t numberOfDigits() const;
    // Funciones de medición
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
    pos = true;
}

inline list::list(size_t size) {
    this->size = size;
    this->length = 0;
    pos = true;
    pointer = new Type[size];
}

inline list::list(size_t size, Type load) {
    this->size = size;
    this->pos = load >= 0;
    pointer = new Type[size];
    memset(pointer, load, size * sizeof(Type));
    this->length = load == 0 ? 0 : this->numberOfDigits();
}

inline list::list(size_t size, Type load, Type len) {
    this->size = size;
    this->pos = load >= 0;
    pointer = new Type[size];
    memset(pointer, load, size * sizeof(Type));
    this->length = len;
}

inline list::list(const size_t _size, Type vec[]) {
    this->size = _size;
    pointer = new Type[_size];
    memcpy(pointer, vec, sizeof(Type) * _size);
    this->length = this->numberOfDigits();
}

inline list::list(const list& rvalue) {
    delete[] pointer;
    this->size = rvalue.size;
    this->length = rvalue.length;
    this->pos = rvalue.pos;
    pointer = new Type[rvalue.size];
    memcpy(pointer, rvalue.pointer, sizeof(Type) * rvalue.size);
}

// Operadores

inline const list& list::operator= (const list&& rvalue) {
    delete[] pointer;
    this->size = rvalue.size;
    this->pos = rvalue.pos;
    this->length = rvalue.length;
    pointer = new Type[rvalue.size];
    memcpy(pointer, rvalue.pointer, sizeof(Type) * rvalue.size);
    return *this;
}

inline void list::operator=(const list& rvalue) {
    delete[] pointer;
    this->size = rvalue.size;
    this->pos = rvalue.pos;
    this->length = rvalue.length;
    pointer = new Type[rvalue.size];
    memcpy(pointer, rvalue.pointer, sizeof(Type) * rvalue.size);
}

inline list list::operator+(const list& rhs) {
    list aux(this->size);
    memcpy(aux.pointer, this->pointer, sizeof(int) * this->size);
    memcpy(aux.pointer+ this->size, rhs.pointer, sizeof(int) * rhs.size);
    aux.pos = aux.pos && this->pos;
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
            memcpy(aux.pointer, this->pointer, sizeof(int) * (this->size + num));
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
    size = 1;
    length = 1;
    pointer = new Type[1];
    pointer[0] = 0;
}

inline void list::resize(size_t _size) {
    delete[] pointer;
    size = _size;
    length = 1;
    pos = true;
    pointer = new Type[_size];
}

inline void list::resize(size_t _size, int fill) {
    delete[] pointer;
    size = _size;
    pos = fill >= 0;
    pointer = new Type[_size];
    memset(pointer, fill, _size * sizeof(Type));
    length = _size == 0 ? 1 : this->numberOfDigits();
}

inline void list::resize(size_t _size, int fill, Type len) {
    delete[] pointer;
    size = _size;
    pos = fill >= 0;
    pointer = new Type[_size];
    memset(pointer, fill, _size * sizeof(Type));
    length = len;
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


/**************************************************** funciones de verificación *********************************************************************/

inline void list::RemoveHeadZeros()
{
    //remueve los ceros que se encuentran en las cifras más significativas
    long long diferencia = 1 + this->realSize() - size;
    if (diferencia != 0) {
        *this >> (long long)(1 + this->realSize() - size);
        length = this->numberOfDigits();
    }
    else {
        length = this->numberOfDigits();
    }
}

inline void list::truncateToBase()
{
    for (size_t i = 0; i < this->size; ++i) // truncate each
    {
        if (this->pointer[i] >= BASE || this->pointer[i] <= -BASE)
        {
            div_t dt = my_div(this->pointer[i], BASE);
            this->pointer[i] = dt.rem;
            if (i + 1 == this->size)//>=
            {
                *this << 1;
                this->pointer[this->size] = dt.quot;
                ++this->size;
                length=this->numberOfDigits();
            }
            else
            {
                this->pointer[i + 1] += dt.quot;
            }
        }
    }
}

inline bool list::equalizeSigns()
{
    //Al momento de restar quedan algunos bloques negativos, esto vuelve a todos los bloques positivos
    bool isPositive = true;
    int i = (int)((this->size)) - 1;
    for (; i >= 0; --i)
    {
        if (this->pointer[i] != 0)
        {
            isPositive = this->pointer[i--] > 0;
            break;
        }
    }

    if (isPositive)
    {
        for (; i >= 0; --i)
        {
            if (this->pointer[i] < 0)
            {
                int k = 0, index = i + 1;
                for (; (size_t)(index) < this->size && this->pointer[index] == 0; ++k, ++index)
                { // number on the left is positive
                    this->pointer[index] -= 1;
                    this->pointer[i] += BASE;
                    for (; k > 0; --k)
                    {
                        this->pointer[i + k] = UPPER_BOUND;
                    }
                }
            }
        }
    }
    else
    {
        for (; i >= 0; --i)
        {
            if (this->pointer[i] > 0)
            {
                int k = 0, index = i + 1;
                for (; (size_t)(index) < this->size && this->pointer[index] == 0; ++k, ++index)
                {
                    // el numero de la izquierda es negativo
                    this->pointer[index] += 1;
                    this->pointer[i] -= BASE;
                    for (; k > 0; --k)
                    {
                        this->pointer[i + k] = -UPPER_BOUND;
                    }
                }
            }
        }
    }

    return isPositive;
}

inline void list::correct(bool justCheckLeadingZeros, bool hasValidSign)
{
    // Verificador si el estado de todos las celdas son correctos
    // Por defecto, se remueven los ceros que están de la cabecera.
    // Truncan los valores de cada celda a la Base se ha definido.
    // convertir los valores negativos de las celdas a positivos sumandole su complemento y cambiando pos como False
    if (!justCheckLeadingZeros)
    {
        this->truncateToBase();
        if (this->equalizeSigns())
        {
            this->pos = ((this->size == 1 && this->pointer[0] == 0) || !hasValidSign) ? true : pos;
        }
        else
        {
            this->pos = hasValidSign ? !this->pos : false;
            for (size_t i = 0; i < this->size; ++i)
            {
                this->pointer[i] = abs(this->pointer[i]);
            }
        }
    }
    this->RemoveHeadZeros();
}


