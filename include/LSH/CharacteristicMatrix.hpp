#ifndef CHARACTERISTIC_MATRIX_HPP
#define CHARACTERISTIC_MATRIX_HPP

#include <vector>
#include <map>
#include <string>
#include <cstdlib>

class CharacteristicMatrix {
private:
    int k_;
    int ndocs_;
    std::vector<std::vector<char>> docs_;
    std::vector<std::vector<std::string>> kshings_;
    std::vector<std::map<int, int>> buckets_;
    std::map<int, int> universal_kshingles_;
    std::vector<std::vector <bool>> CM_;

    void generate_kshingles();
    void hash_shingles();
    void generate_universal();
    void compute_CM();

public:
    CharacteristicMatrix(int k, const std::vector<std::vector<char>> &docs);
    const std::vector<std::vector<bool>>& CM();
};

#endif // CHARACTERISTIC_MATRIX_HPP
