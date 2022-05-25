#include <pspkernel.h>
#include <pspdebug.h>

PSP_MODULE_INFO("Test", 0, 1, 0);

int main()
{
    pspDebugScreenInit();
    pspDebugScreenPrintf("Hello Wordl");

}
