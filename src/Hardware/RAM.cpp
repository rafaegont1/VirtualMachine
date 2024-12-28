#include "Hardware/RAM.hpp"
#include "OS/Process.hpp"

#include <iostream>
#include <fstream>

namespace Hardware {

void RAM::create_process(const std::string& file_name, OS::Process::Time quantum)
{
    std::lock_guard<std::mutex> lock(mtx);

    pcb.emplace_back(file_name, quantum);
    fcfs.push(&pcb.back());
}

OS::Process* RAM::context_restore()
{
    std::lock_guard<std::mutex> lock(mtx);

    if (fcfs.empty()) {
        throw std::runtime_error("Can't get next process; FCFS queue is empty");
    }

    OS::Process* proc;

    proc = fcfs.front();
    fcfs.pop();
    proc->start();

    return proc;
}

void RAM::context_switch(OS::Process* proc, OS::Process::CPUState cpu_state)
{
    std::lock_guard<std::mutex> lock(mtx);

    proc->stop();
    proc->cpu_state(cpu_state);

    if (proc->state() != OS::Process::State::TERMINATED) {
        fcfs.push(proc);
    }
}

void RAM::write_data(uint32_t pid, const RAM::Variable& var)
{
    std::lock_guard<std::mutex> lock(mtx);

    address_space[pid][var.name] = var.data;
}

int32_t RAM::read_data(uint32_t pid, const std::string& var_name)
{
    std::lock_guard<std::mutex> lock(mtx);

    try {
        return address_space[pid].at(var_name);
    } catch (const std::out_of_range& e) {
        throw std::runtime_error("Tried to access undeclared variable: " + var_name);
    }
}

bool RAM::is_fcfs_empty()
{
    std::lock_guard<std::mutex> lock(mtx);

    return this->fcfs.empty();
}

void RAM::print_log(uint32_t pid, std::ofstream& log)
{
    std::lock_guard<std::mutex> lock(mtx);

    log << "\nMEMORY DATA\n";

    if (address_space[pid].empty()) {
        log << "empty\n";
        return;
    }

    for (const auto& d : address_space[pid]) {
        log << d.first << ": " << d.second << '\n';
    }
}

} // namespace Hardware
