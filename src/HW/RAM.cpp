#include "HW/RAM.hpp"

#include <iostream> // rascunho

namespace HW {

std::shared_ptr<OS::PCB> RAM::read(uint32_t addr) const
{
    auto it = page_frames_.find(addr);
    if (it != page_frames_.end()) {
        return it->second;
    }
    return nullptr;
}

void RAM::write(std::shared_ptr<OS::PCB> pcb)
{
    // Page frame size
    constexpr uint32_t FRAME_SIZE = 4096;

    // Set base and limit addresses
    pcb->base_addr(free_addr_);
    pcb->limit_addr(free_addr_ + FRAME_SIZE - 1);
    pcb->create_initial_log();

    // Update free address base
    free_addr_ += FRAME_SIZE;
    // std::cout << "free_addr_: " << free_addr_ << std::endl; // rascunho

    // Push the PCB into memory
    page_frames_[pcb->base_addr()] = pcb;
}

} // namespace HW
