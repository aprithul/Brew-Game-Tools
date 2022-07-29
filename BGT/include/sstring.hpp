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


    private:
        int length;
        char buffer[SSTRING_MAX_SIZE];

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
};


// define this in one and exactly one file
#ifdef SSTRING_IMPLEMENTATION
#undef SSTRING_IMPLEMENTATION




sstring::sstring(const char* str)
{
    this->length = copy_and_calc_length(str, this->buffer);
}

sstring& sstring::operator=(const char* str)
{
    this->length = copy_and_calc_length(str, this->buffer);
    return *this;
}

sstring::sstring(const sstring& sstr)
{
    this->length = copy_and_calc_length(sstr.buffer, this->buffer);
}


void sstring::append(const char* str)
{
    int len_str = get_length(str);
    assert(len_str + length < SSTRING_MAX_SIZE);

    for(int i = 0; i <= len_str; i++)
        buffer[i+length] = str[i];
    
    length += len_str;
}

void sstring::push(char c)
{
    assert(length + 1 < SSTRING_MAX_SIZE);
    buffer[length++] = c;
    buffer[length] = '\0';
}

#endif

#endif
