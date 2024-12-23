#include "OS/Bootloader.hpp"

#include <fstream>
#include <sstream>

namespace OS {

void Bootloader::init(int argc, char ** argv)
{
    for (int i = 1; i < argc; i++) {
        vm.create_process(argv[i]);
    }
}

inline std::vector<std::vector<std::string>> Bootloader::read_file(const std::string & file_name)
{
    std::vector<std::vector<std::string>> text;
    std::ifstream file(file_name);
    std::string line;

    if (not file.is_open()) {
        throw std::runtime_error("Error: failed to open file " + file_name);
    }

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<std::string> instr;
        std::string word;

        while (iss >> word) {
            instr.push_back(std::move(word));
        }

        text.push_back(std::move(instr));
    }

    file.close();

    return text;
}

} // namespace OS
