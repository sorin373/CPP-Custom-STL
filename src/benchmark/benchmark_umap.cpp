#include "../STL/containers/unordered_map/unordered_map.h"

#include <iostream>
#include <unordered_map>
#include <chrono>
#include <cstdint>

using clock_type = std::chrono::steady_clock;
static volatile std::uint64_t sink = 0;

struct Big {
    int a[16];
    Big() { a[0] = 1; }
    Big(int x) { a[0] = x; }
};

template <class F>
long long bench_ms(const char* name, F&& f, int warmup = 1, int iters = 5)
{
    for (int i = 0; i < warmup; ++i) f();

    long long best = (1LL << 60);

    for (int i = 0; i < iters; ++i)
    {
        auto t0 = clock_type::now();
        f();
        auto t1 = clock_type::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        if (ms < best) best = ms;
    }

    std::cout << name << ": " << best << " ms\n";

    return best;
}

static inline std::uint64_t lcg_next(std::uint64_t& x)
{
    x = x * 2862933555777941757ULL + 3037000493ULL;
    return x;
}

template <class Map>
static void touch_map(Map& m)
{
    sink += (std::uint64_t)m.size();
    if (!m.empty())
    {
        auto it = m.find(1);
        if (it != m.end()) sink += (std::uint64_t)it->second.a[0];
    }
}

int main()
{
    const std::size_t N = 1000000; // number of insertions
    const std::size_t Q = 1000000; // number of queries
    const std::size_t E = 200000;  // number of erases

    std::cout << "N=" << N << " Q=" << Q << " E=" << E << "\n\n";

    bench_ms("std::unordered_map<int,Big> insert (no reserve)", [&]{
        std::unordered_map<int, Big> m;
        std::uint64_t x = 123456789ULL;
        
        for (std::size_t i = 0; i < N; ++i) 
        {
            int k = (int)(lcg_next(x) & 0x7fffffff);
            m.insert({k, Big(k)});
        }
        touch_map(m);
    });

    bench_ms("stl::unordered_map<int,Big> insert (no reserve)", [&]{
        stl::unordered_map<int, Big> m;
        std::uint64_t x = 123456789ULL;
        for (std::size_t i = 0; i < N; ++i) {
            int k = (int)(lcg_next(x) & 0x7fffffff);
            m.insert({k, Big(k)});
        }
        touch_map(m);
    });

    std::cout << "\n";

    bench_ms("std::unordered_map<int,Big> insert (reserve)", [&]{
        std::unordered_map<int, Big> m;
        m.reserve(N); // tries to avoid rehashing during inserts
        std::uint64_t x = 123456789ULL;
        for (std::size_t i = 0; i < N; ++i) {
            int k = (int)(lcg_next(x) & 0x7fffffff);
            m.insert({k, Big(k)});
        }
        touch_map(m);
    });

    bench_ms("stl::unordered_map<int,Big> insert (rehash/reserve)", [&]{
        stl::unordered_map<int, Big> m;

        m.rehash(N);

        std::uint64_t x = 123456789ULL;

        for (std::size_t i = 0; i < N; ++i) 
        {
            int k = (int)(lcg_next(x) & 0x7fffffff);
            m.insert({k, Big(k)});
        }

        touch_map(m);
    });

    std::cout << "\n";

    bench_ms("std::unordered_map<int,Big> operator[] insert (reserve)", [&]{
        std::unordered_map<int, Big> m;
        m.reserve(N);
        std::uint64_t x = 987654321ULL;

        for (std::size_t i = 0; i < N; ++i) 
        {
            int k = (int)(lcg_next(x) & 0x7fffffff);
            m[k] = Big(k);
        }

        touch_map(m);
    });

    bench_ms("stl::unordered_map<int,Big> operator[] insert (rehash/reserve)", [&]{
        stl::unordered_map<int, Big> m;

        m.rehash(N);

        std::uint64_t x = 987654321ULL;
        for (std::size_t i = 0; i < N; ++i)
        {
            int k = (int)(lcg_next(x) & 0x7fffffff);
            m[k] = Big(k);
        }

        touch_map(m);
    });

    std::cout << "\n";

    bench_ms("std::unordered_map<int,Big> find HINT", [&]{
        std::unordered_map<int, Big> m;
        m.reserve(N);
        for (int i = 0; i < (int)N; ++i) m.insert({i, Big(i)});

        std::uint64_t x = 111111111ULL;
        std::uint64_t acc = 0;

        for (std::size_t i = 0; i < Q; ++i) 
        {
            int k = (int)(lcg_next(x) % N);
            auto it = m.find(k);
            if (it != m.end()) acc += (std::uint64_t)it->second.a[0];
        }

        sink += acc;
    });

    bench_ms("stl::unordered_map<int,Big> find HINT", [&]{
        stl::unordered_map<int, Big> m;
        m.rehash(N);
        for (int i = 0; i < (int)N; ++i) m.insert({i, Big(i)});

        std::uint64_t x = 111111111ULL;
        std::uint64_t acc = 0;

        for (std::size_t i = 0; i < Q; ++i) 
        {
            int k = (int)(lcg_next(x) % N);
            auto it = m.find(k);
            if (it != m.end()) acc += (std::uint64_t)it->second.a[0];
        }

        sink += acc;
    });

    std::cout << "\n";

    bench_ms("std::unordered_map<int,Big> find MISS", [&]{
        std::unordered_map<int, Big> m;
        m.reserve(N);
        for (int i = 0; i < (int)N; ++i) m.insert({i, Big(i)});

        std::uint64_t x = 222222222ULL;
        std::uint64_t acc = 0;
        for (std::size_t i = 0; i < Q; ++i) 
        {
            int k = (int)(N + (lcg_next(x) % N));
            auto it = m.find(k);
            if (it != m.end()) acc += (std::uint64_t)it->second.a[0];
        }
        sink += acc;
    });

    bench_ms("stl::unordered_map<int,Big> find MISS", [&]{
        stl::unordered_map<int, Big> m;
        m.rehash(N);
        for (int i = 0; i < (int)N; ++i) m.insert({i, Big(i)});

        std::uint64_t x = 222222222ULL;
        std::uint64_t acc = 0;
        for (std::size_t i = 0; i < Q; ++i) {
            int k = (int)(N + (lcg_next(x) % N)); // miss
            auto it = m.find(k);
            if (it != m.end()) acc += (std::uint64_t)it->second.a[0];
        }
        sink += acc;
    });

    std::cout << "\n";

    bench_ms("std::unordered_map<int,Big> erase by key", [&]{
        std::unordered_map<int, Big> m;
        m.reserve(N);
        for (int i = 0; i < (int)N; ++i) m.insert({i, Big(i)});

        std::uint64_t x = 333333333ULL;
        std::size_t removed = 0;
        for (std::size_t i = 0; i < E; ++i) 
        {
            int k = (int)(lcg_next(x) % N);
            removed += (m.erase(k) != 0);
        }
        sink += removed;
        sink += m.size();
    });

    bench_ms("stl::unordered_map<int,Big> erase by key", [&]{
        stl::unordered_map<int, Big> m;
        m.rehash(N);
        for (int i = 0; i < (int)N; ++i) m.insert({i, Big(i)});

        std::uint64_t x = 333333333ULL;
        std::size_t removed = 0;
        for (std::size_t i = 0; i < E; ++i) 
        {
            int k = (int)(lcg_next(x) % N);
            removed += (m.erase(k) != 0);
        }
        sink += removed;
        sink += m.size();
    });

    std::cout << "\nDone. sink=" << sink << "\n";
    return 0;
}