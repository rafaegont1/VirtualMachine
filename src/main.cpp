#include <iostream>
#include "VirtualMachine.hpp"

int main(int argc, char ** argv)
{
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <program_name>" << std::endl;
        exit(EXIT_FAILURE);
    }

    VirtualMachine vm;

    for (int i = 1; i < argc; i++) {
        vm.create_process(argv[i]);
    }

    vm.run();

    return 0;
}
