#include "OS/PCB.hpp"

#include <iostream>

namespace OS {

uint32_t PCB::count_ = 0;

PCB::PCB(const std::string& file_name, const Time quantum) :
    pid_{++count_},
    state_{State::NEW},
    code_{file_name},
    quantum_{quantum},
    timestamp_{std::chrono::milliseconds(0)}
{
    const std::string log_name = "pid_" + std::to_string(pid_) + "_output.txt";

    std::cout << "Log for file '" << file_name << "' for process with PID "
              << pid_ << " will be saved in file: " << log_name << std::endl;

    log.open(log_name);
    if (!log.is_open()) {
        throw std::runtime_error("failed to open log file " + log_name);
    }
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

PCB::Time PCB::get_timestamp() const
{
    return timestamp_;
}

// void PCB::set_cpu_state(const HW::CPU::State& new_value)
// {
//     cpu_state_ = new_value;
// }

// HW::CPU::State PCB::get_cpu_state() const
// {
//     return cpu_state_;
// }

void PCB::set_timestamp(Time new_value)
{
    timestamp_ = new_value;
}

const HW::ISA::Code::Line& PCB::fetch_line(const uint32_t pc) const
{
    return (pc < code_.loc()) ? code_.fetch(pc) : HW::ISA::Code::NOP_LINE;
}

// void PCB::start()
// {
//     state_ = State::RUNNING;
//     begin_ = std::chrono::high_resolution_clock::now();
// }

// void PCB::stop()
// {
//     // update timestamp
//     TimePoint end = std::chrono::high_resolution_clock::now();
//     timestamp_ += (end - begin_);

//     // set process as ready to run again
//     state_ = State::READY;
// }

// PCB::Time PCB::cpu_time() const
// {
//     if (state_ != State::RUNNING) return 0ms;

//     TimePoint end = std::chrono::high_resolution_clock::now();

//     return (end - begin_);
// }

} // namespace OS
