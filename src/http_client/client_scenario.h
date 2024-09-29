#ifndef __CLIENT_SCENARIO__
#define __CLIENT_SCENARIO__

#include "http_session.h"
#include <data/creditcard.h>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace TN {

class ClientScenario
{
public:
    ClientScenario(boost::asio::io_context& ioCtx, std::string const&, std::string const&);
    ~ClientScenario() = default;

    auto run() -> void;

protected:
    auto connectedListener() -> void;

private:
    auto createTransaction() -> cardInfo;
    auto modifyTransaction(cardInfo&) -> void;
    auto queryTransactions() -> std::vector<cardInfo>;
    auto removeTransaction(std::string const&) -> boost::beast::http::status;

private:
    HttpSession::Ptr httpSession_;
    boost::signals2::connection connectedSignal_;

private:
    static double const TipPercentage;
    static std::string const GetTarget;
    static std::string const PostTaget;
    static std::string const PutTarget;
    static std::string const DelTarget;
};

} // namespace TN

#endif // __CLIENT_SCENARIO__