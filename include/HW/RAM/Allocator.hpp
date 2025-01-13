#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <memory>
#include "OS/PCB.hpp"

namespace HW::RAM::Allocator {

std::shared_ptr<OS::PCB> create_process(const std::string& file_name, OS::PCB::Time timestamp, OS::PCB::Time quantum, uint8_t priority = 0);

} // namespace HW::RAM::Allocator

#endif // MEMORY_HPP
