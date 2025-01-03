#ifndef DATASPACE_HPP
#define DATASPACE_HPP

#include <cstdint>
#include <fstream>
#include <string>
#include <unordered_map>

namespace HW::RAM {

class DataSpace {
public:
    struct Variable {
        std::string name;
        int32_t data;
    };

    void write(const Variable& var);
    int32_t read(const std::string& var_name);
    void print_log(std::ofstream& log);

private:
    std::unordered_map<std::string, int32_t> data_;
};

} // namespace HW::RAM

#endif // DATASPACE_HPP
