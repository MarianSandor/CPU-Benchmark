#include "CPU_info.h"

static bool isSuported(unsigned i)
{
    CPUID cpuID(0x80000000);

    return i <= cpuID.EAX();
}

const std::string CPUID_getVendor() 
{
    CPUID cpuID(0); // Get CPU vendor

    //This returns the CPU's manufacturer ID string – a twelve-character ASCII string stored in EBX, EDX, ECX (in that order)

    std::string vendor;
    vendor += std::string((const char*)&cpuID.EBX(), 4);
    vendor += std::string((const char*)&cpuID.EDX(), 4);
    vendor += std::string((const char*)&cpuID.ECX(), 4);

    return vendor;
}

const std::array<unsigned, 5> CPUID_getProcessorVersion()
{
    std::array<unsigned, 5> proc_version{};
    CPUID cpuID(1); //Processor Version Information
 
   //This returns the CPU's stepping, model, and family information in register EAX (also called the signature of a CPU)
     
    proc_version[0] = cpuID.EAX() & 0x0000000f; // Stepping ID
    proc_version[1] = (cpuID.EAX() >> 4) & 0x0000000f; // Model
    proc_version[2] = (cpuID.EAX() >> 8) & 0x0000000f; // Family ID
    proc_version[3] = (cpuID.EAX() >> 16) & 0x0000000f; // Extended Model ID
    proc_version[4] = (cpuID.EAX() >> 20) & 0x000000ff; // Extended Family ID

    if (proc_version[2] == 6 || proc_version[2] == 15)
        proc_version[3] = (proc_version[3] << 4) + proc_version[1];
    else
        proc_version[3] = proc_version[1];

    if (proc_version[2] == 15)
        proc_version[4] = proc_version[4] + proc_version[2];
    else
        proc_version[4] = proc_version[2];

    return proc_version;
}

const std::vector<std::string> CPUID_getInstructions()
{
    std::vector<std::string> proc_instr{};
    CPUID cpuID(1); //Feature Information

    //This returns feature flags in registers EDX and ECX

    if ((cpuID.EDX() >> 23) & 0x00000001)
        proc_instr.push_back(std::string("MMX"));

    if ((cpuID.EDX() >> 25) & 0x00000001)
        proc_instr.push_back(std::string("SSE"));

    if ((cpuID.EDX() >> 26) & 0x00000001)
        proc_instr.push_back(std::string("SSE2"));

    if ((cpuID.EDX() >> 8) & 0x00000001)
        proc_instr.push_back(std::string("CX8"));

    if (cpuID.ECX() & 0x00000001)
        proc_instr.push_back(std::string("SSE3"));

    if ((cpuID.ECX() >> 9) & 0x00000001)
        proc_instr.push_back(std::string("SSSE3"));

    if ((cpuID.ECX() >> 12) & 0x00000001)
        proc_instr.push_back(std::string("FMA"));

    if ((cpuID.ECX() >> 13) & 0x00000001)
        proc_instr.push_back(std::string("CX16"));

    if ((cpuID.ECX() >> 19) & 0x00000001)
        proc_instr.push_back(std::string("SSE4.1"));

    if ((cpuID.ECX() >> 20) & 0x00000001)
        proc_instr.push_back(std::string("SSE4.2"));

    if ((cpuID.ECX() >> 25) & 0x00000001)
        proc_instr.push_back(std::string("AES"));

    if ((cpuID.ECX() >> 28) & 0x00000001)
        proc_instr.push_back(std::string("AVX"));

    if ((cpuID.ECX() >> 29) & 0x00000001)
        proc_instr.push_back(std::string("F16C"));

    if ((cpuID.ECX() >> 30) & 0x00000001)
        proc_instr.push_back(std::string("RDRAND"));

    return proc_instr;
}


const std::string CPUID_getBrand()
{
    if (!isSuported(0x80000004))
        return "Feature not supported!";

    CPUID cpuID1(0x80000002); // Get CPU brand first 16 Bytes
    CPUID cpuID2(0x80000003); // Get CPU brand second 16 Bytes
    CPUID cpuID3(0x80000004); // Get CPU brand third 16 Bytes

    std::string brand;
    brand += std::string((const char*)&cpuID1.EAX(), 4);
    brand += std::string((const char*)&cpuID1.EBX(), 4);
    brand += std::string((const char*)&cpuID1.ECX(), 4);
    brand += std::string((const char*)&cpuID1.EDX(), 4);
       
    brand += std::string((const char*)&cpuID2.EAX(), 4);
    brand += std::string((const char*)&cpuID2.EBX(), 4);
    brand += std::string((const char*)&cpuID2.ECX(), 4);
    brand += std::string((const char*)&cpuID2.EDX(), 4);

    brand += std::string((const char*)&cpuID3.EAX(), 4);
    brand += std::string((const char*)&cpuID3.EBX(), 4);
    brand += std::string((const char*)&cpuID3.ECX(), 4);
    brand += std::string((const char*)&cpuID3.EDX(), 4);

    return brand;
}

