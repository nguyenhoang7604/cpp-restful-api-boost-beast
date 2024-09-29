#include "utility.h"

#include <algorithm>
#include <climits>
#include <functional>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

using namespace TN;

unsigned char Utillity::randomChar()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    return static_cast<unsigned char>(dis(gen));
}

std::string Utillity::generateHex(const unsigned int len)
{
    std::stringstream ss;
    for (uint i = 0; i < len; i++) {
        auto rc = randomChar();
        std::stringstream hexstream;
        hexstream << std::hex << int(rc);
        auto hex = hexstream.str();
        ss << (hex.length() < 2 ? '0' + hex : hex);
    }
    return ss.str();
}

std::string Utillity::generateGuid()
{
    return generateHex(14);
}

//---------------------------------------------------------------------------
std::string Utillity::getTime()
{
    char dateTime[128];
    struct tm* newtime;
    long ltime;

    time(&ltime);
    newtime = gmtime(&ltime);
    strftime(dateTime, 128, "%a, %d %b %Y %H:%M:%S GMT", newtime);
    return dateTime;
}
