#ifndef IDEAL_CACHE_HPP
#define IDEAL_CACHE_HPP

#include <vector>
#include <list>
#include <unordered_map>
#include <stdexcept>
#include <limits>
#include <algorithm>

template<typename KeyType>
class IdealCache
{
private:
    struct CacheEntry
    {
        KeyType key;
        size_t next_occurrence;

        CacheEntry(KeyType k, size_t next) : key(k), next_occurrence(next) {}
    };

    size_t capacity_;
    std::vector<KeyType> requests_;
    std::list<CacheEntry> cache_;
    std::unordered_map<KeyType, typename std::list<CacheEntry>::iterator> cache_map_;
    std::vector<size_t> next_uses_; // precomputed next uses
    size_t current_index_;
    size_t hits_;

    // Precompute all next uses
    void precompute_next_uses();
    //typename std::list<CacheEntry>::iterator find_victim();

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
        next_uses_[i] = std::numeric_limits<size_t>::max(); //max_size
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
        KeyType key = requests_[current_index_];
        auto it = cache_map_.find(key);

        if (it != cache_map_.end())
        {
            hits_++;
            // Update next usage
            it->second->next_occurrence = next_uses_[current_index_];
        }
        else
        {
            // Miss
            size_t next_use = next_uses_[current_index_];

            // Only add if will be used again
            if (next_use != std::numeric_limits<size_t>::max())
            {
                if (cache_.size() < capacity_)
                {
                    // Free space - add
                    cache_.emplace_front(key, next_use);
                    cache_map_[key] = cache_.begin();
                }
                else
                {
                    // Find victim - element with largest next_use
                    auto victim = cache_.begin();
                    for (auto it = cache_.begin(); it != cache_.end(); ++it)
                    {
                        if (it->next_occurrence == std::numeric_limits<size_t>::max())
                        {
                            victim = it;
                            break;
                        }
                        if (it->next_occurrence > victim->next_occurrence)
                        {
                            victim = it;
                        }
                    }

                    // Replace only if new element is needed sooner
                    if (next_use < victim->next_occurrence)
                    {
                        cache_map_.erase(victim->key);
                        cache_.erase(victim);

                        cache_.emplace_front(key, next_use);
                        cache_map_[key] = cache_.begin();
                    }
                }
            }
        }
    }

    return hits_;
}

#endif // IDEAL_CACHE_HPP
