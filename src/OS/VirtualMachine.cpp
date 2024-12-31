#include "OS/VirtualMachine.hpp"

#include <iostream>
#include <random>
#include "Hardware/CPU/Instruction.hpp"

namespace OS {

void VirtualMachine::bootloader(int argc, char** argv)
{
    for (int i = 1; i < argc; i++) {
        const std::string file_name = argv[i];
        PCB::Time quantum = generate_random_quantum(10, 30);

        mem.create_process(file_name, quantum);
    }
}

void VirtualMachine::run()
{
    for (auto& core_thread : cpu) {
        core_thread.th = std::thread(
            &Hardware::CPU::Core::run, &core_thread.core, std::ref(mem)
        );
    }

    for (auto& core_thread : cpu) {
        core_thread.th.join();
    }
}

void VirtualMachine::create_process(const std::string& file_name)
{
    PCB::Time quantum = generate_random_quantum(10, 30);

    mem.create_process(file_name, quantum);
}

PCB::Time VirtualMachine::generate_random_quantum(uint8_t min, uint8_t max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);

    return PCB::Time(distrib(gen));
}

} // namespace OS
