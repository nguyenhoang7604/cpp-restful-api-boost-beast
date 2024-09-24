# cpp_restfu_boost_beast
cpp_restfu_boost_beast is a tiny http library based on [`Boost.Beast`](https://www.boost.org/doc/libs/1_71_0/libs/beast/doc/html/index.html). 

`Boost.Beast` is based on [`Boost.Asio`](https://www.boost.org/doc/libs/1_71_0/doc/html/boost_asio.html) which provides low level asynchronous I/O.  cpp_restfu_boost_beast was originally created to add HTTP functionality and some rest api to an existing application that is using `Boost.Asio`.

## Features
- Http server and client side
- Synchronous or Asynchronous API
- Logger support
- Regular expression based url routing
- Timeout support
- Timers and signals support included
- Customizable thread pool size

## Source code structure

├── build.sh
├── cmake
│   └── Project.cmake
├── CMakeLists.txt
├── include
│   └── thirdparty
├── src
│   ├── CMakeLists.txt
│   ├── data
│   ├── http_client
│   ├── http_handler
│   ├── http_server
│   ├── logger
│   ├── restful_api
│   └── utils
└── tests
    └── CMakeLists.txt

## Build

cpp_restful_boost_beast depends Boost.Beast and nlohmann/json library.

### Linux

To build the examples, you can use the following command:

```shell
git clone https://github.com/nguyenhoang7604/cpp_restful_boost_beast.git
cd cpp_restful_boost_beast/
./build.sh
```

To run the examples, you can use the following command:
```shell
### Run http server on 127.0.0.1::8080 
.\build\sources\build\usr\bin\http_server &

### Run http client to connect to 127.0.0.1::8080 
.\build\sources\build\usr\bin\http_client &
```

### Windows
