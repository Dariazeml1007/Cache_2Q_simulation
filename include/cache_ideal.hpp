#ifndef IDEAL_CACHE_HPP
#define IDEAL_CACHE_HPP

#include <cstddef>
#include <vector>
#include <unordered_map>
#include <list>
#include <stdexcept>

template<typename KeyType = int>
class IdealCache
{
private:
    struct CacheEntry
    {
        KeyType key;
        size_t next_occurrence; // index of next usage

        CacheEntry(KeyType k, size_t next) : key(k), next_occurrence(next) {}
    };

    size_t capacity_;
    std::vector<KeyType> requests_;
    std::list<CacheEntry> cache_;
    std::unordered_map<KeyType, typename std::list<CacheEntry>::iterator> cache_map_;
    size_t current_index_;
    size_t hits_;

    // Helper methods
    size_t find_next_use(const KeyType& key, size_t start_index);
    typename std::list<CacheEntry>::iterator find_victim();
    void process_request(const KeyType& key);
    void add_to_cache(const KeyType& key);

public:
    // Constructor and main interface
    IdealCache(size_t capacity, const std::vector<KeyType>& requests)
    : capacity_(capacity), requests_(requests), current_index_(0), hits_(0)
    {
        if (capacity_ == 0)
        {
            throw std::invalid_argument("Cache capacity must be positive");
        }
    }

    // Add run() declaration to public section
    size_t run();

    // Getters for statistics
    size_t hits()     const { return hits_; }
    size_t size()     const { return cache_.size(); }
    size_t capacity() const { return capacity_; }
};


template<typename KeyType>
size_t IdealCache<KeyType>::run()
{
    hits_ = 0;
    cache_.clear();
    cache_map_.clear();

    for (current_index_ = 0; current_index_ < requests_.size(); ++current_index_)
    {
        process_request(requests_[current_index_]);
    }

    return hits_;
}

template<typename KeyType>
size_t IdealCache<KeyType>::find_next_use(const KeyType& key, size_t start_index)
{
    // Find next usage of key in request sequence
    for (size_t i = start_index + 1; i < requests_.size(); ++i)
    {
        if (requests_[i] == key)
        {
            return i;
        }
    }
    return requests_.size(); // key won't be used again
}

template<typename KeyType>
typename std::list<typename IdealCache<KeyType>::CacheEntry>::iterator IdealCache<KeyType>::find_victim()
{
    // Find element that will be used farthest in future (or never)
    auto victim = cache_.begin();

    for (auto it = cache_.begin(); it != cache_.end(); ++it)
    {
        if (it->next_occurrence > victim->next_occurrence)
        {
            victim = it;
        }
    }

    return victim;
}

template<typename KeyType>
void IdealCache<KeyType>::process_request(const KeyType& key)
{
    auto it = cache_map_.find(key);

    if (it != cache_map_.end())
    {
        // Hit - element already in cache
        hits_++;
        // Update next usage information
        it->second->next_occurrence = find_next_use(key, current_index_);
    }
    else
    {
        // Miss - element not in cache
        add_to_cache(key);
    }
}

template<typename KeyType>
void IdealCache<KeyType>::add_to_cache(const KeyType& key)
{
    size_t next_use = find_next_use(key, current_index_);

    if (cache_.size() < capacity_)
    {
        // Free space available - simply add
        cache_.emplace_front(key, next_use);
        cache_map_[key] = cache_.begin();
    }
    else
    {
        // Cache full - need to evict someone
        auto victim = find_victim();

        // Evict only if new element will be used sooner than current victim
        if (next_use < victim->next_occurrence)
        {
            // Remove victim
            cache_map_.erase(victim->key);
            cache_.erase(victim);

            // Add new element
            cache_.emplace_front(key, next_use);
            cache_map_[key] = cache_.begin();
        }
    }
}

#endif // IDEAL_CACHE_HPP
