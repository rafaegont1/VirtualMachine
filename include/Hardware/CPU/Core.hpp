#ifndef CPU_HPP_
#define CPU_HPP_

#include <array>
#include <cstdint>
#include <fstream>
#include "Hardware/CPU/Pipeline.hpp"
#include "Hardware/CPU/ALU.hpp"
#include "Hardware/RAM.hpp"
#include "OS/Process.hpp"

namespace Hardware::CPU {

class Core {
public:
    Core();
    virtual ~Core();

    void run(RAM& mem);

private:
    static uint8_t count_;

    OS::Process* context_restore(RAM& mem);
    void context_switch(OS::Process* proc, RAM& mem);

    static const std::vector<std::string> NOP_INSTR;

    uint32_t clk = 0;
    RegFile rf_;
    uint32_t pc_;
    Pipeline pipeline_;

    std::ofstream log_;
};

} // namespace Hardware::CPU

#endif // CPU_HPP_
