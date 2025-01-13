#include "HW/RAM/Allocator.hpp"

namespace HW::RAM::Allocator {

std::shared_ptr<OS::PCB> create_process(const std::string& file_name, OS::PCB::Time timestamp, OS::PCB::Time quantum, uint8_t priority)
{
    return std::make_shared<OS::PCB>(file_name, timestamp, quantum, priority);
}

} // namespace HW::RAM::Allocator
