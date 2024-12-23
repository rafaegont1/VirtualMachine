#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <mutex>
#include "Process.hpp"

class RAM {
public:
    struct Variable {
        std::string name;
        int32_t data;
    };

    RAM();
    virtual ~RAM() = default;

    void create_process(const std::string & file_name, const Process::Time quantum);
    // void kill_process(uint32_t pid);
    Process * start_process();
    void stop_process(Process * proc, Process::CPUState cpu_state);

    void write_data(uint32_t pid, const Variable & var);
    int32_t read_data(uint32_t pid, const std::string & var_name);

    bool is_fcfs_empty();

    void print_log(uint32_t pid, std::ofstream & log);

private:
    std::unordered_map<uint32_t, std::unordered_map<std::string, int32_t>> address_space;

    std::vector<Process> pcb; // Process Block Control
    std::queue<Process *> fcfs;

    std::mutex mtx;
};

#endif // MEMORY_HPP
