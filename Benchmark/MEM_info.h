#ifndef MEM_INFO_H
#define MEM__INFO_H

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <math.h>

double MEM_getCapacityGB();
int MEM_getPrecentageMemInUse();
double MEM_getFreeMemGB();

#endif