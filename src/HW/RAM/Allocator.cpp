#include "HW/RAM/Allocator.hpp"

namespace HW::RAM::Allocator {

std::shared_ptr<OS::PCB> create_process(const std::string& file, OS::PCB::Time quantum)
{
    return std::make_shared<OS::PCB>(file, quantum);
}

} // namespace HW::RAM::Allocator
