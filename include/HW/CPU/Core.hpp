#ifndef CPU_HPP_
#define CPU_HPP_

#include <array>
#include <cstdint>
#include <fstream>
#include <thread>
#include "HW/CPU/Pipeline.hpp"
#include "HW/CPU/ALU.hpp"
#include "OS/PCB.hpp"
#include "OS/Scheduler.hpp"

namespace HW::CPU {

class Core {
public:
    Core();

    void run_cycle(std::shared_ptr<OS::PCB> proc);
    // bool is_available() const;

private:
    static uint32_t count;

    uint32_t id_;
    // OS::PCB* context_restore();
    // void context_switch(OS::PCB* proc);
    // void context_log(const std::string& label, const OS::PCB& proc);

    // bool available_ = true;
    uint32_t clk_ = 0;
    Pipeline pipeline_;

    // std::ofstream log_;
};

} // namespace HW::CPU

#endif // CPU_HPP_
