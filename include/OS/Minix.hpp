#ifndef MINIX_HPP
#define MINIX_HPP

#include <array>
#include <thread>
#include "HW/CPU/Core.hpp"
#include "OS/Scheduler.hpp"

namespace OS {

class Minix {
public:
    static const uint8_t NUM_CORES = 1;

    Minix(int argc, char** argv);

    void bootloader(int argc, char** argv);
    void run();

private:
    PCB::Time generate_random_quantum(uint8_t min, uint8_t max);
    void schedule(const uint8_t core_id);

    std::array<HW::CPU::Core, NUM_CORES> cpu_;
    std::array<std::thread, NUM_CORES> threads_;
    OS::Scheduler scheduler_;
};

} // namespace OS

#endif // MINIX_HPP
