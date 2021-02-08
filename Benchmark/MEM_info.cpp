#include "MEM_info.h"


#define DIV 1024

static double KBtoGB(int KB)
{   
    double GB = (double) KB / (1024 * 1024);
    GB *= 10;
    return ceil(GB) / 10;
}

double MEM_getCapacityGB()
{
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);

    return KBtoGB(statex.ullTotalPhys / DIV);
}

int MEM_getPrecentageMemInUse()
{
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);

    return statex.dwMemoryLoad;
}

double MEM_getFreeMemGB()
{
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);

    return KBtoGB(statex.ullAvailPhys / DIV);
}