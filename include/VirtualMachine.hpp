#ifndef VIRTUAL_MACHINE
#define VIRTUAL_MACHINE

#include <vector>
#include "CPU.hpp"

class VirtualMachine
{
public:
    VirtualMachine();
    virtual ~VirtualMachine() = default;

    void create_proc(const std::string& code_file);
    void run();

private:
    CPU cpu;
    Memory mem;
    std::vector<Process> proc; // running processes
};

#endif // VIRTUAL_MACHINE
