#include "OS/Scheduler/FIFOScheduler.hpp"

namespace OS {

void FIFOScheduler::push(std::shared_ptr<OS::PCB> proc)
{
    std::lock_guard<std::mutex> guard(mtx_);

    queue_.push(proc);
}

std::shared_ptr<OS::PCB> FIFOScheduler::pop()
{
    std::lock_guard<std::mutex> guard(mtx_);

    if (queue_.empty()) {
        return nullptr;
    }

    std::shared_ptr<OS::PCB> proc = queue_.front();
    queue_.pop();

    return proc;
}

bool FIFOScheduler::empty()
{
    std::lock_guard<std::mutex> guard(mtx_);
    return queue_.empty();
}

} // namespace OS
