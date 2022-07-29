#ifndef HSTRING_HPP
#define HSTRING_HPP

#include <assert.h>

// string type declarable on the stack
struct hstring
{
    hstring(const char* str);
    hstring& operator=(const char* str);
    hstring(const hstring& sstr);
    ~hstring();

    void append(const char* str);
    void push(char c);


    int _length;
    char* __buffer = nullptr;
};
// define this in one and exactly one file
// #ifdef HSTRING_IMPLEMENTATION
// #undef HSTRING_IMPLEMENTATION

// inline int copy_and_calc_length(const char* src, char* dest)
// {
//     int count = 0;
//     do{
//         dest[count] = src[count];
//     }while(src[count++] != '\0');

//     return count-1;
// }

inline int get_length(const char* src)
{
    int count = 0;
    while(src[count++] != '\0')
        ;

    return count-1;
}


void create_buffer_and_copy(const char* str, char* dest)
{
    if(dest)
        free(dest);

    int length = get_length(str);
    malloc(length);
    for(int i=0; i<=length; i++)
        dest[i] = str[i];
}

hstring::hstring(const char* str)
{
    create_buffer_and_copy(str, __buffer);
}

hstring& hstring::operator=(const char* str)
{
    create_buffer_and_copy(str, __buffer);
    return *this;
}

hstring::hstring(const hstring& sstr)
{
    create_buffer_and_copy(sstr.__buffer, __buffer);
}

hstring::~hstring()
{
    if(__buffer)
        free(__buffer);
    _length = 0;
}

void hstring::append(const char* str)
{
    int len_str = get_length(str);
    assert(len_str + _length < SSTRING_MAX_SIZE);

    for(int i = 0; i <= len_str; i++)
        __buffer[i+_length] = str[i];
    
    _length += len_str;
}

void hstring::push(char c)
{
    assert(_length + 1 < SSTRING_MAX_SIZE);
    __buffer[_length++] = c;
    __buffer[_length] = '\0';
}

#//endif

#endif
