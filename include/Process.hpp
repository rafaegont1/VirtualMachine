#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <vector>
#include <chrono>
#include <cstdint>
#include "CPU/RegFile.hpp"

class Process {
public:
    using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
    using Time = std::chrono::duration<double, std::milli>;

    struct CPUState {
        CPU::RegFile::Registers reg;
        uint32_t pc;
    };

    enum class State : uint8_t {
        READY,
        RUNNING,
        WAITING,
        TERMINATED
    };

    Process(const std::string& file_name, const Time quantum);
    virtual ~Process() = default;

    // setters and getters
    uint32_t get_pid() const;
    State get_state() const;
    void set_quantum(const Time quantum);
    Time get_quantum() const;
    Time get_timestamp() const;
    void set_cpu_state(const CPUState& cpu_state);
    const CPUState& get_cpu_state() const;

    const std::vector<std::string> * get_instr(const uint32_t pc) const;

    void start();
    void stop();
    void finish();

    Time get_cpu_time();

    // memory access methods
    void write_data(const std::string& var, int32_t data);
    int32_t read_data(const std::string& var) const;

private:
    static uint32_t count;

    inline void read_file(const std::string& file_name);

    uint32_t pid;
    State state;
    CPUState cpu_state;
    std::vector<std::vector<std::string>> text;

    TimePoint begin;
    Time quantum;
    Time timestamp;
};

#endif // PROCESS_HPP
