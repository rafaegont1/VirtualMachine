#ifndef CODE_HPP
#define CODE_HPP

#include <cstdint>
#include <vector>
#include <string>

namespace HW::ISA {

class Code {
public:
    using Line = std::vector<std::string>;

    static const Line NOP_LINE;

    Code(const std::string& file_name);
    const Line& fetch(const uint32_t pc) const;
    uint32_t loc() const; // get lines of code

private:
    std::vector<Line> text_;
};

} // namespace HW::ISA

#endif // CODE_HPP
