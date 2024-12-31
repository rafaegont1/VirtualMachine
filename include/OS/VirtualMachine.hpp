#ifndef VIRTUAL_MACHINE_HPP_
#define VIRTUAL_MACHINE_HPP_

#include <array>
#include <vector>
#include <fstream>
#include <thread>
#include "Hardware/CPU/Core.hpp"

namespace OS {

class VirtualMachine {
public:
    static const uint8_t NUM_CORES = 1;

    void bootloader(int argc, char** argv);
    void run();

private:
    struct CoreThreadPool {
        Hardware::CPU::Core core;
        std::thread th;
    };

    void create_process(const std::string& file_name);
    PCB::Time generate_random_quantum(uint8_t min, uint8_t max);

    Hardware::RAM mem;
    std::array<CoreThreadPool, NUM_CORES> cpu;
};

} // namespace OS

#endif // VIRTUAL_MACHINE_HPP_
