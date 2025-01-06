#ifndef CPU_HPP_
#define CPU_HPP_

#include <cstdint>
#include "OS/PCB.hpp"
#include "HW/CPU/Pipeline.hpp"

namespace HW::CPU {

class Core {
public:
    Core();

    void run_cycle(std::shared_ptr<OS::PCB> proc, OS::PCB::TimePoint timestamp_begin);

private:
    static uint32_t count;

    uint32_t id_;

    uint32_t clk_ = 0;
    Pipeline pipeline_;
};

} // namespace HW::CPU

#endif // CPU_HPP_
