#include "HW/CPU/Core.hpp"

#include <iostream>
#include "HW/RAM/DataSpace.hpp"
#include "OS/PCB.hpp"

namespace HW::CPU {

uint32_t Core::count = 0;

Core::Core() : id_{count++}
{
}

void Core::run_cycle(std::shared_ptr<OS::PCB> proc, Cache& cache, OS::PCB::TimePoint timestamp_begin)
{
    pipeline_.set_proc(proc);

    OS::PCB::TimePoint timestamp_end = std::chrono::high_resolution_clock::now();
    OS::PCB::Time timestamp = timestamp_end - timestamp_begin;
    proc->log << "CPU ID: "    << id_    << '\n'
              << "CLOCK: "     << ++clk_ << '\n'
              << "Timestamp: " << timestamp.count() << " ms\n";

    // run pipeline stages
    pipeline_.instr_fetch(cache);
    pipeline_.instr_decode();
    pipeline_.execute(cache);
    pipeline_.mem_access(cache);
    pipeline_.write_back();

    // print cycle on log
    const HW::CPU::CPUState& cpu = proc->cpu_state;
    const HW::DataSpace& mem = proc->mem;

    cpu.rf.print_log(proc->log);
    pipeline_.print_log(proc->log);
    mem.print_log(proc->log);
    proc->log << "==============================================================\n";

    pipeline_.update_stages();
}

} // namespace HW::CPU
