#include "Memory.hpp"

#include <iostream>

Memory::Memory(std::ofstream *log) : log(log) {}

Memory::~Memory() {}

void Memory::new_process(Process proc)
{
    proc_queue.push(proc);
}

Process Memory::get_next_proc()
{
    Process next_process = this->proc_queue.front();

    proc_queue.pop();

    return next_process;
}

void Memory::write_data(const std::string& var, int32_t d)
{
    data[var] = d;
}

int32_t Memory::read_data(const std::string& var)
{
    if (data.find(var) == data.end()) {
        throw std::runtime_error("Error: Variable '" + var + "' not found.");
    }

    return data[var];
}

void Memory::print_data()
{
    *log << "\nMEMORY DATA\n";

    if (data.empty()) {
        *log << "empty\n";
        return;
    }

    for (const auto& d : data) {
        *log << d.first << ": " << d.second << '\n';
    }
}
