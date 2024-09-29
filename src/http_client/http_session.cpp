#include "http_session.h"
#include <logger/logger.h>
#include <iostream>

using namespace TN;
namespace beast = boost::beast;   // from <boost/beast.hpp>
namespace http = beast::http;     // from <boost/beast/http.hpp>
namespace net = boost::asio;      // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>

// Objects are constructed with a strand to
// ensure that handlers do not execute concurrently.
HttpSession::HttpSession(net::io_context& ioc)
    : resolver_(net::make_strand(ioc))
    , stream_(net::make_strand(ioc))
{
}

// Look up the domain name
void HttpSession::run(std::string const& host, std::string const& port)
{
    resolver_.async_resolve(host, port, beast::bind_front_handler(&HttpSession::onResolve, shared_from_this()));
}

void HttpSession::close()
{
    stream_.socket().close();
}

void HttpSession::onResolve(boost::beast::error_code ec, boost::asio::ip::tcp::resolver::results_type results)
{
    if (ec) Logger::getInstance()->error(__func__, "Closing connection");

    // Set a timeout on the operation
    stream_.expires_after(std::chrono::seconds(30));

    // Make the connection on the IP address we get from a lookup
    stream_.async_connect(results, beast::bind_front_handler(&HttpSession::onConnect, shared_from_this()));
}

void HttpSession::onConnect(boost::beast::error_code ec, tcp::resolver::results_type::endpoint_type)
{
    if (ec) Logger::getInstance()->error(__func__, "Closing connection");

    // Set a timeout on the operation
    stream_.expires_after(std::chrono::seconds(30));

    connectedSignal_();
}

size_t HttpSession::doSend(http::request<http::string_body> const& req, boost::beast::error_code& ec)
{
    auto bytesTransferred = http::write(stream_, req, ec);
    if (ec || bytesTransferred == 0) {
        Logger::getInstance()->error(__func__, "Closing connection");
        close();
    }
    return bytesTransferred;
}

void HttpSession::doSendAsync(http::request<http::string_body> const& req, AsyncHandler const& cb)
{
    http::async_write(stream_, req, cb);
}

size_t HttpSession::doRead(http::response<http::string_body>& res, boost::beast::error_code& ec)
{
    buffer_.clear();
    auto bytesTransferred = http::read(stream_, buffer_, res, ec);

    if (ec || bytesTransferred == 0) {
        Logger::getInstance()->error(__func__, "Closing connection");
        close();
    }
    return bytesTransferred;
}

void HttpSession::doReadAsync(http::response<http::string_body>& res, AsyncHandler const& cb)
{
    buffer_.clear();
    http::async_read(stream_, buffer_, res, cb);
}
