#include "Hardware/CPU/Core.hpp"

#include <iostream>
#include "OS/Process.hpp"
#include "Hardware/CPU/Instruction.hpp"
#include "Hardware/CPU/RegFile.hpp"

namespace Hardware::CPU {

uint8_t Core::count_ = 0;

Core::Core()
{
    const std::string log_name = "cpu" + std::to_string(count_++) + "_log.txt";

    log_.open(log_name);

    if (!log_.is_open()) {
        throw std::runtime_error("Error: failed to open log file " + log_name);
    }
}

Core::~Core()
{
    log_.close();
}

void Core::run(RAM& mem)
{
    OS::Process* proc;
    bool quantum_expired;

    while (!mem.is_fcfs_empty()) {
        proc = context_restore(mem);

        // set quantum expired to false before pipeline starts
        quantum_expired = false;

        // run pipeline cycles
        do {
            log_ << "CLOCK: " << ++clk << '\n';

            pipeline_.instr_fetch(pc_, *proc);
            pipeline_.instr_decode(*proc);
            pipeline_.execute(rf_, pc_);
            pipeline_.mem_access(proc->pid(), rf_, mem);
            pipeline_.write_back();

            rf_.print_log(log_);
            pipeline_.print_log(log_);
            mem.print_log(proc->pid(), log_);
            log_ << "==============================================================\n";

            pipeline_.update();

            quantum_expired = (proc->cpu_time() > proc->quantum());
        } while (proc->state() != OS::Process::State::TERMINATED
            && !quantum_expired);

        if (proc->state() != OS::Process::State::TERMINATED) {
            context_switch(proc, mem);
        }
    }
}

OS::Process* Core::context_restore(RAM& mem)
{
    OS::Process* proc = mem.context_restore();
    OS::Process::CPUState cpu_state;

    // fetch cpu state from process
    cpu_state = proc->cpu_state();
    rf_.reg(cpu_state.reg);
    pc_ = cpu_state.pc;

    return proc;
}

void Core::context_switch(OS::Process* proc, RAM& mem)
{
    OS::Process::CPUState cpu_state;

    // store current cpu state in process
    proc->stop();
    cpu_state.reg = rf_.reg();
    cpu_state.pc = pc_;
    proc->cpu_state(cpu_state);

    mem.context_switch(proc, cpu_state);
}

} // namespace Hardware::CPU
