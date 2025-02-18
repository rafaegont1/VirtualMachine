#include "OS/MMU.hpp"

// vetor com valores binários dos IDs de cada thread
// na hora de fazer a política do escalonador, cê aplica no vetor e transforma para decimal novamente

namespace OS::MMU {

unsigned long translate(std::bitset<32> virtual_addr)
{
    return virtual_addr.to_ulong();
    // return page_table[virtual_addr];
}

} // namespace OS::MMU
