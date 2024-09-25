#pragma once

#include "creditcard.h"
#include <thirdparty/json.hpp>

#include <iostream>
#include <map>
#include <mutex>
#include <string>

namespace TN {

// ---------------------------------------------------------------------------
class Store {
public:
    Store(Store const&) = delete;
    Store& operator=(Store const&) = delete;
    ~Store()
    {
    }

    static Store* getInstance();
    void save(const std::string& guid, TN::cardInfo ci);
    auto get(const std::string& guid) -> std::optional<TN::cardInfo>;
    auto list() -> std::vector<TN::cardInfo>;
    void remove(const std::string& guid);
    auto count() -> size_t
    {
        return transactions_.size();
    }
    auto countstr() -> std::string
    {
        std::stringstream ss;
        ss << count();
        return ss.str();
    }

private:
    inline static std::unique_ptr<Store> instance_{nullptr};
    explicit Store(){};

    static std::mutex mutex_;
    std::map<std::string, TN::cardInfo> transactions_;
};

} // namespace TN