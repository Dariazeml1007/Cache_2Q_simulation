#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <unordered_map>

#include "cache_2q.hpp"


bool CacheBase::get(KeyType key)
{
    auto it = cacheMap.find(key);

    if (it == cacheMap.end())
    {
        CacheBase::putNew(key);
        return false;
    }
    else
    {
        hits++;
        // Get metadata about the element's location
        EntryInfo& info = it->second;  // C++: reference avoids copying
                                       // In C: pointer to entry in hash bucket


        if (info.queue_type == IN_QUEUE)
        {
            a1in_queue.erase(info.iter);
            CacheBase::putOld(key);
        }
        else
        {
            // Element is already in Am -> update LRU: move to end
            am_queue.splice(am_queue.end(), am_queue, info.iter);
            // C++: splice moves without copying or memory allocation
            // In C: would manually remove and reinsert node
        }

    }
    return true;

}

void CacheBase::putOld(KeyType key)
{

    am_queue.push_back({key});

    auto new_it = std::prev(am_queue.end());// Get iterator to newly added element
    cacheMap[key] = {new_it, HOT_QUEUE};


    if (am_queue.size() > size_am)
    {
        KeyType front_key = am_queue.front().key;
        am_queue.pop_front();
        cacheMap.erase(front_key);
    }
}


void CacheBase::putNew(KeyType key)
{
    // Create entry
    CacheEntry entry;
    entry.key = key;

    // Add in the end of A1in
    a1in_queue.push_back(entry);

    // Get an iter on last element(new)
    auto it = std::prev(a1in_queue.end());

    //Add info to list A1in
    EntryInfo info;
    info.iter = it;
    info.queue_type = IN_QUEUE;

    //calculate hash; finds the bucket; looks for the key; if not - creates a new entry !!!
    cacheMap[key] = info;

    // Check if overflow - erase front
    if (a1in_queue.size() > size_a1in)

    {
        KeyType front_key = a1in_queue.front().key;
        a1in_queue.pop_front();
        cacheMap.erase(front_key);
    }
}

