#include "LSH/CharacteristicMatrix.hpp"

// PUBLIC METHODS
CharacteristicMatrix::CharacteristicMatrix(int k, const std::vector<std::vector<char>> &docs)
: k_(k), ndocs_(docs.size()), docs_(docs)
{
    generate_kshingles();
    hash_shingles();
    generate_universal();
    compute_CM();
}

const std::vector<std::vector<bool>>& CharacteristicMatrix::CM()
{
    return CM_;
}

void CharacteristicMatrix::generate_kshingles()
{
    // Generar el k-shingles dels dos documents
    kshings_ = std::vector<std::vector<std::string>>(ndocs_);

    for (int index = 0; index<ndocs_; index++){
        std::vector<char> kshingle(k_);

        for (int i = 0; i < k_; i++) {
            kshingle[i] = docs_[index][i];
        }

        std::string srt(kshingle.begin(), kshingle.end());
        kshings_[index].push_back(srt);

        for (size_t i = k_; i < docs_[index].size(); i++) {
            kshingle.erase(kshingle.begin());
            kshingle.push_back(docs_[index][i]);
            std::string srt(kshingle.begin(), kshingle.end());
            kshings_[index].push_back(srt);
        }
    }
}

void CharacteristicMatrix::hash_shingles()
{
    // Fer Hashing dels k-shingles
    std::hash<std::string> h;
    buckets_ = std::vector<std::map<int,int>> (ndocs_);
    size_t kshingsize = kshings_[0].size();
    for (int index = 1; index<ndocs_; index++){
       if (kshings_[index].size() > kshingsize) kshingsize = kshings_[index].size();
    }

    for (int index = 0; index<ndocs_; index++){
        for (size_t i = 0; i < kshings_[index].size(); i++) {
            buckets_[index][h(kshings_[index][i]) % kshingsize] = h(kshings_[index][i]) % kshingsize;
        }
    }
}

void CharacteristicMatrix::generate_universal()
{
    // Generar l'Universal k-shingle
    for (int index = 0; index < ndocs_; index++){
        std::map<int,int>::iterator it = buckets_[index].begin();
        while ((it != buckets_[index].end())) {
            universal_kshingles_[it->first] = it->first;
            it++;
        }
    }
}

void CharacteristicMatrix::compute_CM()
{
    // Calcul de la Matriu Caracteristica
    CM_ = std::vector<std::vector<bool>>(universal_kshingles_.size(), std::vector<bool>(ndocs_, false));
    std::map<int,int>::iterator u_it = universal_kshingles_.begin();
    int i_uni = 0;

    while (u_it != universal_kshingles_.end()) { // row iterator
        for (int index = 0; index < ndocs_; index++) {// column iterator
            if (buckets_[index].find(u_it->second) != buckets_[index].end()) { // means element is in Set1
                CM_[i_uni][index] = true;
            }
        }
        i_uni++;
        u_it++;
    }
}
