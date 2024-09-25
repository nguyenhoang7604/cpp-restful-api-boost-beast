#ifndef __SALE_API__
#define __SALE_API__

#include <boost/beast.hpp>

namespace TN {

class SaleApi : public std::enable_shared_from_this<SaleApi> {
public:
    using Ptr = std::shared_ptr<SaleApi>;

public:
    SaleApi();
    ~SaleApi() = default;

    void auth(
            boost::beast::http::request<boost::beast::http::dynamic_body> const&,
            boost::beast::http::response<boost::beast::http::dynamic_body>&);
    void inc(
            boost::beast::http::request<boost::beast::http::dynamic_body> const&,
            boost::beast::http::response<boost::beast::http::dynamic_body>&);
    void list(
            boost::beast::http::request<boost::beast::http::dynamic_body> const&,
            boost::beast::http::response<boost::beast::http::dynamic_body>&);
    void reverse(
            boost::beast::http::request<boost::beast::http::dynamic_body> const&,
            boost::beast::http::response<boost::beast::http::dynamic_body>&);

private:
    static std::string const ContentType;
};

} // namespace TN

#endif // __SALE_API__