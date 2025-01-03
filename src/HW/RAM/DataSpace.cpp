#include "HW/RAM/DataSpace.hpp"

#include <stdexcept>

namespace HW::RAM {

void DataSpace::write(const Variable& var)
{
    data_[var.name] = var.data;
}

int32_t DataSpace::read(const std::string& var_name)
{
    try {
        return data_.at(var_name);
    } catch (const std::out_of_range& e) {
        throw std::runtime_error(
            "Tried to access undeclared variable: " + var_name
        );
    }
}

void DataSpace::print_log(std::ofstream& log)
{
    log << "\nMEMORY DATA\n";

    if (data_.empty()) {
        log << "empty\n";
    } else {
        for (const auto& d : data_) {
            log << d.first << ": " << d.second << '\n';
        }
    }
}

} // namespace HW::RAM
