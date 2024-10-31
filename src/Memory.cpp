#include "Memory.hpp"

#include <iostream>

Memory::Memory() {}

Memory::~Memory() {}

void Memory::new_process(Process proc)
{
    proc_queue.push(proc);
}

Process Memory::get_next_process()
{
    Process next_process = this->proc_queue.front();

    proc_queue.pop();

    return next_process;
}

void Memory::write_data(const std::string& var, uint32_t d)
{
    data[var] = d;
}

uint32_t Memory::read_data(const std::string& var)
{
    if (data.find(var) == data.end()) {
        throw std::runtime_error("Error: Variable '" + var + "' not found.");
    }

    return data[var];
}

void Memory::print_data()
{
    std::cout << "\nMEMORY DATA\n";

    if (data.empty()) {
        std::cout << "empty\n";
        return;
    }

    for (const auto& d : data) {
        std::cout << d.first << ": " << d.second << '\n';
    }
}
