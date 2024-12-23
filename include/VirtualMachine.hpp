#ifndef VIRTUAL_MACHINE_HPP
#define VIRTUAL_MACHINE_HPP

#include <array>
#include <vector>
#include <fstream>
#include "CPU/Core.hpp"

class VirtualMachine {
public:
    static const uint8_t NUM_CORES = 1;

    VirtualMachine();
    virtual ~VirtualMachine();

    void create_process(const std::string & file_name);
    void run();

private:
    inline Process::Time generate_random_quantum(uint8_t min, uint8_t max);

    RAM mem;
    std::array<CPU::Core, NUM_CORES> cpu;

    std::ofstream log;
};

#endif // VIRTUAL_MACHINE_HPP
