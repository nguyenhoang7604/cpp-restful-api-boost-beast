#include "sale_api.h"

#include <data/creditcard.h>
#include <data/store.h>
#include <logger/logger.h>
#include <thirdparty/json.hpp>
#include <utils/utility.h>
#include <iostream>
#include <string>

using namespace TN;
using namespace boost::beast::http;
using json = nlohmann::json;

std::string const SaleApi::ContentType = "application/json";

SaleApi::SaleApi()
{
}

//---------------------------------------------------------------------------
void SaleApi::auth(request<dynamic_body> const& req, response<dynamic_body>& res)
{

    cardInfo ci = json::parse(buffers_to_string(req.body().data()));

    ci.guid = Utillity::generateGuid();
    Store::getInstance()->save(ci.guid, ci);

    res.result(status::ok);

    json result{ci};

    res.set(field::content_type, ContentType);
    boost::beast::ostream(res.body()) << result.dump();
};

//---------------------------------------------------------------------------
void SaleApi::inc(request<dynamic_body> const& req, response<dynamic_body>& res)
{

    auto const slash = req.target().find_last_of("/");
    auto guid = req.target().substr(slash + 1);

    cardInfo payloadCI = json::parse(buffers_to_string(req.body().data()));
    auto storeCI = Store::getInstance()->get(guid);

    res.set(field::content_type, ContentType);
    if (storeCI.has_value()) {
        cardInfo nci = storeCI.value();
        if (payloadCI.amount != (payloadCI.tipAmount + storeCI->amount)) {
            Logger::getInstance()->warn(__func__, "New amount with tip does not equal differs.");
        }
        nci.tipAmount = payloadCI.tipAmount;
        nci.amount = payloadCI.amount;

        Store::getInstance()->save(storeCI->guid, nci);
        Logger::getInstance()->info(__func__, "Incremented Sale");

        res.result(status::ok);
        json result = nci;
        boost::beast::ostream(res.body()) << result.dump();
    } else {
        Logger::getInstance()->warn(__func__, "Transaction not found.");
        res.result(status::internal_server_error);
    }
};

//---------------------------------------------------------------------------
void SaleApi::list(request<dynamic_body> const&, response<dynamic_body>& res)
{
    json result{};

    result = Store::getInstance()->list();

    res.result(status::ok);

    res.set(field::content_type, ContentType);
    boost::beast::ostream(res.body()) << result.dump();
}

//---------------------------------------------------------------------------
void SaleApi::reverse(request<dynamic_body> const& req, response<dynamic_body>& res)
{
    auto const slash = req.target().find_last_of("/");
    auto guid = req.target().substr(slash + 1);

    auto storeCI = Store::getInstance()->get(guid);

    res.set(field::content_type, ContentType);
    if (storeCI.has_value()) {
        Store::getInstance()->remove(guid);

        Logger::getInstance()->info(__func__, "Reversed/Removed Sale");
        res.result(status::ok);
        json result = storeCI.value();
        boost::beast::ostream(res.body()) << result.dump();
    } else {
        Logger::getInstance()->info(__func__, "Transaction not found to reverse.");
        res.result(status::internal_server_error);
    }
};
