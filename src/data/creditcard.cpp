#include "creditcard.h"

namespace TN {

void from_json(nlohmann::json const& jsonCI, cardInfo& structCI)
{
    structCI.account = jsonCI.value("account", "");
    structCI.expiration = jsonCI.value("expiration", "");
    structCI.guid = jsonCI.value("guid", "");
    structCI.amount = jsonCI.value("amount", 0.0);
    structCI.tipAmount = jsonCI.value("tipAmount", 0.0);
    structCI.batchId = jsonCI.value("batchId", 0);
    structCI.transaction = jsonCI.value("transaction", 0);
}

void to_json(nlohmann::json& jsonCI, cardInfo const& structCI)
{
    jsonCI = nlohmann::json{
            {"account", structCI.account},
            {"expiration", structCI.expiration},
            {"guid", structCI.guid},
            {"amount", structCI.amount},
            {"tipAmount", structCI.tipAmount},
            {"batchId", structCI.batchId},
            {"transaction", structCI.transaction}};
}

} // namespace TN
