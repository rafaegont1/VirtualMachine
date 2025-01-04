#include "OS/Minix.hpp"

#include <memory>
#include <random>
#include "HW/RAM/Allocator.hpp"

namespace OS {

Minix::Minix(int argc, char** argv)
{
    bootloader(argc, argv);
}

void Minix::bootloader(int argc, char** argv)
{
    for (int i = 1; i < argc; i++) {
        const std::string file_name = argv[i];
        PCB::Time quantum = generate_random_quantum(10, 30);
        std::shared_ptr<OS::PCB> proc =
            HW::RAM::Allocator::create_process(file_name, quantum);

        proc->set_state(OS::PCB::State::READY);
        scheduler_.push(proc);
    }
}

void Minix::run()
{
    for (int i = 0; i < NUM_CORES; i++) {
        threads_[i] = std::thread(&Minix::schedule, this, i);
        threads_[i].join();
    }
}

void Minix::schedule(const uint8_t core_id)
{
    while (!scheduler_.empty()) {
        // context restore (restore state of process and cpu)
        std::shared_ptr<OS::PCB> proc = scheduler_.pop();
        OS::PCB::Time cpu_time = std::chrono::milliseconds(0);

        proc->set_state(OS::PCB::State::RUNNING);

        do {
            auto begin = std::chrono::high_resolution_clock::now();

            cpu_[core_id].run_cycle(proc);

            auto end = std::chrono::high_resolution_clock::now();
            cpu_time += (end - begin);
        } while (proc->get_state() == OS::PCB::State::RUNNING
            && (cpu_time < proc->get_quantum() || scheduler_.empty()));

        // update process timestamp
        OS::PCB::Time new_timestamp = proc->get_timestamp() + cpu_time;
        proc->set_timestamp(new_timestamp);

        // context switch (save state of process and cpu)
        if (proc->get_state() != OS::PCB::State::TERMINATED) {
            proc->set_state(OS::PCB::State::READY);
            scheduler_.push(proc);
        }
    }
}

PCB::Time Minix::generate_random_quantum(uint8_t min, uint8_t max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);

    return PCB::Time(distrib(gen));
}

} // namespace OS
