#ifndef CACHE_HPP
#define CACHE_HPP

#include <string>
#include <unordered_map>
#include <list>
#include <optional>
#include <cstdint>
#include <mutex>

namespace HW::CPU {

// Least Recently Used (LRU) caching algorithm
class Cache {
public:
    Cache(uint32_t capacity);

    std::optional<int32_t> read(std::string key);
    void write(std::string key, int32_t value);
    uint32_t capacity() const;

private:
    uint32_t capacity_; // Capacidade do cache
    std::list<std::string> order_; // Lista para manter a ordem de uso dos itens (lista duplamente encadeada)
    std::unordered_map<std::string, std::pair<int32_t, std::list<std::string>::iterator>> cache_; // Mapa de chave -> (valor, posição na lista)
    std::mutex mtx_;
};

} // namespace HW::CPU

#endif // CACHE_HPP
