#include "Hardware/CPU/Core.hpp"

#include <iostream>
#include "OS/PCB.hpp"
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
    OS::PCB* proc;

    while (!mem.is_fcfs_empty()) {
        proc = context_restore(mem);
        // context_log("New process", *proc);

        // run pipeline cycles
        do {
            log_ << "CLOCK: " << ++clk       << '\n';
                 // << "PID: "   << proc->pid() << '\n'
                 // << "Process CPU time: " << proc->cpu_time().count() << '\n';

            pipeline_.instr_fetch(pc_, std::ref(*proc));
            pipeline_.instr_decode(std::ref(*proc));
            pipeline_.execute(std::ref(rf_), std::ref(pc_));
            pipeline_.mem_access(proc->pid(), std::ref(rf_), std::ref(mem));
            pipeline_.write_back();

            rf_.print_log(std::ref(log_));
            pipeline_.print_log(std::ref(log_));
            mem.print_log(proc->pid(), std::ref(log_));
            log_ << "==============================================================\n";

            pipeline_.update();
        } while ((proc->state() != OS::PCB::State::TERMINATED)
            && (proc->cpu_time() < proc->quantum()));

        if (proc->state() != OS::PCB::State::TERMINATED) {
            // auto proc_log = [&log_](const std::string& label, const OS::PCB& proc) {
            //     log_ << "<- " << label <<  " ->"
            //          << "PID: "       << proc.pid()               << '\n'
            //          << "Quantum: "   << proc.quantum().count()   << '\n'
            //          << "CPU time: "  << proc.cpu_time().count()  << '\n'
            //          << "Timestamp: " << proc.timestamp().count() << '\n';
            // };

            log_ << "--------------------------------------------------------------\n"
                 << "Context switch!\n"
                 << "<- Old process ->\n"
                 << proc->info()
                 << "--------------------------------------------------------------\n";
            // context_log("Old process", *proc);
            context_switch(proc, mem);
        }
    }
}

OS::PCB* Core::context_restore(RAM& mem)
{
    OS::PCB* proc = mem.context_restore();
    OS::PCB::CPUState cpu_state;

    // fetch cpu state from process
    cpu_state = proc->cpu_state();
    rf_.gpr(cpu_state.gpr);
    pc_ = cpu_state.pc;

    return proc;
}

void Core::context_switch(OS::PCB* proc, RAM& mem)
{
    OS::PCB::CPUState cpu_state;

    // store current cpu state in process
    proc->stop();
    cpu_state.gpr = rf_.gpr();
    cpu_state.pc = pc_;
    proc->cpu_state(cpu_state);

    mem.context_switch(proc, cpu_state);
}

// void Core::context_log(const std::string& label, const OS::PCB& proc)
// {
//     log_ << "<- " << label <<  " ->\n"
//          << "PID: "       << proc.pid()               << '\n'
//          << "Quantum: "   << proc.quantum().count()   << '\n'
//          << "CPU time: "  << proc.cpu_time().count()  << '\n'
//          << "Timestamp: " << proc.timestamp().count() << '\n';
// }

} // namespace Hardware::CPU
