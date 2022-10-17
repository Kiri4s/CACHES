#pragma once

#include <list>
#include <unordered_map>
#include <iterator>
#include <cassert>

namespace caches {

template <typename T, typename KeyT = int>
struct fifo_cache_t {
    size_t size;
    fifo_cache_t(int sz) : size(sz) {}
    
    std::list<T> cache_;
    using ListIt = typename std::list<T>::iterator;
    std::unordered_map<KeyT, ListIt> hash_;

    bool full() const { return (cache_.size() == size); }
    
    template <typename F>
    bool lookup_update(KeyT key, F slow_get_page_int) {
        if (hash_.count(key) == 1) {
            return true;
        } else {
            if (full()) {
                hash_.erase(cache_.front());
                cache_.pop_front();
            }
            cache_.push_back(slow_get_page_int(key));
            hash_[key] = cache_.end();
            return false;
        }
    }
};

template <typename T, typename KeyT = int>
struct x2Q_cache_t {
    size_t size;
    size_t sz_OUT;
    size_t sz_IN;
    size_t sz_HOT;
    x2Q_cache_t(int sz) : size(sz) {
        assert(sz >= 3);
        sz_OUT = sz * 6 / 10; // 2nd fifo sz
        sz_HOT = (sz - sz_OUT) / 2; // LRU sz
        sz_IN = sz - sz_OUT - sz_HOT; // 1st fifo size
    }

    std::list<T> cache_IN, cache_OUT, cache_HOT; // 1st, 2nd, LRU
    using ListIt = typename std::list<T>::iterator;
    std::unordered_map<KeyT, ListIt> hash_IN, hash_OUT, hash_HOT;

    template <typename F>
    bool lookup_update(KeyT key, F slow_get_page_int) {
        if (hash_HOT.count(key)) { // check LRU
            auto eltit = hash_HOT.at(key); // keeping sorted
            if (eltit != cache_HOT.begin())
                cache_HOT.splice(cache_HOT.begin(), cache_HOT, eltit, std::next(eltit));
            return true;
        }

        if (hash_OUT.count(key)) { // check 2nd fifo
            if (cache_HOT.size() == sz_HOT) {
                hash_HOT.erase(cache_HOT.back()); // delete element from LRU
                cache_HOT.pop_back();
            }
            cache_HOT.push_front(key); // add element to LRU
            hash_HOT[key] = cache_HOT.begin();
            cache_OUT.erase(hash_OUT.at(key)); // delete same element from 2nd fifo
            hash_OUT.erase(key);
            return true;
        }

        if (hash_IN.count(key)) { // check 1st fifo
            return true;
        } else {
            if (cache_IN.size() == sz_IN) {
                if (cache_OUT.size() == sz_OUT) {
                    hash_OUT.erase(cache_OUT.back()); // delete element from 2nd fifo
                    cache_OUT.pop_back();
                }
                cache_OUT.push_front(cache_IN.back()); // add element to 2nd fifo
                hash_OUT[cache_IN.back()] = cache_OUT.begin();
                hash_IN.erase(cache_IN.back()); // delete same element from 1st fifo 
                cache_IN.pop_back();
            }
            cache_IN.push_front(slow_get_page_int(key)); // add element to 1st fifo (and to the CACHE in general)
            hash_IN[key] = cache_IN.begin();
            return false;
        }
    }
};

}
