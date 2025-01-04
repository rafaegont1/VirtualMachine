#include "HW/CPU/Core.hpp"

#include <iostream>
#include "HW/RAM/DataSpace.hpp"
#include "OS/PCB.hpp"
#include "HW/CPU/RegFile.hpp"

namespace HW::CPU {

uint32_t Core::count = 0;

Core::Core() : id_{count++}
{
}

void Core::run_cycle(std::shared_ptr<OS::PCB> proc)
{
    pipeline_.set_proc(proc);

    // run pipeline stages
    pipeline_.instr_fetch();
    pipeline_.instr_decode();
    pipeline_.execute();
    pipeline_.mem_access();
    pipeline_.write_back();

    // print cycle on log
    const OS::PCB::CPUState& cpu = proc->cpu_state;
    const HW::RAM::DataSpace& mem = proc->mem;

    proc->log << "CPU ID: " << id_    << '\n'
              << "CLOCK: "  << ++clk_ << '\n';
    cpu.rf.print_log(proc->log);
    pipeline_.print_log(proc->log);
    mem.print_log(proc->log);
    proc->log << "==============================================================\n";

    pipeline_.update_stages();
}

} // namespace HW::CPU
