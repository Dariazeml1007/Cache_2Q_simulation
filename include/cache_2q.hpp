#ifndef CACHE_BASE_HPP
#define CACHE_BASE_HPP

#include <cstddef>       // for size_t
#include <list>          // for std::list
#include <unordered_map> // for std::unordered_map
#include <stdexcept>     // for std::invalid_argument
#include <iterator>      // std::prev
#include <functional>    //

template<typename KeyType, typename ValueType>

class CacheBase
{
private:
    struct CacheEntry
    {
        KeyType key;
        ValueType value;
    };

    enum QueueType
    {
        NotCached = -1,
        InQueue   = 0,
        HotQueue  = 1,
        OutQueue  = 2
    };

    struct EntryInfo
    {
        typename std::list<CacheEntry>::iterator iter;
        QueueType queue_type;
    };

    // Internal data structures
    std::list<CacheEntry> a1in_queue_;  // FIFO queue
    std::list<CacheEntry> am_queue_   ; // LRU queue
    std::list<KeyType> a_out_queue_;

    std::unordered_map<KeyType, EntryInfo> cache_map_;
    std::unordered_map<KeyType, typename std::list<KeyType>::iterator> a_out_map_; // для метаданных

    // Cache configuration
    size_t size_a1in_;
    size_t size_am_  ;
    size_t size_a_out_ ;

    using PageLoader = std::function<ValueType(const KeyType&)>;
    PageLoader slow_get_page_;

    void put(std::list<CacheEntry>& queue, QueueType type, size_t max_size, const KeyType& key, const ValueType& val);
    void insert_into_Aout(const KeyType& key);


public:
    // Constructor
    CacheBase(size_t total_cache_size, PageLoader loader)
        : size_a1in_(total_cache_size / 2)
        , size_am_(total_cache_size / 2)
        , size_a_out_(total_cache_size)
        , slow_get_page_(std::move(loader))
    {
        if (total_cache_size < 2)
        {
            throw std::invalid_argument("Cache size must be at least 3");
        }
    }

    bool get(const KeyType& key);
    // Getters for statistics and for safety too !!!
    size_t size() const { return cache_map_.size(); }
    size_t max_size() const { return size_a1in_ + size_am_ + size_a_out_; }
};

// Implementation of template methods must be in the header file
template<typename KeyType, typename ValueType>
bool CacheBase<KeyType, ValueType>::get(const KeyType& key)
{
    auto it = cache_map_.find(key);
    if (it != cache_map_.end())
    {
        // Get metadata about the element's location
        EntryInfo& info = it->second;  // Reference avoids copying

        if (info.queue_type == InQueue)
        {
            // Move from A1in to Am (promotion to hot queue)
            ValueType val = info.iter->value;
            a1in_queue_.erase(info.iter);
            put(am_queue_, HotQueue, size_am_, key, val);
        }
        else
        {
            // Element is already in Am -> update LRU: move to end
            am_queue_.splice(am_queue_.end(), am_queue_, info.iter);
            // splice moves without copying or memory allocation
        }
        return true;
    }

    auto it_out = a_out_map_.find(key);

    if (it_out != a_out_map_.end())
    {
        ValueType value = slow_get_page_(key);
        put(am_queue_, HotQueue, size_am_, key, value);

        // Удаляем из A_out
        a_out_queue_.erase(it_out->second);
        a_out_map_.erase(it_out);


        return false;
    }


    // Key not found - insert as new
    ValueType val = slow_get_page_(key);
    put(a1in_queue_, InQueue, size_a1in_, key, val);
    return false;

}

template<typename KeyType, typename ValueType>
void CacheBase<KeyType, ValueType>::put(std::list<CacheEntry>& queue, QueueType type, size_t max_size, const KeyType& key, const ValueType& val)
{
    // Create and add entry to the end of куеуе
    queue.push_back({key, val});  // aggregate initialization

    cache_map_[key] = {std::prev(queue.end()), type};;  // Automatically creates or updates entry

    // Check if A1in queue overflow - evict from front (FIFO)
    if (queue.size() > max_size)
    {
        if (type == InQueue)
        {
            insert_into_Aout(queue.front().key);
        }
        cache_map_.erase(queue.front().key); //first deleted entry
        queue.pop_front(); //second deleted front elem
    }

}

template<typename KeyType, typename ValueType>
void CacheBase<KeyType, ValueType>::insert_into_Aout(const KeyType& key)
{
    a_out_queue_.push_back(key);
    a_out_map_[key] = std::prev(a_out_queue_.end());

    if (a_out_queue_.size() > size_a_out_)
    {
        a_out_map_.erase(a_out_queue_.front());
        a_out_queue_.pop_front();
    }
}
#endif // CACHE_BASE_HPP
