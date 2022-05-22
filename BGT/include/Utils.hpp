#ifndef UTILS_HPP
#define UTILS_HPP

void Swap(float* a, float* b);
void Swap(unsigned int* a, unsigned int* b);
void Swap(unsigned char* a, unsigned char* b);
float Clamp(float _val, float _min, float _max);

#endif

#ifdef UTILS_IMPLEMENTATION
#undef UTILS_IMPLEMENTATION

void Swap(float* a, float* b)
{
    float _temp = *a;
    *a = *b;
    *b = _temp;
}

void Swap(int* a, int* b)
{
    int _temp = *a;
    *a = *b;
    *b = _temp;
}

void Swap(unsigned int* a, unsigned int* b)
{
    unsigned int _temp = *a;
    *a = *b;
    *b = _temp;
}

void Swap(unsigned char* a, unsigned char* b)
{
    unsigned char _temp = *a;
    *a = *b;
    *b = _temp;
}

float Clamp(float val, float min, float max )
{
    if(val < min )
        return min;
    if(val > max)
        return max;

    return val;
}


#endif