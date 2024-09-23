#include "http_connection.h"
#include <logger/logger.h>
#include <iostream>

using namespace TN;
using namespace boost::asio;
using namespace boost::beast;

HttpConnection::HttpConnection(ip::tcp::socket socket, uint32_t connectionId)
    : socket_(std::move(socket))
    , connectionId_(connectionId)
    , httpHandler_(std::make_unique<HttpServerHandler>())
    , deadline_(socket_.get_executor(), std::chrono::seconds(60))
{
}

void HttpConnection::onConnect()
{
    readRequest();
    checkDeadline();
}

void HttpConnection::close()
{
    deadline_.cancel();
    socket_.close();
    /// Todo - single close event to HttpServer
}

void HttpConnection::readRequest()
{
    auto self = shared_from_this();

    buffer_.clear();
    request_ = http::request<http::dynamic_body>();
    response_ = http::response<http::dynamic_body>();

    http::async_read(
            socket_,
            buffer_,
            request_,
            std::bind(&HttpConnection::doRead, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

// Asynchronously transmit the response message.
void HttpConnection::doRead(error_code ec, std::size_t bytes_transferred)
{
    if (ec || bytes_transferred == 0) {
        Logger::getInstance()->error(__func__, "Closing connection - " + ec.message());
        close();
        return;
    }

    if (!httpHandler_) {
        Logger::getInstance()->error(__func__, "Invalid http handler.");
        close();
        return;
    }

    httpHandler_->processRequest(request_, response_);
    writeResponse();
}

// Asynchronously transmit the response message.
void HttpConnection::writeResponse()
{
    auto self = shared_from_this();

    response_.content_length(response_.body().size());

    http::async_write(socket_, response_, [self](error_code ec, std::size_t) {
        if (ec || !self->request_.keep_alive()) {
            self->socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
            self->deadline_.cancel();
            return;
        }

        self->readRequest();
    });
}

void HttpConnection::doWrite(error_code, std::size_t)
{
    // Todo
}

// Check whether we have spent enough time on this connection.
void HttpConnection::checkDeadline()
{
    auto self = shared_from_this();

    deadline_.async_wait([self](error_code ec) {
        if (!ec) {
            // Close socket to cancel any outstanding operation.
            self->socket_.close(ec);
        }
    });
}