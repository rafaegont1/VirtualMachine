#include "OS/Scheduler/PriorityQueue.hpp"

namespace OS {

void PriorityQueue::push(std::shared_ptr<OS::PCB> proc)
{
    std::lock_guard<std::mutex> guard(mtx_);

    queue_.push(proc);
}

std::shared_ptr<OS::PCB> PriorityQueue::pop()
{
    std::lock_guard<std::mutex> guard(mtx_);

    if (queue_.empty()) {
        return nullptr;
    }

    std::shared_ptr<OS::PCB> proc = queue_.top();
    queue_.pop();

    return proc;
}

bool PriorityQueue::empty()
{
    std::lock_guard<std::mutex> guard(mtx_);
    return queue_.empty();
}

} // namespace OS
