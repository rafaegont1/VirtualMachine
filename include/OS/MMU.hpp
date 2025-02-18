#ifndef MMU_HPP
#define MMU_HPP

#include <bitset>

namespace OS::MMU {

unsigned long translate(std::bitset<32> virtual_addr);

} // namespace OS::MMU

#endif // MMU_HPP
