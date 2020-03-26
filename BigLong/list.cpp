#include <iostream>
#include <stdio.h>
//#include <string.h>

//template <class Type>// forma general
typedef int Type;// forma particular

using namespace std;

class list {
public:
    Type* pointer = nullptr;// array que 
    size_t size;
    Type fill = 0;
    list(Type initVal = 0, size_t size = 1, const Type fill = 0) {
        this->size = size;
        pointer = new Type[size];
        memset(pointer, fill, size * sizeof(Type));
        pointer[0] = initVal;
        this->fill = fill;
    }
    ~list() {
        delete[] pointer;
        size = 0;
        pointer = nullptr;
    }
    void del() {
        delete[] pointer;
        size = 0;
        pointer = nullptr;
    }
    list(const list& rvalue) {
        delete[] pointer;
        this->size = rvalue.size;
        pointer = new Type[rvalue.size];
        memcpy(pointer, rvalue.pointer, sizeof(Type) * rvalue.size);
    }
    const list& operator= (const list&& rvalue) {
        delete[] pointer;
        this->size = rvalue.size;
        pointer = new Type[rvalue.size];
        memcpy(pointer, rvalue.pointer, sizeof(Type) * rvalue.size);
        return *this;
    }
    void operator=(const list& rvalue) {
        delete[] pointer;
        this->size = rvalue.size;
        pointer = new Type[rvalue.size];
        memcpy(pointer, rvalue.pointer, sizeof(Type) * rvalue.size);
    }
    inline list operator+(const list& rhs) {
        list aux(size = this->size + rhs.size, fill = 0);
        memcpy(aux.pointer, this->pointer, sizeof(int) * this->size);
        memcpy(aux.pointer, rhs.pointer, sizeof(int) * rhs.size);
        return aux;
    }
    void operator<<(int num)
    {
        list aux(size = this->size + num, fill = 0);
        memcpy(aux.pointer, this->pointer, sizeof(int) * this->size);
        *this=aux;
    }

    void operator>>(int num)
    {
        list aux(size = this->size + num, fill = 0);
        memcpy(aux.pointer + num, this->pointer, sizeof(int) * this->size);
        *this = aux;
    }

    size_t realSize() const {
        size_t i = size - 1;
        while (pointer[i] == 0 && i > 0)i--;
        return i;
    }

    Type& operator[]  (const size_t& index) {
        if (index >= size) {
            expand(index + 1);
        }
        return pointer[index];
    }
    const Type operator[]  (const size_t& index) const {
        if (index >= size) {
            return 0;
        }
        return pointer[index];
    }

    Type* expand(const size_t& size) {
        Type* new_ptr = new Type[size];
        memset(new_ptr + this->size, fill, (size - this->size) * sizeof(Type));
        while (this->size--) {
            new_ptr[this->size] = pointer[this->size];
        }
        delete[] pointer;
        this->size = size;
        return pointer = new_ptr;
    }
};