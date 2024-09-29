#include <http_handler/http_server.h>
#include <logger/logger.h>

#include <iostream>
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
              << "    http_server www.example.com 80 /\n"
              << "    http_server 0.0.0.0 8080 / 1.0\n";
    std::cout << std::endl;
    return ret;
}

void runTreadPoolIoContext(size_t nThreads, boost::asio::io_context& ioCtx)
{
    std::vector<std::thread> threadPool;
    for (size_t t = 0; t < nThreads; ++t) {
        threadPool.emplace_back([&ioCtx, t]() { ioCtx.run(); });
    }
    for (auto& thread : threadPool) {
        try {
            if (thread.joinable()) thread.join();
        } catch (std::exception& e) {
            Logger::getInstance()->error(__func__, e.what());
        }
    }
}
} // namespace

//---------------------------------------------------------------------------
auto main(int argc, char* argv[]) -> int
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
    std::string host = (argc - optind >= 1) ? argv[optind++] : "127.0.0.1";
    std::string port = (argc - optind >= 1) ? argv[optind++] : "8080";

    Logger::getInstance()->setLogLocation(appName);

    Logger::getInstance()->info(__func__, "Starting http server");

    auto httpServer = new HttpServer(ioCtx, host, port);
    httpServer->start();

    runTreadPoolIoContext(std::thread::hardware_concurrency(), ioCtx);
    return 0;
}