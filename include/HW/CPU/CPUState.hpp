#ifndef STATE_HPP
#define STATE_HPP

#include <cstdint>
#include "HW/CPU/RegFile.hpp"
#include "HW/ISA/Encoding.hpp"

namespace HW::CPU {

struct PipelineState {
    uint32_t stall_count_ = 0;
    // current instructions in pipeline
    HW::ISA::Encoding if_enc;
    HW::ISA::Encoding id_enc;
    HW::ISA::Encoding ex_enc;
    HW::ISA::Encoding mem_enc;
    HW::ISA::Encoding wb_enc;
};

struct CPUState {
    RegFile rf;
    PipelineState pipeline;
    uint32_t pc = 0;
};

}// namespace HW::CPU

#endif // STATE_HPP
