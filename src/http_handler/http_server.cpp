#include "http_server.h"

#include <logger/logger.h>
#include <iostream>
#include <string>

using namespace TN;
using namespace boost::asio;

uint32_t HttpServer::ConnectionId = 0;
HttpServer::HttpServer(io_context& ioCtx, std::string const& addr, std::string const& port)
    : ioCtx_(ioCtx)
    , acceptor_(ioCtx)
    , address_(addr)
    , port_(port)
{
}

void HttpServer::close()
{
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto connection : connections_) connection.second->close();
    connections_.clear();
}

bool HttpServer::empty() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return connections_.empty();
}

void HttpServer::start()
{
    ip::tcp::resolver resolver(ioCtx_);
    ip::tcp::resolver::query query((address_.empty() ? "0.0.0.0" : address_), (port_.empty() ? "8080" : port_));
    ip::tcp::endpoint endpoint = *resolver.resolve(query);

    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    doAccept();
}

void HttpServer::doAccept()
{
    acceptor_.async_accept([this](boost::beast::error_code ec, ip::tcp::socket socket) {
        if (ec) {
            Logger::getInstance()->info(__func__, ec.message());
            return;
        }

        ++ConnectionId;
        auto connection = std::make_shared<HttpConnection>(std::move(socket), ConnectionId);
        connection->onConnect();
        {
            std::lock_guard<std::mutex> lock(mutex_);

            Logger::getInstance()->info(__func__, "Connection created: " + std::to_string(ConnectionId));
            connections_.emplace(ConnectionId, std::move(connection));
        }
        doAccept();
    });
}