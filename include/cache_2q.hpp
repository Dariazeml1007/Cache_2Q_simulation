#ifndef CACHE_BASE_HPP
#define CACHE_BASE_HPP

#include <cstddef>       // for size_t
#include <list>          // for std::list
#include <unordered_map> // for std::unordered_map
#include <stdexcept>     // for std::invalid_argument

template<typename KeyType = int>

class CacheBase
{
private:
    struct CacheEntry
    {
        KeyType key;
    };

    enum QueueType
    {
        NOT_CACHED = -1,
        IN_QUEUE   = 0,  // A1in
        HOT_QUEUE  = 1   // Am
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
    size_t hits_;

      // Main cache interface

    void putNew(const KeyType& key);
    void putOld(const KeyType& key);


public:
    // Constructor
    CacheBase(size_t total_cache_size)
        : size_a1in_(total_cache_size / 2),
          size_am_(total_cache_size - size_a1in_),
          hits_(0)
    {
        if (total_cache_size == 0)
        {
            throw std::invalid_argument("Cache size must be positive"); //exception throwing operator !!!
        }
    }

    bool get(const KeyType& key);
    // Getters for statistics and for safety too !!!
    size_t hits() const { return hits_; }
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
        CacheBase<KeyType>::putNew(key);
        return false;
    }
    else
    {
        hits_++;
        // Get metadata about the element's location
        EntryInfo& info = it->second;  // Reference avoids copying

        if (info.queue_type == IN_QUEUE)
        {
            // Move from A1in to Am (promotion to hot queue)
            a1in_queue_.erase(info.iter);
            CacheBase<KeyType>::putOld(key);
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
void CacheBase<KeyType>::putOld(const KeyType& key)
{
    // Add to the end of Am (LRU queue)
    am_queue_.push_back({key});

    // Get iterator to newly added element
    auto new_it = std::prev(am_queue_.end());
    cache_map_[key] = {new_it, HOT_QUEUE};

    // Check if Am queue overflow - evict from front (LRU)
    if (am_queue_.size() > size_am_)
    {
        KeyType front_key = am_queue_.front().key;
        am_queue_.pop_front();
        cache_map_.erase(front_key);
    }
}

template<typename KeyType>
void CacheBase<KeyType>::putNew(const KeyType& key)
{
    // Create and add entry to the end of A1in (FIFO queue)
    a1in_queue_.push_back({key});

    // Get iterator to newly added element
    auto it = std::prev(a1in_queue_.end());

    // Create entry info and add to hash map
    EntryInfo info{it, IN_QUEUE};
    cache_map_[key] = info;  // Automatically creates or updates entry

    // Check if A1in queue overflow - evict from front (FIFO)
    if (a1in_queue_.size() > size_a1in_)
    {
        KeyType front_key = a1in_queue_.front().key;
        a1in_queue_.pop_front();
        cache_map_.erase(front_key);
    }
}

#endif // CACHE_BASE_HPP
