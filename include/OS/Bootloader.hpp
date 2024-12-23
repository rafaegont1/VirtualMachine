#ifndef BOOTLOADER_HPP
#define BOOTLOADER_HPP

#include <string>
#include <vector>

namespace OS {

class Bootloader {

public:
    Bootloader();
    virtual ~Bootloader() = default;

    void init(int argc, char ** argv);

private:
    inline std::vector<std::vector<std::string>> read_file(const std::string & file_name);
};

} // namespace OS

#endif // BOOTLOADER_HPP
