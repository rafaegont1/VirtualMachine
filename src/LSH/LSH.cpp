#include "LSH/LSH.hpp"

#include <fstream>
// #include <iostream> // rascunho
#include <set>
#include "LSH/CharacteristicMatrix.hpp"
#include "LSH/MinHashSignature.hpp"

// Construtor
LSH::LSH(int k, int N) : k_{k}, N_{N} {
}

const std::vector<int>& LSH::compute(int argc, char** argv) {
    ndocs_ = argc - 1;

    // Ler documentos
    read_documents(argv);

    // Construir matriz de características
    CharacteristicMatrix cm(k_, docs_);
    CM_ = cm.CM();

    // Calcular assinaturas MinHash
    MinHashSignature sm(CM_, N_);
    SM_ = sm.SM();

    organize_texts();

    return queue_;
}

// Ler documentos
void LSH::read_documents(char** argv) {
    std::ifstream file;
    std::string out;

    for (int index = 1; index <= ndocs_; index++) {
        const char* filename = argv[index];
        file.open(filename);
        std::vector<char> document;
        char c;
        while (file.get(c)) {
            document.push_back(c);
            // file >> out;
            // for (size_t i = 0; i < out.size(); i++) {
            //     document.push_back(out[i]);
            // }
            // document.push_back(' ');  // Incluir espaço como shingle
        }
        // for (char c : document) std::cout << c; // rascunho
        // std::cout << "\n-----\n"; // rascunho
        docs_.push_back(document);
        file.close();
    }
}

// Função para obter a similaridade entre dois documentos
double LSH::get_similarity(int doc1, int doc2) {
    double value = 0.0;
    for (int k = 0; k < N_; k++) {
        if (SM_[k][doc1] == SM_[k][doc2]) value++;
    }
    return value / N_;
}

// Função para calcular a similaridade total de uma fila
double LSH::total_similarity(const std::vector<int>& queue) {
    double total = 0.0;
    for (size_t i = 0; i < queue.size() - 1; ++i) {
        total += get_similarity(queue[i], queue[i + 1]);
    }
    return total;
}

// Heurística para organizar os textos
void LSH::organize_texts() {
    std::vector<int> texts(ndocs_);
    for (int i = 0; i < ndocs_; i++) {
        texts[i] = i;
    }

    // Comece com o documento que tem a maior similaridade média
    int start_doc = -1;
    double max_avg_similarity = -1.0;

    for (const auto& doc : texts) {
        double total = 0.0;
        for (const auto& other : texts) {
            if (doc != other) {
                total += get_similarity(doc, other);
            }
        }
        double avg_similarity = total / (texts.size() - 1);
        if (avg_similarity > max_avg_similarity) {
            max_avg_similarity = avg_similarity;
            start_doc = doc;
        }
    }

    queue_ = {start_doc};
    std::set<int> remaining_docs(texts.begin(), texts.end());
    remaining_docs.erase(start_doc);

    while (!remaining_docs.empty()) {
        int last_doc = queue_.back();
        int next_doc = -1;
        double max_similarity = -1.0;

        for (const auto& doc : remaining_docs) {
            double similarity = get_similarity(last_doc, doc);
            if (similarity > max_similarity) {
                max_similarity = similarity;
                next_doc = doc;
            }
        }

        queue_.push_back(next_doc);
        remaining_docs.erase(next_doc);
    }
}

// Método para exibir a fila ordenada e a similaridade total
// void LSH::display_organized_texts() {
//     std::vector<int> ordered_texts = organize_texts();

//     // Exibir a fila ordenada
//     std::cout << "Fila ordenada (do mais similar ao menos similar):\n";
//     for (size_t i = 0; i < ordered_texts.size(); ++i) {
//         std::cout << "text" << ordered_texts[i] + 1;
//         if (i < ordered_texts.size() - 1) {
//             std::cout << " -> ";
//         }
//     }
//     std::cout << "\n";

//     // Calcular a similaridade total da fila
//     double total_sim = total_similarity(ordered_texts);
//     std::cout << "Similaridade total da fila: " << std::fixed << std::setprecision(3) << total_sim << "\n";
// }

const std::vector<int>& LSH::queue() const
{
    return queue_;
}

const std::vector<std::vector<char>>& LSH::docs() const
{
    return docs_;
}

int LSH::ndocs() const
{
    return ndocs_;
}
