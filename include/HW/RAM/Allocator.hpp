#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <memory>
#include "OS/PCB.hpp"

namespace HW::RAM::Allocator {

std::shared_ptr<OS::PCB> create_process(const std::string& file, OS::PCB::Time quantum);

} // namespace HW::RAM::Allocator

#endif // MEMORY_HPP
