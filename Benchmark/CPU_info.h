#ifndef CPU_INFO_H
#define CPU_INFO_H

#include <limits.h>
#include <string.h>
#include <iostream>
#include <array>
#include <vector>

typedef unsigned __int32  uint32_t;

class CPUID {
private:
    uint32_t regs[4];

    void __cpuid(int CPUInfo[4], int InfoType, int option)
    {
        __asm
        {
            mov    esi, CPUInfo
            mov    eax, InfoType
            mov    ecx, option
            cpuid
            mov    dword ptr[esi + 0], eax
            mov    dword ptr[esi + 4], ebx
            mov    dword ptr[esi + 8], ecx
            mov    dword ptr[esi + 12], edx
        }
    }

public:
    explicit CPUID(unsigned i, unsigned op = 0) {
        __cpuid((int*)regs, (int)i, op);

    }

    const uint32_t& EAX() const { return regs[0]; }
    const uint32_t& EBX() const { return regs[1]; }
    const uint32_t& ECX() const { return regs[2]; }
    const uint32_t& EDX() const { return regs[3]; }
};

const std::string CPUID_getVendor();
const std::string CPUID_getBrand();
const std::array<unsigned, 5> CPUID_getProcessorVersion();
const std::vector<std::string> CPUID_getInstructions();
const std::array<int, 3> CPUID_getTopology(const std::string& vendor);

#endif