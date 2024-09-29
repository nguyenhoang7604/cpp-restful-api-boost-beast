#ifndef __UTILLITY_
#define __UTILLITY_

#include <string>

namespace TN {

class Utillity {
public:
    static unsigned char randomChar();
    static std::string generateHex(const unsigned int len);
    static std::string generateGuid();
    static std::string getTime();
};

} // namespace TN

#endif // __UTILLITY_