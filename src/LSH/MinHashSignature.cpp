#include "LSH/MinHashSignature.hpp"

#include <cstdlib>

MinHashSignature::MinHashSignature(const std::vector<std::vector<bool>> &CM, int N)
: N_(N), CM_(CM)
{
    full_randoms();
    compute_SM();
}

void MinHashSignature::full_randoms()
{
    //Omplir std::vectors per al diversos Hashings de forma aleatoria
    for (int i = 0; i < N_; i++) {
        coefs_.push_back(rand() % 100);
        tInd_.push_back(rand() % 100);
    }
}

int MinHashSignature::hv(int i, int key)
{
    //Retorna el Hashing 'i' per a la clau key (return Hi(key))
    int coef = coefs_[i];
    int indep_term = tInd_[i];
    return ((coef * key + indep_term) % CM_.size());
}

void MinHashSignature::compute_SM()
{
    //Calcul de la Matriu de Signatures
    //Sets are columns of M, rows are each hi (0<=i<N) we must applicate
    //sig(h,c) = min(sig(h,c) , h(h,r).
    int ndocs = CM_[0].size();
    SM_ = std::vector<std::vector<int>> (N_, std::vector<int>(ndocs, CM_.size()));
    for (size_t r = 0; r < CM_.size(); r++) {
        for (int c = 0; c < ndocs; c++) {
            if (CM_[r][c]) {
                for (int h = 0; h < N_; h++) {
                    SM_[h][c] = std::min(hv(h,r), SM_[h][c]);
                }
            }
        }
    }
}

const std::vector<std::vector<int>>& MinHashSignature::SM()
{
    return SM_;
}
