#include "../STL/containers/forward_list/forward_list.h"

#include <forward_list>
#include <iostream>
#include <chrono>
#include <cstdint>
#include <limits>

using clock_type = std::chrono::steady_clock;
static volatile std::uint64_t sink = 0;

struct Big {
    int a[16];
    Big() { a[0] = 1; }
    explicit Big(int x) { a[0] = x; }
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

template <class List>
static void touch_list(List& l)
{
    std::uint64_t acc = 0;
    auto it = l.begin();

    for (int i = 0; i < 4 && it != l.end(); ++i, ++it)
        acc += (std::uint64_t)it->a[0];

    sink += acc;
}

template <class List>
static inline bool linear_find(const List& l, int key, std::uint64_t& acc)
{
    for (auto it = l.cbegin(); it != l.cend(); ++it)
    {
        if (it->a[0] == key)
        {
            acc += (std::uint64_t)it->a[0];
            return true;
        }
    }
    return false;
}

int main()
{
    const std::size_t N = 200000; // number of elements
    const std::size_t Q = 20000;  // number of linear-search queries
    const std::size_t E = 100000; // number of erases (pop_front)

    std::cout << "N=" << N << " Q=" << Q << " E=" << E << "\n\n";

    bench_ms("std::forward_list<Big> push_front", [&]{
        std::forward_list<Big> l;
        std::uint64_t x = 123456789ULL;
        for (std::size_t i = 0; i < N; ++i)
        {
            int k = (int)(lcg_next(x) & 0x7fffffff);
            l.push_front(Big(k));
        }
        touch_list(l);
    });

    bench_ms("stl::forward_list<Big> push_front", [&]{
        stl::forward_list<Big> l;
        std::uint64_t x = 123456789ULL;

        for (std::size_t i = 0; i < N; ++i)
        {
            int k = (int)(lcg_next(x) & 0x7fffffff);
            l.push_front(Big(k));
        }

        touch_list(l);
    });

    std::cout << "\n";

    bench_ms("std::forward_list<Big> insert_after (before_begin)", [&]{
        std::forward_list<Big> l;
        auto pos = l.before_begin();
        std::uint64_t x = 987654321ULL;
        for (std::size_t i = 0; i < N; ++i)
        {
            int k = (int)(lcg_next(x) & 0x7fffffff);
            pos = l.insert_after(pos, Big(k));
        }
        touch_list(l);
    });

    bench_ms("stl::forward_list<Big> insert_after (before_begin)", [&]{
        stl::forward_list<Big> l;
        auto pos = l.before_begin();
        std::uint64_t x = 987654321ULL;
        for (std::size_t i = 0; i < N; ++i)
        {
            int k = (int)(lcg_next(x) & 0x7fffffff);
            pos = l.insert_after(pos, Big(k));
        }
        touch_list(l);
    });

    std::cout << "\n";

    bench_ms("std::forward_list<Big> iterate/sum", [&]{
        std::forward_list<Big> l;
        for (int i = 0; i < (int)N; ++i) l.push_front(Big(i));

        std::uint64_t acc = 0;
        for (const auto& v : l) acc += (std::uint64_t)v.a[0];
        sink += acc;
    });

    bench_ms("stl::forward_list<Big> iterate/sum", [&]{
        stl::forward_list<Big> l;
        for (int i = 0; i < (int)N; ++i) l.push_front(Big(i));

        std::uint64_t acc = 0;
        for (auto it = l.begin(); it != l.end(); ++it) acc += (std::uint64_t)it->a[0];
        sink += acc;
    });

    std::cout << "\n";

    bench_ms("std::forward_list<Big> linear_find HIT", [&]{
        std::forward_list<Big> l;
        for (int i = 0; i < (int)N; ++i) l.push_front(Big(i));

        std::uint64_t x = 111111111ULL;
        std::uint64_t acc = 0;
        std::size_t hits = 0;

        for (std::size_t i = 0; i < Q; ++i)
        {
            int k = (int)(lcg_next(x) % N);
            hits += linear_find(l, k, acc);
        }

        sink += acc + hits;
    }, 1, 3);

    bench_ms("stl::forward_list<Big> linear_find HIT", [&]{
        stl::forward_list<Big> l;
        for (int i = 0; i < (int)N; ++i) l.push_front(Big(i));

        std::uint64_t x = 111111111ULL;
        std::uint64_t acc = 0;
        std::size_t hits = 0;

        for (std::size_t i = 0; i < Q; ++i)
        {
            int k = (int)(lcg_next(x) % N);
            hits += linear_find(l, k, acc);
        }

        sink += acc + hits;
    }, 1, 3);

    std::cout << "\n";

    bench_ms("std::forward_list<Big> linear_find MISS", [&]{
        std::forward_list<Big> l;
        for (int i = 0; i < (int)N; ++i) l.push_front(Big(i));

        std::uint64_t x = 222222222ULL;
        std::uint64_t acc = 0;
        std::size_t hits = 0;

        for (std::size_t i = 0; i < Q; ++i)
        {
            int k = (int)(N + (lcg_next(x) % N));
            hits += linear_find(l, k, acc);
        }

        sink += acc + hits;
    }, 1, 3);

    bench_ms("stl::forward_list<Big> linear_find MISS", [&]{
        stl::forward_list<Big> l;
        for (int i = 0; i < (int)N; ++i) l.push_front(Big(i));

        std::uint64_t x = 222222222ULL;
        std::uint64_t acc = 0;
        std::size_t hits = 0;

        for (std::size_t i = 0; i < Q; ++i)
        {
            int k = (int)(N + (lcg_next(x) % N));
            hits += linear_find(l, k, acc);
        }

        sink += acc + hits;
    }, 1, 3);

    std::cout << "\n";

    bench_ms("std::forward_list<Big> pop_front (E times)", [&]{
        std::forward_list<Big> l;
        for (int i = 0; i < (int)N; ++i) l.push_front(Big(i));

        std::size_t cnt = 0;
        for (std::size_t i = 0; i < E && !l.empty(); ++i)
        {
            l.pop_front();
            ++cnt;
        }
        sink += cnt;
    });

    bench_ms("stl::forward_list<Big> pop_front (E times)", [&]{
        stl::forward_list<Big> l;
        for (int i = 0; i < (int)N; ++i) l.push_front(Big(i));

        std::size_t cnt = 0;
        for (std::size_t i = 0; i < E && !l.empty(); ++i)
        {
            l.pop_front();
            ++cnt;
        }
        sink += cnt;
    });

    std::cout << "\nDone. sink=" << sink << "\n";

    return 0;
}