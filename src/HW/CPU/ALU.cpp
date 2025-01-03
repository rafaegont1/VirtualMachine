#include "HW/CPU/ALU.hpp"

#include <stdexcept> // for zero division execution

namespace HW::CPU::ALU {

int32_t add(const int32_t op_a, const int32_t op_b)
{
    return op_a + op_b;
}

int32_t sub(const int32_t op_a, const int32_t op_b)
{
    return op_a - op_b;
}

int32_t mul(const int32_t op_a, const int32_t op_b)
{
    return op_a * op_b;
}

int32_t div(const int32_t op_a, const int32_t op_b)
{
    if (op_b == 0) {
        throw std::invalid_argument("Zero division!");
    }

    return op_a / op_b;
}

bool eq(const int32_t op_a, const int32_t op_b)
{
    return op_a == op_b;
}

bool ne(const int32_t op_a, const int32_t op_b)
{
    return op_a != op_b;
}

bool lt(const int32_t op_a, const int32_t op_b)
{
    return op_a < op_b;
}

bool le(const int32_t op_a, const int32_t op_b)
{
    return op_a <= op_b;
}

bool gt(const int32_t op_a, const int32_t op_b)
{
    return op_a > op_b;
}

bool ge(const int32_t op_a, const int32_t op_b)
{
    return op_a >= op_b;
}

} // namespace HW::CPU::ALU
