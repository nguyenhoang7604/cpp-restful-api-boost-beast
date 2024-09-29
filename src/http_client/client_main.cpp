#include "client_scenario.h"
#include <logger/logger.h>

#include <iostream>
#include <string>
#include <cstdlib>
#include <getopt.h>


using namespace TN;

namespace {

struct option long_options[] = {{"help", no_argument, 0, 'h'}, {0, 0, 0, 0}};

auto usage(std::string const& appName, int const ret) -> int
{
    std::cout << "Usage: " << appName << " [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h, --help            display this message and exit" << std::endl;
    std::cout << std::endl;
    std::cout << "Example:\n"
              << "    http_server www.example.com 80\n";
    std::cout << std::endl;
    return ret;
}
} // namespace

int main(int argc, char** argv)
{

    boost::asio::io_context ioCtx;

    char const* appName = [&argv]() {
        auto p = strrchr(argv[0], '/');
        return (p ? ++p : argv[0]);
    }();

    int c = 0, option_index = 0;
    while ((c = getopt_long(argc, argv, "h", long_options, &option_index)) != -1) {
        switch (c) {
        case 'h': return usage(appName, 0);
        default: return usage(appName, 1);
        }
    }

    std::string const host = (argc - optind >= 1) ? argv[optind++] : "0.0.0.0";
    std::string const port = (argc - optind >= 1) ? argv[optind++] : "8080";

    Logger::getInstance()->setLogLocation(appName);
    Logger::getInstance()->info(__func__, "Starting http client");

    ClientScenario scenario(ioCtx, host, port);
    scenario.run();
    ioCtx.run();
    return 0;
}
