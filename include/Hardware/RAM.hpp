#ifndef MEMORY_HPP_
#define MEMORY_HPP_

#include <fstream>
#include <deque>
#include <queue>
#include <unordered_map>
#include <mutex>
#include "OS/Process.hpp"

namespace Hardware {

class RAM {
public:
    struct Variable {
        std::string name;
        int32_t data;
    };

    void create_process(const std::string& file_name, OS::Process::Time quantum);
    OS::Process* context_restore();
    void context_switch(OS::Process* proc, OS::Process::CPUState cpu_state);

    void write_data(uint32_t pid, const Variable& var);
    int32_t read_data(uint32_t pid, const std::string& var_name);

    bool is_fcfs_empty();

    void print_log(uint32_t pid, std::ofstream& log);

private:
    std::unordered_map<uint32_t, std::unordered_map<std::string, int32_t>> address_space;

    std::deque<OS::Process> pcb; // Process Control Block
    std::queue<OS::Process*> fcfs;

    std::mutex mtx;
};

} // namespace Hardware

#endif // MEMORY_HPP_
