#ifndef __HTTP_SERVER_HANDLER__
#define __HTTP_SERVER_HANDLER__

#include <restful_api/sale_api.h>

#include <boost/beast.hpp>
#include <boost/regex.hpp>
#include <regex>

namespace TN {

class HttpServerHandler {
public:
    using Ptr = std::unique_ptr<HttpServerHandler>;

public:
    HttpServerHandler();
    ~HttpServerHandler() = default;

    void processRequest(
            boost::beast::http::request<boost::beast::http::dynamic_body>&,
            boost::beast::http::response<boost::beast::http::dynamic_body>&);

private:
    boost::beast::http::verb routingRequest(boost::beast::http::request<boost::beast::http::dynamic_body> const&);

private:
    TN::SaleApi::Ptr saleApi_;
    boost::regex getReqPattern_;
    boost::regex putReqPattern_;
    boost::regex postReqPattern_;
    boost::regex delReqPattern_;
};

} // namespace TN

#endif // __HTTP_SERVER_HANDLER__