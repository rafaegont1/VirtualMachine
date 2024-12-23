#include "CPU/Core.hpp"

#include <iostream>
#include <thread>
#include "Process.hpp"
#include "CPU/Instruction.hpp"
#include "CPU/RegFile.hpp"

namespace CPU {

Core::Core(RAM & mem, const std::string & log_file) :
    mem(mem),
    pipeline(clk, pc, rf, mem, alu)
{
    log.open(log_file);

    if (not log.is_open()) {
        throw std::runtime_error("Error: failed to open log file " + log_file);
    }
}

Core::~Core()
{
    log.close();
}

void Core::run()
{
    std::thread t(&Core::perform_in_thread, this);

    t.detach();
}

void Core::perform_in_thread()
{
    Process * proc;
    Process::CPUState cpu_state;

    while (not mem.is_fcfs_empty()) {
        proc = mem.start_process();

        // fetch cpu state from process
        cpu_state = proc->get_cpu_state();
        rf.set_reg(cpu_state.reg);
        pc = cpu_state.pc;

        // run pipeline cycle
        pipeline.run(proc, log);

        // store current cpu state in process
        proc->stop();
        cpu_state.reg = rf.get_reg();
        cpu_state.pc = pc;
        proc->set_cpu_state(cpu_state);
    }
}

} // namespace CPU
