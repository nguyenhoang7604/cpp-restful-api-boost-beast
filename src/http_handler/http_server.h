#ifndef __HTTP_SERVER__
#define __HTTP_SERVER__

#include "http_connection.h"

#include "map"
class HttpServer {
public:
    using HttpConnectionMapType = std::map<uint32_t, TN::HttpConnection::Ptr>;

public:
    HttpServer(boost::asio::io_context& ioCtx, std::string const&, std::string const&);
    ~HttpServer() = default;

    void start();

    /// Closes and disconnects all connections
    void close();

    /// Retrieves a connection
    TN::HttpConnection::Ptr get(uint32_t const connectionId);

    /// Iterates the connection list
    typename HttpConnectionMapType::const_iterator begin();
    typename HttpConnectionMapType::const_iterator end();

    /// Returns true if no connections exist
    bool empty() const;

protected:
    /// Listens to connections being made
    void doAccept();

private:
    boost::asio::io_context& ioCtx_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::string address_;
    std::string port_;
    mutable std::mutex mutex_;
    HttpConnectionMapType connections_;

private:
    static uint32_t ConnectionId;
};

#endif // __HTTP_SERVER__