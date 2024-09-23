#include "http_server_handler.h"
#include <logger/logger.h>

#include <iostream>
#include <string>

using namespace TN;
using namespace boost::beast;

HttpServerHandler::HttpServerHandler()
    : saleApi_(std::make_shared<SaleApi>())
    , getReqPattern_("/sale", boost::regex_constants::optimize)
    , putReqPattern_(R"(^/sale/inc/(\w+)$)", boost::regex_constants::optimize)
    , postReqPattern_("/sale", boost::regex_constants::optimize)
    , delReqPattern_(R"(^/sale/reverse/(\w+)$)", boost::regex_constants::optimize)
{
}

// Determine what needs to be done with the request message.
void HttpServerHandler::processRequest(
        http::request<http::dynamic_body>& request,
        http::response<http::dynamic_body>& response)
{
    response.version(request.version());
    response.keep_alive(request.keep_alive());

    // Do routing request base on request target
    auto reqMethod = routingRequest(request);
    switch (reqMethod) {
    case http::verb::get:
        response.set(http::field::server, "Beast");
        saleApi_->list(request, response);
        break;

    case http::verb::post:
        response.set(http::field::server, "Beast");
        saleApi_->auth(request, response);
        break;

    case http::verb::put:
        response.set(http::field::server, "Beast");
        saleApi_->inc(request, response);
        break;

    case http::verb::delete_:
        response.set(http::field::server, "Beast");
        saleApi_->reverse(request, response);
        break;

    default:
        // We return responses indicating an error if
        // we do not recognize the request method.
        response.result(http::status::bad_request);
        response.set(http::field::content_type, "text/plain");
        ostream(response.body()) << "Invalid request-method '" << std::string(request.method_string()) << "'";
        break;
    }
}

http::verb HttpServerHandler::routingRequest(http::request<http::dynamic_body> const& req)
{
    if (req.method() == http::verb::get && boost::regex_match(req.target().data(), getReqPattern_)) {
        // Handle GET request
        return req.method();
    } else if (req.method() == http::verb::post && boost::regex_match(req.target().data(), postReqPattern_)) {
        // Handle POST request
        return req.method();
    } else if (req.method() == http::verb::put && boost::regex_match(req.target().data(), putReqPattern_)) {
        // Handle PUT request
        return req.method();
    } else if (req.method() == http::verb::delete_ && boost::regex_match(req.target().data(), delReqPattern_)) {
        // Handle DELETE request
        return req.method();
    }

    // Default request Type is unknown
    return http::verb::unknown;
}
