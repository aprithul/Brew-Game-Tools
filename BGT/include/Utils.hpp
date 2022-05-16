#ifndef UTILS_HPP
#define UTILS_HPP

void Utils_Swap_f(float* a, float* b);
void Utils_Swap_i(int* a, int* b);
void Utils_Swap_ui(unsigned int* a, unsigned int* b);
void Utils_Swap_uc(unsigned char* a, unsigned char* b);
float Utils_Clamp_f(float _val, float _min, float _max);

#endif

#ifdef UTILS_IMPLEMENTATION
#undef UTILS_IMPLEMENTATION

void Utils_Swap_f(float* a, float* b)
{
    float _temp = *a;
    *a = *b;
    *b = _temp;
}

void Utils_Swap_i(int* a, int* b)
{
    int _temp = *a;
    *a = *b;
    *b = _temp;
}

void Utils_Swap_ui(unsigned int* a, unsigned int* b)
{
    unsigned int _temp = *a;
    *a = *b;
    *b = _temp;
}

void Utils_Swap_uc(unsigned char* a, unsigned char* b)
{
    unsigned char _temp = *a;
    *a = *b;
    *b = _temp;
}

float Utils_Clamp_f(float val, float min, float max )
{
    if(val < min )
        return min;
    if(val > max)
        return max;

    return val;
}


#endif