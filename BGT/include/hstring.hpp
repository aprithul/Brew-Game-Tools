#ifndef HSTRING_HPP
#define HSTRING_HPP

#include <assert.h>

// string type declarable on the stack
struct hstring
{
    hstring();
    hstring(const char* str);
    hstring& operator=(const hstring& hstr);
    hstring& operator=(hstring&& hstr) noexcept;
    hstring(const hstring& sstr);
    hstring(hstring&& sstr) noexcept;

    ~hstring();

    void append(const char* str);
    void push(char c);
    inline const char* hstring::c_str() const
    {
        return buffer;
    }
    
    
    private:
        int length;
        int capacity;
        char* buffer;
        
        inline int get_length(const char* src)
        {
            int count = 0;
            while(src[count++] != '\0')
                ;

            return count-1;
        }
        inline void create_buffer_and_copy(const char* str)
        {
            length = get_length(str);
            if(length >= capacity)
            {
                if(buffer)
                    free(buffer);

                capacity = 2;
                while(capacity <= length)
                    capacity *= 2;
                
                buffer = (char*)malloc(capacity);
            }
            for(int i=0; i<=length; i++)
                buffer[i] = str[i];
        }
        inline void reset()
        {
            capacity = 0;
            length = 0;
            buffer = nullptr;
        }

};
// define this in one and exactly one file
#ifdef HSTRING_IMPLEMENTATION
#undef HSTRING_IMPLEMENTATION

hstring::hstring()
{
    reset();
}

hstring::hstring(const char* str)
{
    reset();
    create_buffer_and_copy(str);
}

hstring& hstring::operator=(const hstring& hstr)
{
    reset();
    create_buffer_and_copy(hstr.c_str());
    return *this;
}

hstring& hstring::operator=(hstring&& hstr) noexcept
{
    if(buffer)
        free(buffer);
    
    length = hstr.length;
    capacity = hstr.capacity;
    buffer = hstr.buffer;
    
    hstr.reset();

    return *this;
}

hstring::hstring(const hstring& hstr)
{
    reset();
    create_buffer_and_copy(hstr.c_str());
}

hstring::hstring(hstring&& hstr) noexcept
{
    capacity = hstr.capacity;
    length = hstr.length;
    buffer = hstr.buffer;

    hstr.reset();
}


hstring::~hstring()
{
    if(buffer)
        free(buffer);
}

void hstring::append(const char* str)
{
}

void hstring::push(char c)
{
}




#endif

#endif
