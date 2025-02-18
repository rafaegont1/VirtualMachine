#include "OS/Scheduler/FIFOScheduler.hpp"

namespace OS {

void FIFOScheduler::push(std::bitset<32> proc)
{
    std::lock_guard<std::mutex> guard(mtx_);

    queue_.push(proc);
}

std::bitset<32> FIFOScheduler::pop()
{
    std::lock_guard<std::mutex> guard(mtx_);

    std::bitset<32> proc_virtual_addr = queue_.front();
    queue_.pop();

    return proc_virtual_addr;
}

bool FIFOScheduler::empty()
{
    std::lock_guard<std::mutex> guard(mtx_);
    return queue_.empty();
}

} // namespace OS
