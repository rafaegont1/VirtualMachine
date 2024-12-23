#include "RAM.hpp"
#include "Process.hpp"

#include <iostream>
#include <fstream>
// #include <algorithm> // for std::remove_if and std::erase

RAM::RAM()
{
}

void RAM::create_process(const std::string & file_name, const Process::Time quantum)
{
    std::lock_guard<std::mutex> lock(mtx);

    pcb.emplace_back(file_name, quantum);
    fcfs.push(&pcb.back());
}

// void RAM::kill_process(uint32_t pid)
// {
//     std::lock_guard<std::mutex> lock(mtx);

//     auto it_pcb = std::remove_if(pcb.begin(), pcb.end(), 
//         [pid](const Process & proc) {
//             return proc.get_pid() == pid;
//         });
//     if (it_pcb != pcb.end()) {
//         pcb.erase(it_pcb, pcb.end());
//         std::cout << "Process with pid " << pid << " killed.\n";
//     } else {
//         std::cout << "Process with pid " << pid << " not found in PCB.\n";
//     }

//     auto it_address_space = address_space.find(pid);
//     if (it_address_space != address_space.end()) {
//         address_space.erase(it_address_space);
//         std::cout << "Process with pid " << pid << " removed from address space.\n";
//     } else {
//         std::cout << "Process with pid " << pid << " not found in address space.\n";
//     }
// }

Process * RAM::start_process()
{
    std::lock_guard<std::mutex> lock(mtx);

    if (fcfs.empty()) {
        throw std::runtime_error("Can't get next process; FCFS queue is empty");
    }

    Process * proc;

    proc = fcfs.front();
    fcfs.pop();
    proc->start();

    return proc;
}

void RAM::stop_process(Process * proc, Process::CPUState cpu_state)
{
    std::lock_guard<std::mutex> lock(mtx);

    proc->stop();
    proc->set_cpu_state(cpu_state);
    if (proc->get_state() != Process::State::TERMINATED) {
        fcfs.push(proc);
    }
}

void RAM::write_data(uint32_t pid, const RAM::Variable & var)
{
    std::lock_guard<std::mutex> lock(mtx);

    address_space[pid][var.name] = var.data;
}

int32_t RAM::read_data(uint32_t pid, const std::string & var_name)
{
    std::lock_guard<std::mutex> lock(mtx);

    try {
        return address_space[pid].at(var_name);
    } catch (const std::out_of_range & e) {
        throw std::runtime_error("Tried to access undeclared variable: " + var_name);
    }
}

bool RAM::is_fcfs_empty()
{
    std::lock_guard<std::mutex> lock(mtx);

    return this->fcfs.empty();
}

void RAM::print_log(uint32_t pid, std::ofstream & log)
{
    std::lock_guard<std::mutex> lock(mtx);

    log << "\nMEMORY DATA\n";

    if (address_space[pid].empty()) {
        log << "empty\n";
        return;
    }

    for (const auto & d : address_space[pid]) {
        log << d.first << ": " << d.second << '\n';
    }
}
