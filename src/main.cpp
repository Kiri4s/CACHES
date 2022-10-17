#include <iostream>
#include <cassert>
#include "caches.hpp"

int slow_get_page_int(int key) { return key; }

int main() {
    int n = 0, m = 0;
    std::cin >> m >> n;
    assert(std::cin.good());

    caches::x2Q_cache_t<int> c{m};
    
    int hits = 0;
    for (int i = 0; i < n; ++i) {
        int p = 0;
        std::cin >> p;
        assert(std::cin.good());
        if (c.lookup_update(p, slow_get_page_int)) hits++;
    }
    std::cout << hits << std::endl;
    return 0;
}
