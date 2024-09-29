#ifndef __HTTP_SESSION__
#define __HTTP_SESSION__

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/signals2.hpp>

namespace TN {

class HttpSession : public std::enable_shared_from_this<HttpSession> {
public:
    using Ptr = std::shared_ptr<HttpSession>;
    using AsyncHandler = boost::function<void(boost::beast::error_code, std::size_t)>;

public:
    // Objects are constructed with a strand to
    // ensure that handlers do not execute concurrently.
    explicit HttpSession(boost::asio::io_context& ioc);
    ~HttpSession() = default;

    // Start the asynchronous operation
    void run(std::string const& host, std::string const& port);

    void onResolve(boost::beast::error_code, boost::asio::ip::tcp::resolver::results_type);

    void onConnect(boost::beast::error_code, boost::asio::ip::tcp::resolver::results_type::endpoint_type);

    size_t doSend(boost::beast::http::request<boost::beast::http::string_body> const&, boost::beast::error_code&);
    void doSendAsync(boost::beast::http::request<boost::beast::http::string_body> const&, AsyncHandler const&);

    size_t doRead(boost::beast::http::response<boost::beast::http::string_body>&, boost::beast::error_code&);
    void doReadAsync(boost::beast::http::response<boost::beast::http::string_body>&, AsyncHandler const&);

    /// connect Listener with connected event
    boost::signals2::connection connectConnectedNotification(boost::function<void()> const&);

protected:
    void close();

private:
    boost::asio::ip::tcp::resolver resolver_;
    boost::beast::tcp_stream stream_;
    boost::beast::flat_buffer buffer_;

    boost::signals2::signal<void()> connectedSignal_;
};

inline boost::signals2::connection HttpSession::connectConnectedNotification(boost::function<void()> const& cb)
{
    return connectedSignal_.connect(cb);
}

} // namespace TN

#endif // __HTTP_SESSION__