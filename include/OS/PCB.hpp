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

    // struct CPUState {
    //     HW::CPU::RegFile rf;
    //     uint32_t pc;
    // };

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
    void set_timestamp(Time new_value);
    Time get_timestamp() const;
    // void set_cpu_state(const HW::CPU::State& new_value);
    // HW::CPU::State get_cpu_state() const;

    const HW::ISA::Code::Line& fetch_line(const uint32_t pc) const;

    void start();
    void stop();
    // void terminate();

    Time cpu_time() const;

    // std::string info();

    HW::CPU::CPUState cpu_state;
    HW::RAM::DataSpace mem;
    std::ofstream log;

private:
    static uint32_t count_;

    uint32_t pid_;
    State state_;
    HW::ISA::Code code_;

    // TimePoint begin_;
    Time quantum_;
    // TODO: verificar se o timestamp é isso mesmo, ou deve ser usado de outra forma
    Time timestamp_;
};

} // namespace OS

#endif // PROCESS_HPP
