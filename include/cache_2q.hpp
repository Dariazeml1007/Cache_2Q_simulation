#ifndef CACHE_BASE_HPP
#define CACHE_BASE_HPP

#include <cstddef>       // for size_t
#include <list>          // for std::list
#include <unordered_map> // for std::unordered_map
#include <stdexcept>     // for std::invalid_argument
#include <iterator>      // std::prev
#include <functional>    //
#include <utility>     // std::pair
#include <cassert>

namespace caches
{
template<typename KeyType, typename ValueType>

class Cache_2Q
{
private:

    enum class QueueType
    {
        NotCached = -1,
        InQueue   = 0,
        HotQueue  = 1,
        OutQueue  = 2
    };

    using EntryInfo = std::pair<typename std::list<std::pair<KeyType, ValueType>>::iterator,QueueType>;

    // Internal data structures
    std::list<std::pair<KeyType, ValueType>> a1in_queue_;  // FIFO queue
    std::list<std::pair<KeyType, ValueType>> am_queue_;    // LRU queue
    std::list<KeyType> a_out_queue_;

    std::unordered_map<KeyType, EntryInfo> cache_map_;
    std::unordered_map<KeyType, typename std::list<KeyType>::iterator> a_out_map_; // for meta data

    // Cache configuration
    size_t size_a1in_;
    size_t size_am_  ;
    size_t size_a_out_ ;

    using PageLoader = std::function<ValueType(const KeyType&)>;
    PageLoader slow_get_page_;

    void put(std::list<std::pair<KeyType, ValueType>>& queue, QueueType type, size_t max_size, const KeyType& key, const ValueType& val);
    void insert_into_Aout(const KeyType& key);

    void handle_hit(const KeyType& key, EntryInfo& info);
    void handle_miss_in_aout(const KeyType& key, const ValueType& value);
    void handle_cache_miss(const KeyType& key, const ValueType& value);


public:
    // Constructor
    Cache_2Q(size_t total_cache_size, PageLoader loader)
        : size_a1in_(total_cache_size / 2)
        , size_am_(total_cache_size / 2)
        , size_a_out_(total_cache_size) // it is only keys so we don't need to share memory
        , slow_get_page_(std::move(loader))
    {
        if (total_cache_size < 2)
        {
            throw std::invalid_argument("Cache size must be at least 2");
        }
    }

     //Rule of 5
     // 1.
    ~Cache_2Q() = default;

    // 2.
    Cache_2Q(const Cache_2Q&) = delete;

    // 3.
    Cache_2Q& operator=(const Cache_2Q&) = delete;

    // 4.
    Cache_2Q(Cache_2Q&&) = default;

    // 5.
    Cache_2Q& operator=(Cache_2Q&&) = default;


    std::pair<bool, ValueType> get(const KeyType& key);

    // Getters for statistics and for safety too !!!
    size_t size() const { return cache_map_.size(); }
    size_t max_size() const { return size_a1in_ + size_am_ + size_a_out_; }
};

template<typename KeyType, typename ValueType>
std::pair<bool, ValueType> Cache_2Q<KeyType, ValueType>::get(const KeyType& key)
{
    auto it = cache_map_.find(key);
    if (it != cache_map_.end())
    {
        ValueType value = it->second.first->second;
        handle_hit(key, it->second);
        return {true, value};  // Hit (return true and value)
    }

    if (a_out_map_.find(key) != a_out_map_.end())
    {
        ValueType value = slow_get_page_(key);
        handle_miss_in_aout(key, value);
        return {false, value};  // Miss (was in A_out)
    }

    ValueType value = slow_get_page_(key);
    handle_cache_miss(key, value);
    return {false, value};  // miss
}

template<typename KeyType, typename ValueType>
void Cache_2Q<KeyType, ValueType>::handle_hit(const KeyType& key, EntryInfo& info)
{
    assert(cache_map_.find(key) != cache_map_.end());
    assert(info.second == QueueType::InQueue || info.second == QueueType::HotQueue);

    if (info.second == QueueType::InQueue)
    {
        // Move from A1in to Am
        ValueType val = info.first->second;
        a1in_queue_.erase(info.first);
        put(am_queue_, QueueType::HotQueue, size_am_, key, val);
    }
    else
    {
        // Update LRU
        am_queue_.splice(am_queue_.end(), am_queue_, info.first);
    }
}

template<typename KeyType, typename ValueType>
void Cache_2Q<KeyType, ValueType>::handle_miss_in_aout(const KeyType& key, const ValueType& value)
{
    put(am_queue_, QueueType::HotQueue, size_am_, key, value);

    auto it_out = a_out_map_.find(key);
    a_out_queue_.erase(it_out->second);
    a_out_map_.erase(it_out);
}

template<typename KeyType, typename ValueType>
void Cache_2Q<KeyType, ValueType>::handle_cache_miss(const KeyType& key, const ValueType& value)
{
    put(a1in_queue_, QueueType::InQueue, size_a1in_, key, value);
}


template<typename KeyType, typename ValueType>
void Cache_2Q<KeyType, ValueType>::put(
    std::list<std::pair<KeyType, ValueType>>& queue,
    QueueType type,
    size_t max_size,
    const KeyType& key,
    const ValueType& val
)
{
    queue.emplace_back(key, val);

    cache_map_[key] = { std::prev(queue.end()), type };

    assert(cache_map_.find(key) != cache_map_.end());
    assert(cache_map_[key].first != queue.end());

    if (queue.size() > max_size)
    {

        KeyType victim_key = queue.front().first;

        // KEY should be in
        assert(cache_map_.find(victim_key) != cache_map_.end());

        if (type == QueueType::InQueue)
        {
            insert_into_Aout(queue.front().first);
        }
        cache_map_.erase(queue.front().first);
        queue.pop_front();
    }
}

template<typename KeyType, typename ValueType>
void Cache_2Q<KeyType, ValueType>::insert_into_Aout(const KeyType& key)
{
    assert(a_out_map_.find(key) == a_out_map_.end());

    a_out_queue_.emplace_back(key);
    a_out_map_[key] = std::prev(a_out_queue_.end());

    if (a_out_queue_.size() > size_a_out_)
    {
        KeyType victim_key = a_out_queue_.front();

        assert(a_out_map_.find(victim_key) != a_out_map_.end());

        a_out_map_.erase(a_out_queue_.front());
        a_out_queue_.pop_front();
    }
}
} // namespace caches

#endif // CACHE_2Q_HPP
