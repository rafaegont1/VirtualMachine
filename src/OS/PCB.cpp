#include "OS/PCB.hpp"

#include <iostream>

namespace OS {

uint32_t PCB::count_ = 0;

PCB::PCB(const std::string& filename, PCB::Time timestamp, Time quantum, uint8_t priority)
:   pid_{++count_},
    priority_{priority},
    code_{filename},
    quantum_{quantum},
    arrival_time_(timestamp)
{
    create_log();
}

PCB::PCB(const HW::ISA::Code& code, PCB::Time timestamp)
:   pid_{++count_},
    code_{code},
    arrival_time_(timestamp)
{
    create_log();
}

PCB::~PCB()
{
    if (log.is_open()) {
        log.close();
    }
}

void PCB::create_log()
{
    const std::string log_name = "pid_" + std::to_string(pid_) + "_output.txt";

    std::cout << "Log for file for process with PID " << pid_ << " will be saved in file: " << log_name << std::endl;

    log.open(log_name);
    if (!log.is_open()) {
        throw std::runtime_error("failed to open log file " + log_name);
    }

    log << "--- Process info ---\n"
        << "PID: "           << pid_                  << '\n'
        << "Quantum: "       << quantum_.count()      << " ms\n"
        << "Priority: "      << priority_             << '\n'
        << "LOC: "           << code_.size()          << '\n'
        << "Arrival time : " << arrival_time_.count() << " ms\n"
        << "==============================================================\n";
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

uint8_t PCB::get_priority() const
{
    return priority_;
}

const HW::ISA::Code::Line& PCB::fetch_line(const uint32_t pc) const
{
    // std::cout << "code size: " << code_.size() << std::endl; // rascunho
    return (pc < code_.size()) ? code_.fetch(pc) : HW::ISA::Code::NOP_LINE;
}

OS::PCB::Time PCB::get_arrival_time()
{
    return arrival_time_;
}

void PCB::update_burst_time(OS::PCB::Time cpu_time)
{
    burst_time_ += cpu_time;
}

OS::PCB::Time PCB::get_burst_time()
{
    return burst_time_;
}

void PCB::set_response_time(OS::PCB::Time timestamp)
{
    response_time_ = timestamp - arrival_time_;
}

OS::PCB::Time PCB::get_response_time()
{
    return response_time_;
}

void PCB::update_waiting_time(OS::PCB::Time timestamp)
{
    waiting_time_ = timestamp - arrival_time_ - burst_time_;
}

OS::PCB::Time PCB::get_waiting_time()
{
    return waiting_time_;
}

} // namespace OS
