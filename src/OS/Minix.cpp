#include "OS/Minix.hpp"

#include <iostream>
#include <memory>
#include <random>
#include "HW/RAM/Allocator.hpp"

namespace OS {

Minix::Minix(int argc, char** argv) :
    timestamp_begin_(std::chrono::high_resolution_clock::now())
{
    bootloader(argc, argv);
}

void Minix::bootloader(int argc, char** argv)
{
    for (int i = 1; i < argc; i++) {
        const std::string file_name = argv[i];
        PCB::Time quantum = OS::PCB::Time(random_number(1, 5));
        uint16_t priority = random_number(0, 2);
        std::shared_ptr<OS::PCB> proc =
            HW::RAM::Allocator::create_process(file_name, timestamp(), quantum, priority);
        scheduler_.push(proc);
    }
}

void Minix::run()
{
    for (int i = 0; i < NUM_CORES; i++) {
        threads_[i] = std::thread(&Minix::schedule, this, i);
    }

    for (int i = 0; i < NUM_CORES; i++) {
        threads_[i].join();
    }

    double throughput = procs_executed_ / timestamp().count();
    std::cout << "Throughput: " << throughput << " proc/ms" << std::endl;
}

void Minix::schedule(const uint8_t core_id)
{
    std::shared_ptr<OS::PCB> proc;
    OS::PCB::Time cpu_time;
    OS::PCB::TimePoint begin, end;

    while (!scheduler_.empty()) {
        // Context restore (restore state of process and cpu)
        proc = context_restore();

        if (proc == nullptr) break;

        // Run process in CPU
        cpu_time = std::chrono::milliseconds(0);
        do {
            begin = std::chrono::high_resolution_clock::now();
            cpu_[core_id].run_cycle(proc, timestamp_begin_);
            end = std::chrono::high_resolution_clock::now();
            cpu_time += (end - begin);

#ifdef PREEMPTIVE
            if ((cpu_time > proc->get_quantum()) && !scheduler_.empty()) break;
#endif // PREEMPTIVE
        } while (proc->get_state() == OS::PCB::State::RUNNING);

        proc->update_burst_time(cpu_time);

        // Context switch (save state of process and cpu)
        context_switch(proc);
    }
}

std::shared_ptr<OS::PCB> Minix::context_restore()
{
    std::shared_ptr<OS::PCB> proc = scheduler_.pop();

    if (proc == nullptr) return nullptr;

    if (proc->get_state() == OS::PCB::State::NEW) {
        proc->set_response_time(timestamp());
    }
    proc->set_state(OS::PCB::State::RUNNING);

    return proc;
}

void Minix::context_switch(std::shared_ptr<OS::PCB> proc)
{
    if (proc->get_state() == OS::PCB::State::TERMINATED) {
        proc->update_waiting_time(timestamp());

        auto response_time = proc->get_response_time();
        auto burst_time = proc->get_burst_time();
        auto waiting_time = proc->get_waiting_time();

        proc->log << "Process finished!\n"
                  << "Timestamp: " << timestamp().count() << " ms\n"
                  << "Response time: " << response_time.count() << " ms\n"
                  << "Burst time: " << burst_time.count() << " ms\n"
                  << "Waiting time: " << waiting_time.count() << " ms\n";

        procs_executed_++;
    } else {
#ifdef PREEMPTIVE
        proc->log << "Quantum expired!\n"
                  << "Timestamp: " << timestamp().count() << " ms\n"
                  << "Context switching...\n"
                  << "==============================================================\n";

        proc->set_state(OS::PCB::State::READY);
        scheduler_.push(proc);
#endif // PREEMPTIVE
    }
}

OS::PCB::Time Minix::timestamp()
{
    return std::chrono::high_resolution_clock::now() - timestamp_begin_;
}

uint32_t Minix::random_number(uint32_t min, uint32_t max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);

    return distrib(gen);
}

} // namespace OS
