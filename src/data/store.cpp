#include "store.h"
#include <logger/logger.h>
#include <iterator>
#include <optional>

using namespace TN;

std::mutex Store::mutex_;

Store* Store::getInstance()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (!instance_) {
        // std::make_unique will not work
        instance_ = std::unique_ptr<Store>(new Store());
    }
    return instance_.get();
}

//---------------------------------------------------------------------------
void Store::save(const std::string& guid, TN::cardInfo ci)
{

    std::lock_guard<std::mutex> lock(mutex_);

    transactions_.insert_or_assign(guid, ci);

    Logger::getInstance()->info(__func__, "Stored " + guid);
    Logger::getInstance()->info(__func__, "Total Count: " + countstr());
}

//---------------------------------------------------------------------------
auto Store::get(const std::string& guid) -> std::optional<TN::cardInfo>
{

    std::lock_guard<std::mutex> lock(mutex_);

    const auto t = transactions_.find(guid);
    if (t == transactions_.end()) { return std::nullopt; }

    Logger::getInstance()->info(__func__, "Retrieved " + guid);
    Logger::getInstance()->info(__func__, "Total Count: " + countstr());

    return {t->second};
}

//---------------------------------------------------------------------------
auto Store::list() -> std::vector<TN::cardInfo>
{
    std::vector<TN::cardInfo> l;
    l.reserve(count());

    std::lock_guard<std::mutex> lock(mutex_);
    std::transform(
            transactions_.begin(),
            transactions_.end(),
            std::back_insert_iterator(l),
            [](std::pair<std::string, TN::cardInfo> const& pair) { return pair.second; });

    return l;
}

//---------------------------------------------------------------------------
void Store::remove(const std::string& guid)
{
    //   const auto __func__ = logline(static_cast<const char *>(__func__));
    std::lock_guard<std::mutex> lock(mutex_);
    auto t = transactions_.find(guid);
    if (t != transactions_.end()) { transactions_.erase(t); }

    Logger::getInstance()->info(__func__, "Removed " + guid);
    Logger::getInstance()->info(__func__, "Total Count: " + countstr());
}
