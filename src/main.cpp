#include <iostream>
#include "OS/Minix.hpp"

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <program_name>" << std::endl;
        exit(EXIT_FAILURE);
    }

    OS::Minix vm(argc, argv);
    vm.run();

    return 0;
}
