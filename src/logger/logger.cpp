#include "logger.h"
#include "utils/utility.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <errno.h>

using namespace std;
using namespace TN;

std::mutex Logger::mutex_;
std::string const Logger::DefaultLogPath = "/tmp/";
std::string Logger::logFileName = "/tmp/messages.log";

//---------------------------------------------------------------------------
Logger* Logger::getInstance()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (!instance_) {
        // std::make_unique will not work
        instance_ = std::unique_ptr<Logger>(new Logger());
    }
    return instance_.get();
}

//---------------------------------------------------------------------------
void Logger::setLogLocation(string const& appName, string const& location)
{
    ostringstream logFileName;
    logFileName << location << "/" << appName << ".log";

    this->logFileName = logFileName.str();
}

//---------------------------------------------------------------------------
void Logger::info(string const& methodName, string const& msg)
{
    ofstream logFile;

    // Try to open file
    logFile.open(logFileName.c_str(), ios::out | ios::app);

    if (!logFile.is_open()) {
        ostringstream errorMessage;
        errorMessage << "Error opening: " << logFileName << " errno: " << errno;
        cout << errorMessage.str() << endl;
        return;
    }

    auto currentTime = Utillity::getTime();
    {
        std::lock_guard<std::mutex> lock(mutex_);
        logFile << currentTime << " PID:" << getpid() << " [" << methodName << "] " << msg << endl;
    }

    logFile.close();
}

//---------------------------------------------------------------------------
void Logger::info(string const& methodName, string const& msg, int i)
{
    ostringstream logString;
    logString << msg << i;
    info(methodName, logString.str());
}

//---------------------------------------------------------------------------
void Logger::info(string const& methodName, string const& msg, int i, string const& msg1)
{
    ostringstream logString;
    logString << msg << i << msg1;
    info(methodName, logString.str());
}

//---------------------------------------------------------------------------
void Logger::info(string const& methodName, string const& msg, string const& msg1)
{
    ostringstream logString;
    logString << msg << msg1;
    info(methodName, logString.str());
}

//---------------------------------------------------------------------------
void Logger::warn(string const& methodName, string const& msg)
{
    ostringstream logString;
    logString << "[WARN] " << msg;
    info(methodName, logString.str());
}

//---------------------------------------------------------------------------
void Logger::error(string const& methodName, string const& msg)
{
    ostringstream logString;
    logString << "[ERROR] " << msg;
    info(methodName, logString.str());
}
