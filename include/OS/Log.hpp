#ifndef LOG_HPP
#define LOG_HPP

#include <fstream>

namespace OS {

class Log {
public:
    Log(const std::string& log_name);
    virtual ~Log();

    void write_buffer();

    std::string buffer;

private:
    std::ofstream file_;
};

} // namespace OS

#endif // LOG_HPP
