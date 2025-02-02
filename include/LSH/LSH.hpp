#ifndef LSH_HPP
#define LSH_HPP

// #include <fstream>
// #include <set>
#include <vector>

class LSH {
public:
    // Construtor
    LSH(int k, int N);
    const std::vector<int>& compute(int argc, char** argv);
    double get_similarity(int doc1, int doc2);
    double total_similarity(const std::vector<int>& queue);
    // void display_organized_texts();
    const std::vector<int>& queue() const;
    const std::vector<std::vector<char>>& docs() const;
    int ndocs() const;

private:
    void read_documents(char** argv);
    void organize_texts();

    int k_; // Número de funções de hash por tabela
    int N_; // Número de tabelas hash
    int ndocs_;
    std::vector<std::vector<char>> docs_;
    std::vector<std::vector<bool>> CM_;
    std::vector<std::vector<int>> SM_;
    std::vector<int> queue_;
};

#endif // LSH_HPP
