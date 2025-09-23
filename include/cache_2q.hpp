#ifndef CACHE_BASE_HPP
#define CACHE_BASE_HPP

#include <cstddef>       // for size_t
#include <list>          // for std::list
#include <unordered_map> // for std::unordered_map
#include <stdexcept>     // for std::invalid_argument
#include <iterator>      // std::prev

template<typename KeyType>

class CacheBase
{
private:
    struct CacheEntry
    {
        KeyType key;
    };

    enum QueueType //safety and more modern (deepseek said)
    {
        NotCached = -1,
        InQueue   = 0,
        HotQueue  = 1
    };

    struct EntryInfo
    {
        typename std::list<CacheEntry>::iterator iter;
        QueueType queue_type;
    };

    // Internal data structures
    std::list<CacheEntry> a1in_queue_;  // FIFO queue
    std::list<CacheEntry> am_queue_;    // LRU queue
    std::unordered_map<KeyType, EntryInfo> cache_map_;

    // Cache configuration
    size_t size_a1in_;
    size_t size_am_;

    void put(std::list<CacheEntry>& queue, QueueType type, size_t max_size, const KeyType& key);

public:
    // Constructor
    CacheBase(size_t total_cache_size)
        : size_a1in_(total_cache_size / 2),
          size_am_(total_cache_size - size_a1in_)
    {
        if (total_cache_size < 2)
        {
            throw std::invalid_argument("Cache size must be positive"); //exception throwing operator !!!
        }
    }

    bool get(const KeyType& key);
    // Getters for statistics and for safety too !!!
    size_t size() const { return cache_map_.size(); }
    size_t max_size() const { return size_a1in_ + size_am_; }
};

// Implementation of template methods must be in the header file
template<typename KeyType>
bool CacheBase<KeyType>::get(const KeyType& key)
{
    auto it = cache_map_.find(key);

    if (it == cache_map_.end())
    {
        // Key not found - insert as new
        put(a1in_queue_,InQueue, size_a1in_, key );
        return false;
    }
    else
    {
        // Get metadata about the element's location
        EntryInfo& info = it->second;  // Reference avoids copying

        if (info.queue_type == InQueue)
        {
            // Move from A1in to Am (promotion to hot queue)
            a1in_queue_.erase(info.iter);
            put(am_queue_, HotQueue, size_am_, key);
        }
        else
        {
            // Element is already in Am -> update LRU: move to end
            am_queue_.splice(am_queue_.end(), am_queue_, info.iter);
            // splice moves without copying or memory allocation
        }
        return true;
    }
}

template<typename KeyType>
void CacheBase<KeyType>::put(std::list<CacheEntry>& queue, QueueType type, size_t max_size, const KeyType& key)
{
    // Create and add entry to the end of куеуе
    queue.push_back({key});  // aggregate initialization

    cache_map_[key] = {std::prev(queue.end()), type};;  // Automatically creates or updates entry

    // Check if A1in queue overflow - evict from front (FIFO)
    if (queue.size() > max_size)
    {
        cache_map_.erase(queue.front().key); //first deleted entry
        queue.pop_front(); //second deleted front elem
    }

}


#endif // CACHE_BASE_HPP
