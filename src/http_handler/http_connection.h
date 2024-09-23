#ifndef __HTTP_CONNECTION__
#define __HTTP_CONNECTION__

#include "http_server_handler.h"

#include <boost/asio.hpp>
#include <boost/asio/basic_socket_acceptor.hpp>
#include <boost/asio/basic_stream_socket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

namespace TN {

class HttpConnection : public std::enable_shared_from_this<HttpConnection> {
public:
    using Ptr = std::shared_ptr<HttpConnection>;

public:
    HttpConnection(boost::asio::ip::tcp::socket, uint32_t);
    ~HttpConnection() = default;

    // Initiate the asynchronous operations associated with the connection.
    void onConnect();
    void close();

private:
    // Asynchronously receive a complete request message.
    void readRequest();

    // Determine what needs to be done with the request message.
    void doRead(boost::beast::error_code, std::size_t);

    // Construct a response message based on the program state.
    void doWrite(boost::beast::error_code, std::size_t);

    // Asynchronously transmit the response message.
    void writeResponse();

    // Check whether we have spent enough time on this connection.
    void checkDeadline();

private:
    // The socket for the currently connected client.
    boost::asio::ip::tcp::socket socket_;
    uint32_t connectionId_;

    // The buffer for performing reads.
    boost::beast::flat_buffer buffer_{8192};

    // The request message.
    boost::beast::http::request<boost::beast::http::dynamic_body> request_;

    // The response message.
    boost::beast::http::response<boost::beast::http::dynamic_body> response_;

    TN::HttpServerHandler::Ptr httpHandler_;

    // The timer for putting a deadline on connection processing.
    boost::asio::steady_timer deadline_;
};

} // namespace TN

#endif // __HTTP_CONNECTION__