#include <iostream>
#include "OS/VirtualMachine.hpp"

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <program_name>" << std::endl;
        exit(EXIT_FAILURE);
    }

    OS::VirtualMachine vm;

    vm.bootloader(argc, argv);
    vm.run();

    return 0;
}
