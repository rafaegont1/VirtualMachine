#include "HW/ISA/Code.hpp"

// #include <iostream> // rascunho
#include <fstream>
#include <sstream>

namespace HW::ISA {

const Code::Line Code::NOP_LINE = {"nop"};

Code::Code(const std::string& filename)
{
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file " + filename);
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

Code::Code(const std::vector<char>& doc)
{
    std::vector<std::string> line;
    std::string word;

    for (char c : doc) {
        // std::cout << c; // rascunho
        switch (c) {
            case ' ':
                if (!word.empty()) line.push_back(std::move(word));
                break;

            case '\n':
                if (!word.empty()) line.push_back(std::move(word));
                if (!line.empty()) text_.push_back(std::move(line));
                break;

            default:
                word.push_back(c);
                break;
        }
    }
    // std::cout << '\n'; // rascunho
    // for (const auto& line : text_) { // rascunho
    //     for (const auto& word : line) { // rascunho
    //         std::cout << word << ' '; // rascunho
    //     } // rascunho
    //     std::cout << '\n'; // rascunho
    // } // rascunho
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
