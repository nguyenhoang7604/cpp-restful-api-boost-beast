#ifndef __LOGGER__
#define __LOGGER__

#include <algorithm>
#include <fstream>
#include <iterator>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <unistd.h>

// bool const IS_MAIN = true;
// bool const IS_THREAD = false;

namespace TN {

class Logger {
public:
    Logger(Logger const&) = delete;
    Logger& operator=(Logger const&) = delete;
    ~Logger()
    {
    }

    static Logger* getInstance();

    void setLogLocation(std::string const& appName, std::string const& location = DefaultLogPath);

    // Log write routines
    void info(std::string const& methodName, std::string const&);
    void info(std::string const& methodName, std::string const&, int i);
    void info(std::string const& methodName, std::string const&, int i, std::string const&);
    void info(std::string const& methodName, std::string const&, std::string const&);

    void warn(std::string const& methodName, std::string const&);
    void error(std::string const& methodName, std::string const&);

private:
    explicit Logger(){};

private:
    inline static std::unique_ptr<Logger> instance_{nullptr};
    static std::mutex mutex_;

    static std::string logFileName;
    static std::string const DefaultLogPath;
};

} // namespace TN

#endif // __LOGGER__
