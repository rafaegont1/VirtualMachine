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

    PCB(const std::string& file_name, const Time quantum);
    virtual ~PCB();

    // setters and getters
    uint32_t get_pid() const;
    State get_state() const;
    void set_state(PCB::State new_value);
    void set_quantum(Time new_value);
    Time get_quantum() const;

    const HW::ISA::Code::Line& fetch_line(const uint32_t pc) const;

    Time cpu_time() const;

    HW::CPU::CPUState cpu_state;
    HW::RAM::DataSpace mem;
    std::ofstream log;

private:
    static uint32_t count_;

    uint32_t pid_;
    State state_;
    HW::ISA::Code code_;

    Time quantum_;
};

} // namespace OS

#endif // PROCESS_HPP
