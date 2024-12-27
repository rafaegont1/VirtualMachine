#include "Hardware/CPU/ALU.hpp"

#include <stdexcept> // for zero division execution

namespace Hardware::CPU {

int32_t ALU::add(const int32_t op_a, const int32_t op_b)
{
    int32_t result = op_a + op_b;
    set_flags(result);
    return result;
}

int32_t ALU::sub(const int32_t op_a, const int32_t op_b)
{
    int32_t result = op_a - op_b;
    set_flags(result);
    return result;
}

int32_t ALU::mul(const int32_t op_a, const int32_t op_b)
{
    int32_t result = op_a * op_b;
    set_flags(result);
    return result;
}

int32_t ALU::div(const int32_t op_a, const int32_t op_b)
{
    if (op_b == 0) {
        throw std::invalid_argument("Zero division!");
    }

    int32_t result = op_a / op_b;
    set_flags(result);
    return result;
}

bool ALU::eq(const int32_t op_a, const int32_t op_b)
{
    return op_a == op_b;
}

bool ALU::ne(const int32_t op_a, const int32_t op_b)
{
    return op_a != op_b;
}

bool ALU::lt(const int32_t op_a, const int32_t op_b)
{
    return op_a < op_b;
}

bool ALU::le(const int32_t op_a, const int32_t op_b)
{
    return op_a <= op_b;
}

bool ALU::gt(const int32_t op_a, const int32_t op_b)
{
    return op_a > op_b;
}

bool ALU::ge(const int32_t op_a, const int32_t op_b)
{
    return op_a >= op_b;
}

inline void ALU::set_flags(int32_t result)
{
    zero = (result == 0);
    negative = (result < 0);
    overflow = (result < 0 && result > INT32_MAX) ||
               (result > 0 && result < INT32_MIN);
}

} // namespace Hardware::CPU
