#ifndef PROCESS_HPP_
#define PROCESS_HPP_

#include <vector>
#include <chrono>
#include <cstdint>
#include "Hardware/CPU/RegFile.hpp"

namespace OS {

class PCB {
public:
    using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
    using Time = std::chrono::duration<double, std::milli>;

    struct CPUState {
        Hardware::CPU::RegFile::Registers gpr;
        uint32_t pc;
    };

    enum class State : uint8_t {
        READY,
        RUNNING,
        WAITING,
        TERMINATED
    };

    PCB(const std::string& file_name, const Time quantum);

    // setters and getters
    uint32_t pid() const;
    State state() const;
    void quantum(const Time new_value);
    Time quantum() const;
    Time timestamp() const;
    void cpu_state(const CPUState& cpu_state);
    const CPUState& cpu_state() const;

    const std::vector<std::string>* get_instr(const uint32_t pc) const;

    void start();
    void stop();
    void terminate();

    Time cpu_time() const;

    std::string info();

private:
    static uint32_t count_;

    void read_file(const std::string& file_name);

    uint32_t pid_;
    State state_;
    CPUState cpu_state_;
    std::vector<std::vector<std::string>> text_;

    TimePoint begin_;
    Time quantum_;
    Time timestamp_;
};

} // namespace OS

#endif // PROCESS_HPP_
