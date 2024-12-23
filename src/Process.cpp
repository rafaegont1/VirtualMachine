#include "Process.hpp"
#include "CPU/Instruction.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std::chrono_literals;

uint32_t Process::count = 0;

Process::Process(const std::string& file_name, const Time quantum) :
    pid{++count},
    state{State::READY},
    quantum{quantum},
    timestamp{0}
{
    read_file(file_name);
}

uint32_t Process::get_pid() const
{
    return this->pid;
}

Process::State Process::get_state() const
{
    return this->state;
}

void Process::set_quantum(const Time quantum)
{
    this->quantum = quantum;
}

Process::Time Process::get_quantum() const
{
    return this->quantum;
}

Process::Time Process::get_timestamp() const
{
    return this->timestamp;
}

const std::vector<std::string> * Process::get_instr(const uint32_t pc) const
{
    return &text[pc];
}

void Process::set_cpu_state(const Process::CPUState& cpu_state)
{
    this->cpu_state = cpu_state;
}

const Process::CPUState& Process::get_cpu_state() const
{
    return this->cpu_state;
}

void Process::start()
{
    state = State::RUNNING;
    begin = std::chrono::high_resolution_clock::now();
}

void Process::stop()
{
    // update timestamp
    TimePoint end = std::chrono::high_resolution_clock::now();
    timestamp += (end - begin);

    // set process as ready to run again
    state = State::READY;
}

void Process::finish()
{
    state = State::TERMINATED;
}

Process::Time Process::get_cpu_time() {
    if (state != State::RUNNING) return 0ms;

    TimePoint end = std::chrono::high_resolution_clock::now();

    return (end - begin);
}

inline void Process::read_file(const std::string& file_name)
{
    std::ifstream file(file_name);
    std::string line;

    if (not file.is_open()) {
        throw std::runtime_error("Error: failed to open file " + file_name);
    }

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<std::string> instr;
        std::string word;

        while (iss >> word) {
            instr.push_back(std::move(word));
        }

        text.push_back(std::move(instr));
    }

    file.close();
}