static std::array<int, 3> intel(bool hyperthreadingSupported)
{
    int logicalProc = 1;                         // number of logical processor cores
    int physicalProc = 1;                        // number of physical processor cores
    int procPerCore = 1;                         // logical cores per physical core

    int hyper = 0;                           // hyperthreading status: 0 = unknown, 1 = disabled, 2 = enabled
    if (isSuported(0xB)) {                    // leaf 0xB or 0x1F: Extended Topology Enumeration
        int num = 0xB;

        for (int c = 0; c < 5; c++) {
            CPUID cpuID(num, c);             // enumeration level c
            int type = (cpuID.ECX() >> 8) & 0xFF;// enumeration type at level c
            if (type == 1) {                 // SMT level
                procPerCore = cpuID.EBX() & 0xFFFF;
            }
            else if (type >= 2) {            // core level
                logicalProc = cpuID.EBX() & 0xFFFF;
            }
            else if (type == 0) break;
            // There are more types/levels to consider if we use num = 0x1F. We may need  
            // to fix this in the future if CPUs with more complex configurations appear
        }
        physicalProc = logicalProc / procPerCore;

        // The number of performance monitor registers depends on hyperthreading status
        // on Intel CPUs with performance monitoring version 3 or 4
        CPUID cpuID(0xA);               // performance monitor counters information
        int perfVersion = cpuID.EAX() & 0xFF;    // performance monitoring version
        int perfNum = (cpuID.EAX() >> 8) & 0xFF; // number of performance monitoring registers
        if (perfVersion == 3 || perfVersion == 4) {
            if (perfNum == 4) {
                hyper = 2;                   // 4 performance registers when hyperthreading enabled
            }
            else if (perfNum == 8) {         // 8 performance registers when hyperthreading disabled
                hyper = 1;
                procPerCore = 1;
                logicalProc = physicalProc;  // reduce the number of logical processors when hyperthreading is disabled
            }
            // hyper remains 0 in all other cases, indicating unknown status
        }
    }
    else if (isSuported(4)) {                 // CPUID function 4: cache parameters and cores
        CPUID cpuID(4);
        logicalProc = (cpuID.EAX() >> 26) + 1;
        if (hyperthreadingSupported) {
            // number of logical processors per core is not known. Assume 2 if hyperthreading supported
            procPerCore = 2;
        }
        physicalProc = logicalProc / procPerCore;
    }
    else {
        // no information. Assume 1 processor
    }

    return std::array<int, 3> { physicalProc, logicalProc, procPerCore };
}

static std::array<int, 3> amd(bool hyperthreadingSupported)
{
    int logicalProc = 1;                         // number of logical processor cores
    int physicalProc = 1;                        // number of physical processor cores
    int procPerCore = 1;                         // logical cores per physical core

    if (isSuported(8)) {
        CPUID cpuID(0x80000008);
        logicalProc = (cpuID.ECX() & 0xFF) + 1;

        if (isSuported(0x1E)) {
            CPUID cpuID(0x8000001E);
            procPerCore = ((cpuID.EBX() >> 8) & 0x03) + 1;
        }
        else { // procPerCore = 2 if simultaneous multithreading is enabled, 1 if disabled
            if (hyperthreadingSupported) {
                procPerCore = 2;
            }
            else {
                procPerCore = 1;
            }
        }
        physicalProc = logicalProc / procPerCore;
    }
    else if (hyperthreadingSupported) {
        // number of logical processors per core is not known. Assume 2 if SMT supported
        logicalProc = 2;
        physicalProc = 1;
    }

    return std::array<int, 3> { physicalProc, logicalProc, procPerCore };
}

const std::array<int, 3> CPUID_getTopology(const std::string& vendor)
{
    bool hyperthreadingSupported{ false };

    CPUID cpuID(1);
    if (cpuID.EDX() & (1 << 28))
        hyperthreadingSupported = true;

    if (vendor == "GenuineIntel")
        return intel(hyperthreadingSupported);
    else if (vendor == "AuthenticAMD")
        return amd(hyperthreadingSupported);

    return std::array<int, 3> {-1, -1, -1};
}
