#include "HW/ISA/Code.hpp"

#include <fstream>
#include <sstream>

namespace HW::ISA {

const Code::Line Code::NOP_LINE = {"nop"};

Code::Code(const std::string& file_name)
{
    std::ifstream file(file_name);
    std::string line;

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file " + file_name);
    }

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::vector<std::string> instr;
        std::string word;

        while (ss >> word) {
            instr.push_back(std::move(word));
        }

        text_.push_back(std::move(instr));
    }

    file.close();
}

const Code::Line& Code::fetch(const uint32_t pc) const
{
    return text_[pc];
}

uint32_t Code::size() const
{
    return static_cast<uint32_t>(text_.size());
}

} // namespace HW::ISA
