#include <iostream>
#include "VirtualMachine.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <nome_do_programa>" << std::endl;
        return 1;
    }

    VirtualMachine vm("log.txt");

    vm.create_proc(argv[1]);
    vm.run();

    return 0;
}
