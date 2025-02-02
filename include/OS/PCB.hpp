#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <chrono>
#include <cstdint>
#include <fstream>
#include "HW/ISA/Code.hpp"
#include "HW/CPU/CPUState.hpp"
#include "HW/RAM/DataSpace.hpp"

namespace OS {

class PCB {
public:
    using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
    using Time = std::chrono::duration<double, std::milli>;

    enum class State : uint8_t {
        NEW,
        READY,
        RUNNING,
        WAITING,
        TERMINATED
    };

    PCB(const std::string& filename, PCB::Time timestamp, Time quantum, uint8_t priority = 0);
    PCB(const HW::ISA::Code& code, PCB::Time timestamp);
    virtual ~PCB();

    void create_log();

    // setters and getters
    uint32_t get_pid() const;
    State get_state() const;
    void set_state(PCB::State new_value);
    void set_quantum(Time new_value);
    Time get_quantum() const;
    uint8_t get_priority() const;

    OS::PCB::Time get_arrival_time();
    void update_burst_time(OS::PCB::Time cpu_time);
    OS::PCB::Time get_burst_time();
    void set_response_time(OS::PCB::Time timestamp);
    OS::PCB::Time get_response_time();
    void update_waiting_time(OS::PCB::Time timestamp);
    OS::PCB::Time get_waiting_time();

    const HW::ISA::Code::Line& fetch_line(const uint32_t pc) const;

    HW::CPU::CPUState cpu_state;
    HW::RAM::DataSpace mem;
    std::ofstream log;

private:
    static uint32_t count_;

    uint32_t pid_;
    uint16_t priority_;
    State state_ = State::NEW;
    HW::ISA::Code code_;

    // • Arrival time: the time when a process enters into the ready state and
    //   is ready for its execution.
    //
    // • Burst time: total time taken by the process for its execution on the
    //   CPU.
    //
    // • Response time: time spent when the process is in the ready state and
    //   gets the CPU for the first time.
    //
    // • Waiting time: total time spent by the process in the ready state
    //   waiting for CPU.

    Time quantum_;
    Time arrival_time_;
    Time response_time_;
    Time burst_time_ = std::chrono::milliseconds(0);
    Time waiting_time_;
};

} // namespace OS

#endif // PROCESS_HPP
