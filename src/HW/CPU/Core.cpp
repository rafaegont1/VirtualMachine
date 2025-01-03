#include "HW/CPU/Core.hpp"

#include <iostream>
#include "OS/PCB.hpp"
#include "HW/CPU/RegFile.hpp"

namespace HW::CPU {

uint8_t Core::count = 0;

Core::Core() : id_{count++}
{
    // const std::string log_name = "cpu" + std::to_string(id_) + "_log.txt";

    // log_.open(log_name);

    // if (!log_.is_open()) {
    //     throw std::runtime_error("Error: failed to open log file " + log_name);
    // }
}

Core::~Core()
{
    // log_.close();
}

void Core::run_cycle(std::shared_ptr<OS::PCB> proc)
{
    // while (!mem.is_fcfs_empty()) {
    //     proc = context_restore(mem);
    //     // context_log("New process", *proc);

    //     // run pipeline cycles
        // do {
            // log_ << "CLOCK: " << ++clk_       << '\n';
            //      << "PID: "   << proc->pid() << '\n'
            //      << "Process CPU time: " << proc->cpu_time().count() << '\n';

        pipeline_.set_proc(proc);

        pipeline_.instr_fetch();
        pipeline_.instr_decode();
        pipeline_.execute();
        pipeline_.mem_access();
        pipeline_.write_back();

        rf_.print_log(log_);
        pipeline_.print_log(log_);
        mem.print_log(proc->pid(), log_);
        log_ << "==============================================================\n";

        pipeline_.update_stages();
        // } while ((proc->get_state() != OS::PCB::State::TERMINATED)
        //     && (proc->cpu_time() < proc->quantum()));

        // if (proc->state() != OS::PCB::State::TERMINATED) {
        //     // auto proc_log = [&log_](const std::string& label, const OS::PCB& proc) {
        //     //     log_ << "<- " << label <<  " ->"
        //     //          << "PID: "       << proc.pid()               << '\n'
        //     //          << "Quantum: "   << proc.quantum().count()   << '\n'
        //     //          << "CPU time: "  << proc.cpu_time().count()  << '\n'
        //     //          << "Timestamp: " << proc.timestamp().count() << '\n';
        //     // };

        //     log_ << "--------------------------------------------------------------\n"
        //          << "Context switch!\n"
        //          << "<- Old process ->\n"
        //          << proc->info()
        //          << "--------------------------------------------------------------\n";
        //     // context_log("Old process", *proc);
        //     context_switch(proc, mem);
        // }
    // }
}

bool Core::is_available() const
{
    return available_;
}

// OS::PCB* Core::context_restore(Mem& mem)
// {
//     OS::PCB* proc = mem.context_restore();
//     OS::PCB::CPUState cpu_state;

//     // fetch cpu state from process
//     cpu_state = proc->cpu_state();
//     rf_.gpr(cpu_state.gpr);
//     pc_ = cpu_state.pc;

//     return proc;
// }

// void Core::context_switch(OS::PCB* proc, Mem& mem)
// {
//     OS::PCB::CPUState cpu_state;

//     // store current cpu state in process
//     proc->stop();
//     cpu_state.gpr = rf_.gpr();
//     cpu_state.pc = pc_;
//     proc->cpu_state(cpu_state);

//     mem.context_switch(proc, cpu_state);
// }

// void Core::context_log(const std::string& label, const OS::PCB& proc)
// {
//     log_ << "<- " << label <<  " ->\n"
//          << "PID: "       << proc.pid()               << '\n'
//          << "Quantum: "   << proc.quantum().count()   << '\n'
//          << "CPU time: "  << proc.cpu_time().count()  << '\n'
//          << "Timestamp: " << proc.timestamp().count() << '\n';
// }

} // namespace HW::CPU
