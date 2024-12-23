#ifndef CPU_HPP
#define CPU_HPP

#include <array>
#include <cstdint>
#include <fstream>
#include "CPU/Pipeline.hpp"
#include "CPU/ALU.hpp"
#include "RAM.hpp"
#include "Process.hpp"

namespace CPU {

class Core {
public:
    Core(RAM & mem, const std::string & log_file);
    virtual ~Core();

    void run();

private:
    void perform_in_thread();

    static const std::vector<std::string> NOP_INSTR;

    uint32_t clk = 0;
    RegFile rf;
    uint32_t pc;
    RAM & mem;
    ALU alu;
    Pipeline pipeline;

    std::ofstream log;
};

} // namespace CPU

#endif // CPU_HPP
