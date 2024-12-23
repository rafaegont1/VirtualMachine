#ifndef ALU_HPP
#define ALU_HPP

#include <cstdint>

namespace CPU {

class ALU {
public:
    ALU();
    virtual ~ALU() = default;

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

    // flag getters
    bool zero_flag() const;
    bool negative_flag() const;
    bool overflow_flag() const;

private:
    // status flags
    bool zero;
    bool negative;
    bool overflow;

    // method to update flags after an operation
    inline void set_flags(int32_t result);
};

} // namespace CPU

#endif // ALU_HPP
