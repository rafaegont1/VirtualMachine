#include "HW/CPU/Cache.hpp"

namespace HW::CPU {

Cache::Cache(uint32_t capacity) : capacity_(capacity)
{
}

std::optional<int32_t> Cache::read(std::string key)
{
    std::lock_guard<std::mutex> guard(mtx_);
    std::optional<int32_t> value;

    if (cache_.find(key) != cache_.end()) {
        order_.erase(cache_[key].second);
        order_.push_front(key);
        cache_[key].second = order_.begin();

        value = cache_[key].first;
    } else {
        value = std::nullopt;
    }

    return value;
}

void Cache::write(std::string key, int32_t value)
{
    std::lock_guard<std::mutex> guard(mtx_);

    if (cache_.find(key) != cache_.end()) {
        cache_[key].first = value;
        order_.erase(cache_[key].second);
    } else {
        if (cache_.size() == capacity_) {
            std::string last = order_.back();
            order_.pop_back();
            cache_.erase(last);
        }
        cache_[key] = {value, order_.begin()};
    }

    order_.push_front(key);
    cache_[key].second = order_.begin();
}

uint32_t Cache::capacity() const
{
    return capacity_;
}

} // namespace HW::CPU
