#include "OS/Log.hpp"

namespace OS {

Log::Log(const std::string& log_name)
{
    file_.open("log.txt");
    if (!file_.is_open()) {
        throw std::runtime_error("failed to open log file " + log_name);
    }
}

Log::~Log()
{
    if (file_.is_open()) {
        file_.close();
    }
}

void Log::write_buffer()
{
    if (file_.is_open()) {
        file_ << buffer << std::endl;
        buffer.clear();
    } else {
        throw std::runtime_error("Log file isn't open");
    }
}

} // namespace OS
