#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <queue>
#include <unordered_map>
#include "Process.hpp"

class Memory {
public:
    Memory(std::ofstream *log);
    virtual ~Memory();

    void new_process(Process proc);
    Process get_next_proc();
    void write_data(const std::string& var, int32_t d);
    int32_t read_data(const std::string& var);
    void print_data();

private:
    std::ofstream *log;

    std::queue<Process> proc_queue;
    std::unordered_map<std::string, int32_t> data;
};

#endif // MEMORY_HPP
