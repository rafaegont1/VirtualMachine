#ifndef MINHASH_SIGNATURE_HPP
#define MINHASH_SIGNATURE_HPP

#include <vector>

class MinHashSignature {
private:
    int N_;

    std::vector<std::vector<bool>> CM_;
    std::vector<std::vector<int>> SM_;

    std::vector<int> coefs_; //ith position stores Coeficient  of ith hash function.
    std::vector<int> tInd_;  //ith position stores Independent of ith hash function.

    void full_randoms();
    int hv(int i, int key);
    void compute_SM();

public:
    MinHashSignature(const std::vector<std::vector<bool>> &CM, int N);
    const std::vector<std::vector<int>>& SM();
};

#endif // MINHASH_SIGNATURE_HPP
