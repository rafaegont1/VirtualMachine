#include "OS/Process.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include "Hardware/CPU/Instruction.hpp"

namespace OS {

using namespace std::chrono_literals;

uint32_t Process::count_ = 0;

Process::Process(const std::string& file_name, const Time quantum) :
    pid_{++count_},
    state_{State::READY},
    quantum_{quantum},
    timestamp_{0ms}
{
    read_file(file_name);
}

uint32_t Process::pid() const { return pid_; }

Process::State Process::state() const { return state_; }

void Process::quantum(const Time new_value) { quantum_ = new_value; }

Process::Time Process::quantum() const { return quantum_; }

Process::Time Process::timestamp() const { return timestamp_; }

const std::vector<std::string>* Process::get_instr(const uint32_t pc) const
{
    return (pc < text_.size())
        ? &text_[pc]
        : &Hardware::CPU::Instruction::NOP_INSTR;
}

void Process::cpu_state(const Process::CPUState& cpu_state)
{
    cpu_state_ = cpu_state;
}

const Process::CPUState& Process::cpu_state() const { return cpu_state_; }

void Process::start()
{
    state_ = State::RUNNING;
    begin_ = std::chrono::high_resolution_clock::now();
}

void Process::stop()
{
    // update timestamp
    TimePoint end = std::chrono::high_resolution_clock::now();
    timestamp_ += (end - begin_);

    // set process as ready to run again
    state_ = State::READY;
}

void Process::terminate()
{
    state_ = State::TERMINATED;
}

Process::Time Process::cpu_time()
{
    if (state_ != State::RUNNING) return 0ms;

    TimePoint end = std::chrono::high_resolution_clock::now();

    return (end - begin_);
}

void Process::read_file(const std::string& file_name)
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

        text_.push_back(std::move(instr));
    }

    file.close();
}

} // namespace OS
