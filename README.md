# CACHES

## Overview

This project provides an implementation of simple **fifo cache** (`fifo_cache_t`) and more smart **2Q cache** (`x2Q_cache_t`).

## 2Q cache explanation

2Q separates entries into **three** logical structures:

**IN** (1st FIFO - Incoming Queue)

- Acts as a small probationary buffer for newly fetched items.
- On a cache miss, the new item is added to the front after fetching it via slow_get_page_int. If full, the oldest (back) item is moved to the front of OUT, making space. Hits in IN do not trigger any movement or promotion.

**HOT** (LRU - Hot Queue)

- Maintains frequently accessed ("hot") items using Least Recently Used (LRU) eviction to prioritize recency and frequency.
- Items are promoted here from OUT on a hit. On a hit in HOT, the item is moved to the front (Most Recently Used position) via splice for efficiency. If full during promotion, the least recently used (back) item is evicted.

**OUT** (2nd FIFO - Outgoing Queue)

- Provides a larger "second chance" buffer for items evicted from IN, allowing them to be promoted to HOT if re-accessed, thus capturing longer-term reuse patterns.
- Receives oldest items from IN when IN is full. On a hit, the item is promoted to the front of HOT and removed from OUT. If full, the oldest (back) item is permanently evicted to make space for new arrivals from IN.

## Build

```bash
# create build directory
mkdir build && cd build
# build
cmake .. && cmake --build .
# run the project
./2Q.out
```

## Usage

The program reads from standard input. The input format is:

```
<m> <n>
<p1> <p2> ... <pn>
```

* `m` — cache capacity (integer)
* `n` — number of page requests (integer)
* `p1..pn` — page keys (integers). These may be separated by spaces or newlines.

Prints the number of cache hits to stdout.

## Example

Input:
```
10 20
1 2 3 4 1 2 5 6 7 1 8 9 2 10 11 12 1 2 3 4
```

IN size = 2

OUT size = 6

LRU size = 2

Expected program output:
```
6
```