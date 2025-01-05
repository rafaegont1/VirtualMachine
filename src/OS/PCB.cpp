#include "OS/PCB.hpp"

#include <iostream>

namespace OS {

uint32_t PCB::count_ = 0;

PCB::PCB(const std::string& file_name, const Time quantum) :
    pid_{++count_},
    state_{State::NEW},
    code_{file_name},
    quantum_{quantum}
{
    const std::string log_name = "pid_" + std::to_string(pid_) + "_output.txt";

    std::cout << "Log for file '" << file_name << "' for process with PID "
              << pid_ << " will be saved in file: " << log_name << std::endl;

    log.open(log_name);
    if (!log.is_open()) {
        throw std::runtime_error("failed to open log file " + log_name);
    }

    log << "--- Process info ---\n"
        << "PID: "     << pid_             << '\n'
        << "Quantum: " << quantum_.count() << " ms\n"
        << "==============================================================\n";
}

PCB::~PCB()
{
    if (log.is_open()) {
        log.close();
    }
}

uint32_t PCB::get_pid() const
{
    return pid_;
}

void PCB::set_state(PCB::State new_value)
{
    state_ = new_value;
}

PCB::State PCB::get_state() const
{
    return state_;
}

void PCB::set_quantum(Time new_value)
{
    quantum_ = new_value;
}

PCB::Time PCB::get_quantum() const
{
    return quantum_;
}

const HW::ISA::Code::Line& PCB::fetch_line(const uint32_t pc) const
{
    return (pc < code_.size()) ? code_.fetch(pc) : HW::ISA::Code::NOP_LINE;
}

} // namespace OS
