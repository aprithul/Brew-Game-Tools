#ifndef UTILS_HPP
#define UTILS_HPP

void Utils_Swap_f(float* a, float* b);
void Utils_Swap_i(int* a, int* b);
void Utils_Swap_ui(unsigned int* a, unsigned int* b);

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

#endif