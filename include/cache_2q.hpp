#ifndef CACHE_BASE_HPP
#define CACHE_BASE_HPP

#include <cstddef>
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <unordered_map>

using KeyType = int;

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
    std::list<CacheEntry>::iterator iter;
    QueueType queue_type;
};

struct CacheBase
{
    std::list<CacheEntry> a1in_queue;  // FIFO
    std::list<CacheEntry> am_queue;    // LRU

    std::unordered_map<KeyType, EntryInfo> cacheMap;
    // Размеры
    size_t size_a1in ;
    size_t size_am   ;
    size_t hits     ;


       // Конструктор
     CacheBase(size_t total_cache_size)
        : size_a1in(total_cache_size / 2),
          size_am(total_cache_size - size_a1in),
          hits(0)
    {}

    bool get(KeyType key);

    void putNew(KeyType key);

    void putOld(KeyType key);


};


#endif // CACHE_BASE_HPP
