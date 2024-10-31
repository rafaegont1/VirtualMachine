#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <queue>
#include <unordered_map>
#include "Process.hpp"

class Memory {
public:
    Memory();
    virtual ~Memory();

    void new_process(Process proc);
    Process get_next_process();
    void write_data(const std::string& var, uint32_t d);
    uint32_t read_data(const std::string& var);
    void print_data();

private:
    std::queue<Process> proc_queue;
    std::unordered_map<std::string, uint32_t> data;
};

#endif // MEMORY_HPP
