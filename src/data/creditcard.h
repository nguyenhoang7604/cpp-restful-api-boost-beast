#ifndef __CREDITCARD__
#define __CREDITCARD__

#include <thirdparty/json.hpp>
#include <string>

namespace TN {

struct cardInfo {
    std::string account{};
    std::string expiration{};
    double amount{0.0};
    double tipAmount{0.0};
    uint batchId{0};
    uint transaction{0};
    std::string guid{};
};

void from_json(nlohmann::json const&, cardInfo&);
void to_json(nlohmann::json&, cardInfo const&);

} // namespace TN

#endif // __CREDITCARD__