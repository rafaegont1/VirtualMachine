#ifndef VIRTUAL_MACHINE
#define VIRTUAL_MACHINE

#include <vector>
#include <fstream>
#include "CPU.hpp"

class VirtualMachine
{
public:
    VirtualMachine(const std::string& log_file);
    virtual ~VirtualMachine();

    void create_proc(const std::string& code_file);
    void run();

private:
    std::ofstream log;

    CPU cpu;
    Memory mem;
    std::vector<Process> proc; // running processes
};

#endif // VIRTUAL_MACHINE
