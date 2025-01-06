#include "OS/Minix.hpp"

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
        PCB::Time quantum = generate_random_quantum(1, 5);
        std::shared_ptr<OS::PCB> proc =
            HW::RAM::Allocator::create_process(file_name, quantum);

        proc->set_state(OS::PCB::State::READY);
        fcfs_.push(proc);
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
}

void Minix::schedule(const uint8_t core_id)
{
    std::shared_ptr<OS::PCB> proc;
    OS::PCB::Time cpu_time;
    OS::PCB::TimePoint begin, end;

    while (!fcfs_.empty()) {
        // Context restore (restore state of process and cpu)
        // OPTIMIZE: Looks like even checking is fcfs queue isn't empty, the
        // fcfs queue can pop a nullptr (like a process was popped from the fcfs
        // queue between those two lines). Check if there is a better way to
        // handle it than using this if-continue
        proc = fcfs_.pop();
        if (proc == nullptr) continue;
        proc->set_state(OS::PCB::State::RUNNING);

        // Run process in CPU
        cpu_time = std::chrono::milliseconds(0);
        do {
            begin = std::chrono::high_resolution_clock::now();
            cpu_[core_id].run_cycle(proc, timestamp_begin_);
            end = std::chrono::high_resolution_clock::now();
            cpu_time += (end - begin);
        } while (proc->get_state() == OS::PCB::State::RUNNING
            && (cpu_time < proc->get_quantum() || fcfs_.empty()));

        // Context switch (save state of process and cpu)
        if (proc->get_state() != OS::PCB::State::TERMINATED) {
            proc->log << "Quantum expired!\n"
                      << "Time spent in CPU: " << cpu_time.count() << " ms\n"
                      << "Context switching...\n"
                      << "==============================================================\n";
            proc->set_state(OS::PCB::State::READY);
            fcfs_.push(proc);
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
