#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include <queue>
#include <memory>
#include "OS/Scheduler/SchedulerBase.hpp"
#include "OS/PCB.hpp"

namespace OS {

class PriorityQueue : public SchedulerBase {
public:
    void push(std::shared_ptr<OS::PCB> proc) override;
    std::shared_ptr<OS::PCB> pop() override;
    bool empty() override;

private:
    class PriorityComparator {
    public:
        bool operator()(const std::shared_ptr<OS::PCB>& p1, const std::shared_ptr<OS::PCB>& p2) const
        {
            return p1->get_priority() < p2->get_priority();
        }
    };

    std::priority_queue<std::shared_ptr<OS::PCB>, std::vector<std::shared_ptr<OS::PCB>>, PriorityComparator> queue_;
};

} // namespace OS

#endif // PRIORITY_QUEUE_HPP
