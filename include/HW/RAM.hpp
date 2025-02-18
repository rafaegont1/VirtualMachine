#ifndef RAM_HPP
#define RAM_HPP

#include <memory>
#include <unordered_map>
#include "OS/PCB.hpp"

namespace HW {

class RAM {
public:
    std::shared_ptr<OS::PCB> read(uint32_t addr) const;
    void write(std::shared_ptr<OS::PCB> pcb);

private:
    std::unordered_map<uint32_t, std::shared_ptr<OS::PCB>> page_frames_;
    uint32_t free_addr_ = 0;
};

} // namespace HW

#endif // RAM_HPP
