#ifndef ALU_HPP
#define ALU_HPP

#include <cstdint>

namespace HW::CPU::ALU {

union Result {
    int32_t as_num;
    bool as_bool;
};

// arithmetic operation methods
int32_t add(const int32_t op_a, const int32_t op_b);
int32_t sub(const int32_t op_a, const int32_t op_b);
int32_t mul(const int32_t op_a, const int32_t op_b);
int32_t div(const int32_t op_a, const int32_t op_b);

// comparison operation methods
bool eq(const int32_t op_a, const int32_t op_b);
bool ne(const int32_t op_a, const int32_t op_b);
bool lt(const int32_t op_a, const int32_t op_b);
bool le(const int32_t op_a, const int32_t op_b);
bool gt(const int32_t op_a, const int32_t op_b);
bool ge(const int32_t op_a, const int32_t op_b);

} // namespace HW::CPU::ALU

#endif // ALU_HPP
