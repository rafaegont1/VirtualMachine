#include "VirtualMachine.hpp"

#include <iostream>
#include <thread>
#include <random>
#include "CPU/Instruction.hpp"

VirtualMachine::VirtualMachine() :
    cpu {
        CPU::Core(mem, "cpu1_log.txt"),
        CPU::Core(mem, "cpu2_log.txt"),
        CPU::Core(mem, "cpu2_log.txt"),
        CPU::Core(mem, "cpu2_log.txt")
    }
{
}

VirtualMachine::~VirtualMachine()
{
    log.close();
}

void VirtualMachine::create_process(const std::string & file_name)
{
    Process::Time quantum = generate_random_quantum(10, 30);

    mem.create_process(file_name, quantum);
}

void VirtualMachine::run()
{
    for (auto & core : cpu) {
        core.run();
    }
}

inline Process::Time VirtualMachine::generate_random_quantum(uint8_t min, uint8_t max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);

    return Process::Time(distrib(gen));
}
