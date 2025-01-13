#ifndef MINIX_HPP
#define MINIX_HPP

#include <array>
#include <thread>
#include "HW/CPU/Core.hpp"
#include "OS/Scheduler/FIFOScheduler.hpp"
#include "OS/Scheduler/PriorityQueue.hpp"

namespace OS {

class Minix {
public:
    static const uint8_t NUM_CORES = 2;

    Minix(int argc, char** argv);

    void bootloader(int argc, char** argv);
    void run();

private:
    uint32_t random_number(uint32_t min, uint32_t max);
    void schedule(const uint8_t core_id);
    std::shared_ptr<OS::PCB> context_restore();
    void context_switch(std::shared_ptr<OS::PCB> proc);
    OS::PCB::Time timestamp();

    PCB::TimePoint timestamp_begin_;
    uint32_t procs_executed_ = 0;

    std::array<HW::CPU::Core, NUM_CORES> cpu_;
    std::array<std::thread, NUM_CORES> threads_;
#if defined(FIFO)
    OS::FIFOScheduler scheduler_;
#elif defined(PRIORITY_QUEUE)
    OS::PrioPriorityQueue scheduler_;
#endif
};

} // namespace OS

#endif // MINIX_HPP
