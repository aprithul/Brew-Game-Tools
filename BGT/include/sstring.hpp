#ifndef SSTRING_HPP
#define SSTRING_HPP

#include <assert.h>
#define SSTRING_MAX_SIZE 64

// string type declarable on the stack
struct sstring
{
    sstring(const char* str);
    sstring& operator=(const char* str);
    sstring(const sstring& sstr);

    void append(const char* str);
    void push(char c);


    int _length;
    char __buffer[SSTRING_MAX_SIZE];
};


// define this in one and exactly one file
#ifdef SSTRING_IMPLEMENTATION
#undef SSTRING_IMPLEMENTATION

inline int copy_and_calc_length(const char* src, char* dest)
{
    int count = 0;
    do{
        dest[count] = src[count];
    }while(src[count++] != '\0');

    return count-1;
}

inline int get_length(const char* src)
{
    int count = 0;
    while(src[count++] != '\0')
        ;

    return count-1;
}


sstring::sstring(const char* str)
{
    this->_length = copy_and_calc_length(str, this->__buffer);
}

sstring& sstring::operator=(const char* str)
{
    this->_length = copy_and_calc_length(str, this->__buffer);
}

sstring::sstring(const sstring& sstr)
{
    this->_length = copy_and_calc_length(sstr.__buffer, this->__buffer);
}


void sstring::append(const char* str)
{
    int len_str = get_length(str);
    assert(len_str + _length < SSTRING_MAX_SIZE);

    for(int i = 0; i <= len_str; i++)
        __buffer[i+_length] = str[i];
    
    _length += len_str;
}

void sstring::push(char c)
{
    assert(_length + 1 < SSTRING_MAX_SIZE);
    __buffer[_length++] = c;
    __buffer[_length] = '\0';
}

#endif

#endif
