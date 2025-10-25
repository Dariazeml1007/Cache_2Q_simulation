#ifndef IDEAL_CACHE_HPP
#define IDEAL_CACHE_HPP

#include <vector>
#include <list>
#include <unordered_map>
#include <stdexcept>
#include <limits>
#include <algorithm>
#include <utility> // for std::pair
#include <cassert>  // for asserts

namespace caches
{

template<typename KeyType>
class IdealCache
{
private:
    size_t capacity_;
    std::vector<KeyType> requests_;
    std::list<std::pair<KeyType, size_t>> cache_; // CacheEntry -> pair<Key, next_use>
    std::unordered_map<KeyType, typename std::list<std::pair<KeyType, size_t>>::iterator> cache_map_;
    std::vector<size_t> next_uses_; // precomputed next uses
    size_t current_index_;
    size_t hits_;

    // Precompute all next uses
    void precompute_next_uses();
    void handle_access(const KeyType& key);
    void handle_hit(typename std::unordered_map<KeyType,
    typename std::list<std::pair<KeyType, size_t>>::iterator>::iterator cache_it);
    void handle_miss(const KeyType& key);
    typename std::list<std::pair<KeyType, size_t>>::iterator find_victim();
    void insert(const KeyType& key, size_t next_use);

public:
    IdealCache(size_t capacity, const std::vector<KeyType>& requests)
    : capacity_(capacity), requests_(requests), current_index_(0), hits_(0)
    {
        if (capacity_ == 0)
        {
            throw std::invalid_argument("Cache capacity must be positive");
        }
        precompute_next_uses();
    }

    size_t run();
    size_t hits() const { return hits_; }
    size_t size() const { return cache_.size(); }
    size_t capacity() const { return capacity_; }
};

template<typename KeyType>
void IdealCache<KeyType>::precompute_next_uses()
{
    next_uses_.resize(requests_.size());
    std::unordered_map<KeyType, size_t> last_occurrence;

    // Initialize with "never used again"
    for (size_t i = 0; i < requests_.size(); ++i)
    {
        next_uses_[i] = std::numeric_limits<size_t>::max();
    }

    // Fill next uses in reverse to be efficient
    for (int i = requests_.size() - 1; i >= 0; --i)
    {
        KeyType key = requests_[i];
        if (last_occurrence.find(key) != last_occurrence.end())
        {
            next_uses_[i] = last_occurrence[key];
        }
        last_occurrence[key] = i;
    }
}

template<typename KeyType>
size_t IdealCache<KeyType>::run()
{
    hits_ = 0;
    cache_.clear();
    cache_map_.clear();

    for (current_index_ = 0; current_index_ < requests_.size(); ++current_index_)
    {
        handle_access(requests_[current_index_]);
    }

    return hits_;
}

template<typename KeyType>
void IdealCache<KeyType>::handle_access(const KeyType& key)
{
    auto it = cache_map_.find(key);

    if (it != cache_map_.end())
    {
        // Hit: update next occurrence
        handle_hit(it);
    }
    else
    {
        handle_miss(key);
    }
}

template<typename KeyType>
void IdealCache<KeyType>::handle_hit(typename std::unordered_map<KeyType,
    typename std::list<std::pair<KeyType, size_t>>::iterator>::iterator cache_it)
{
    hits_++;
    assert(cache_it->second != cache_.end()); // iterator must be valid
    cache_it->second->second = next_uses_[current_index_]; // iterator points to pair, then .second is next_use

}

template<typename KeyType>
void IdealCache<KeyType>::handle_miss(const KeyType& key)
{
    // Miss
    size_t next_use = next_uses_[current_index_];
    // Don't cache elements that won't be used again
    if (next_use == std::numeric_limits<size_t>::max())
        return;
    if (cache_.size() < capacity_)
    {
        insert(key, next_use);
    }
    else
    {
        auto victim = find_victim();
        if (next_use < victim->second)
        {
            assert(cache_map_.find(victim->first) != cache_map_.end()); // key must be in cache_map_
            cache_map_.erase(victim->first); // .first is the key
            cache_.erase(victim);
            insert(key, next_use);
        }
    }
}


template<typename KeyType>
typename std::list<std::pair<KeyType, size_t>>::iterator
IdealCache<KeyType>::find_victim()
{
    auto victim = cache_.begin();
    for (auto it = cache_.begin(); it != cache_.end(); ++it)
    {
        if (it->second == std::numeric_limits<size_t>::max()) // it->second is next_occurrence
        {
            return it;
        }
        if (it->second > victim->second)
        {
            victim = it;
        }
    }
    return victim;
}

template<typename KeyType>
void IdealCache<KeyType>::insert(const KeyType& key, size_t next_use)
{
    cache_.emplace_front(key, next_use);
    cache_map_[key] = cache_.begin();
    assert(cache_map_.find(key) != cache_map_.end()); // verify key was added to cache_map_
}

}
#endif
