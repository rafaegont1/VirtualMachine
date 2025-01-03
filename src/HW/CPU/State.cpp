#include "HW/CPU/State.hpp"

namespace HW::CPU {

State::State()
{
    gpr[HW::CPU::RegFile::ZERO] = 0;

    for (int n = 1; n < HW::CPU::RegFile::NUM_REGISTERS; n++) {
        gpr[n] = n + 100;
    }
}

}// namespace HW::CPU
