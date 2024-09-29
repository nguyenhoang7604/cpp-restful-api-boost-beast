#include "client_scenario.h"
#include <logger/logger.h>
#include <thirdparty/json.hpp>
#include <string_view>

using namespace TN;
using json = nlohmann::json;

namespace beast = boost::beast;
namespace http = beast::http;

namespace Config
{
    std::string Host;
    std::string Port;
    std::string_view constexpr ContenType = "application/json";
    uint32_t constexpr Version = 11;
}

double const ClientScenario::TipPercentage = .1;
std::string const ClientScenario::GetTarget = "/sale";
std::string const ClientScenario::PostTaget = "/sale";
std::string const ClientScenario::PutTarget = "/sale/inc/";
std::string const ClientScenario::DelTarget = "/sale/reverse/";

ClientScenario::ClientScenario(boost::asio::io_context& ioCtx, std::string const& host, std::string const& port)
    : httpSession_(std::make_shared<HttpSession>(ioCtx))
    , connectedSignal_(httpSession_->connectConnectedNotification(std::bind(&ClientScenario::connectedListener, this)))
{
    Config::Host = host;
    Config::Port = port;
}

void ClientScenario::run()
{
    httpSession_->run(Config::Host, Config::Port);
}


void ClientScenario::connectedListener()
{
    uint idleTime = 1;

    for (uint32_t i = 0; i < 5; ++i) {
        auto c2 = createTransaction();
        modifyTransaction(c2);
        sleep(idleTime);
    }

    auto transactions = queryTransactions();
    for (auto transaction : transactions) {
        removeTransaction(transaction.guid);
        sleep(idleTime);
    }
}

//---------------------------------------------------------------------------
auto ClientScenario::createTransaction() -> cardInfo
{
    cardInfo ci{"4111111111111111", "0524", 125.00, 0, 1, 1, ""};
    json payload = ci;

    http::request<http::string_body> req {http::verb::post, PostTaget, Config::Version};
    req.set(http::field::host, Config::Host);
    req.set(http::field::content_type, Config::ContenType);
    req.body() = payload.dump();
    req.set(http::field::content_length, std::to_string(req.body().size()));

    boost::beast::error_code ec;
    httpSession_->doSend(req, ec);

    http::response<http::string_body> res;
    httpSession_->doRead(res, ec);

    cardInfo rci = json::parse((res.body().data()))[0];

    return rci;
}

// //---------------------------------------------------------------------------
auto ClientScenario::modifyTransaction(cardInfo& ci) -> void
{
    ci.tipAmount = ci.amount * TipPercentage;
    ci.amount += ci.tipAmount;
    json payload = ci;

    std::stringstream putTarget{};
    putTarget << PutTarget << ci.guid;
    http::request<http::string_body> req{http::verb::put, putTarget.str(), Config::Version};

    req.set(http::field::host, Config::Host);
    req.set(http::field::content_type, Config::ContenType);
    req.body() = payload.dump();
    req.set(http::field::content_length, std::to_string(req.body().size()));

    // Send the HTTP request to the remote host
    boost::beast::error_code ec;
    http::response<http::string_body> res;
    httpSession_->doSend(req, ec);
    httpSession_->doRead(res, ec);
    Logger::getInstance()->info(__func__, res.body().data());

    cardInfo rci = json::parse((res.body().data()));
}

//---------------------------------------------------------------------------
auto ClientScenario::queryTransactions() -> std::vector<cardInfo>
{
    http::request<http::string_body> req{http::verb::get, GetTarget, Config::Version};

    req.set(http::field::host, Config::Host);
    req.set(http::field::content_type, Config::ContenType);

    boost::beast::error_code ec;
    http::response<http::string_body> res;
    httpSession_->doSend(req, ec);
    httpSession_->doRead(res, ec);

    Logger::getInstance()->info(__func__, res.body().data());

    auto trans = json::parse((res.body().data()));

    return trans;
}

//---------------------------------------------------------------------------
auto ClientScenario::removeTransaction(std::string const& guid) -> boost::beast::http::status
{
    std::stringstream delTarget{};
    delTarget << DelTarget << guid;
    http::request<http::string_body> req{http::verb::delete_, delTarget.str(), Config::Version};

    req.set(http::field::host, Config::Host);
    req.set(http::field::content_type, Config::ContenType);

    boost::beast::error_code ec;
    http::response<http::string_body> res;
    httpSession_->doSend(req, ec);
    httpSession_->doRead(res, ec);

    Logger::getInstance()->info(__func__,"DELETE/DELETE - Status: ", res.reason());

    return res.result();
}