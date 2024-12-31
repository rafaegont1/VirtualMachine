#include "OS/PCB.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include "Hardware/CPU/Instruction.hpp"

namespace OS {

using namespace std::chrono_literals;

uint32_t PCB::count_ = 0;

PCB::PCB(const std::string& file_name, const Time quantum) :
    pid_{++count_},
    state_{State::READY},
    quantum_{quantum},
    timestamp_{0ms}
{
    read_file(file_name);

    cpu_state_.gpr[Hardware::CPU::RegFile::ZERO] = 0;
    for (int n = 1; n < Hardware::CPU::RegFile::NUM_REGISTERS; n++) {
        cpu_state_.gpr[n] = n + 100;
    }
}

uint32_t PCB::pid() const { return pid_; }

PCB::State PCB::state() const { return state_; }

void PCB::quantum(const Time new_value) { quantum_ = new_value; }

PCB::Time PCB::quantum() const { return quantum_; }

PCB::Time PCB::timestamp() const { return timestamp_; }

const std::vector<std::string>* PCB::get_instr(const uint32_t pc) const
{
    return (pc < text_.size())
        ? &text_[pc]
        : &Hardware::CPU::Instruction::NOP_INSTR;
}

void PCB::cpu_state(const PCB::CPUState& cpu_state)
{
    cpu_state_ = cpu_state;
}

const PCB::CPUState& PCB::cpu_state() const { return cpu_state_; }

void PCB::start()
{
    state_ = State::RUNNING;
    begin_ = std::chrono::high_resolution_clock::now();
}

void PCB::stop()
{
    // update timestamp
    TimePoint end = std::chrono::high_resolution_clock::now();
    timestamp_ += (end - begin_);

    // set process as ready to run again
    state_ = State::READY;
}

void PCB::terminate()
{
    state_ = State::TERMINATED;
}

PCB::Time PCB::cpu_time() const
{
    if (state_ != State::RUNNING) return 0ms;

    TimePoint end = std::chrono::high_resolution_clock::now();

    return (end - begin_);
}

std::string PCB::info()
{
    return "PID: "       + std::to_string(pid())               + "\n"
           "Quantum: "   + std::to_string(quantum().count())   + "\n"
           "CPU time: "  + std::to_string(cpu_time().count())  + "\n"
           "Timestamp: " + std::to_string(timestamp().count()) + "\n";
}

void PCB::read_file(const std::string& file_name)
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
