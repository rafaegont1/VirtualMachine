#include "OS/Minix.hpp"

#include <iostream>
#include <memory>
#include <random>
#include "HW/ISA/Encoding.hpp"
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

        scheduler_.push(proc);
    }
}

void Minix::run()
{
    for (int i = 0; i < NUM_CORES; i++) {
        threads_[i] = std::thread(&Minix::schedule, this, i);
        threads_[i].join();
    }

    // for (auto& core_thread : cpu) {
    //     core_thread.th = std::thread(
    //         &HW::CPU::Core::run, &core_thread.core, scheduler
    //     );
    // }

    // for (auto& core_thread : cpu) {
    //     core_thread.th.join();
    // }
}

void Minix::schedule(const uint8_t core_id)
{
    while (!scheduler_.empty()) {
        std::shared_ptr<OS::PCB> proc = scheduler_.pop();
        OS::PCB::Time cpu_time = std::chrono::milliseconds(0);

        do {
            auto begin = std::chrono::high_resolution_clock::now();

            cpu_[core_id].run_cycle(proc); // TODO

            auto end = std::chrono::high_resolution_clock::now();
            cpu_time += (end - begin);
        } while ((proc->get_state() == OS::PCB::State::RUNNING)
            && (cpu_time < proc->get_quantum()));

        // update process timestamp
        OS::PCB::Time new_timestamp = proc->get_timestamp() + cpu_time;
        proc->set_timestamp(new_timestamp);

        // std::unique_lock<std::mutex> lock(mtx_);
        // cv_.wait(lock, [] { return !scheduler_.empty(); });
    }
}

PCB::Time Minix::generate_random_quantum(uint8_t min, uint8_t max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);

    return PCB::Time(distrib(gen));
}

bool Minix::are_cores_available() const
{
    for (const auto& core : cpu_) {
        if (core.is_available()) {
            return true;
        }
    }
    return false;
}

} // namespace OS
