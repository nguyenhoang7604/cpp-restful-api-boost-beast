#ifndef __HTTP_CLIENT_HANDLER__
#define __HTTP_CLIENT_HANDLER__

#include <restful_api/sale_api.h>

#include <boost/beast.hpp>
#include <boost/regex.hpp>
#include <regex>

namespace TN {

// template <typename T>
class HttpClientHandler {
public:
    using Ptr = std::unique_ptr<HttpClientHandler>;

public:
    HttpClientHandler();
    ~HttpClientHandler() = default;

    void processRequest(
            boost::beast::http::request<boost::beast::http::dynamic_body>&,
            boost::beast::http::response<boost::beast::http::dynamic_body>&);

private:
private:
    TN::SaleApi::Ptr saleApi_;
};

} // namespace TN

#endif // __HTTP_CLIENT_HANDLER__