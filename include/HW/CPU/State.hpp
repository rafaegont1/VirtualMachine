#ifndef STATE_HPP
#define STATE_HPP

#include <cstdint>
#include "HW/CPU/RegFile.hpp"

namespace HW::CPU {

struct State {
    State();

    HW::CPU::RegFile rf;
    uint32_t pc;
};

}// namespace HW::CPU

#endif // STATE_HPP
