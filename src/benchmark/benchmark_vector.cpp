#include "../STL/containers/vector/vector.h"

#include <iostream>
#include <chrono>
#include <vector>
#include <cstdint>
#include <fstream>

std::ofstream fout("data.out");
using clock_type = std::chrono::steady_clock;
static volatile std::uint64_t sink = 0;

struct Big {
    int a[16];
    Big() { a[0]=1; }
    Big(int x) { a[0]=x; }
    Big(const Big&) = default;
    Big(Big&&) noexcept = default;
    Big& operator=(const Big&) = default;
    Big& operator=(Big&&) noexcept = default;
    ~Big() {}
};

template <class F>
long long bench_ms(const char* name, F&& f, int warmup = 1, int iters = 5)
{
    for (int i = 0; i < warmup; ++i) f();

    long long best = (1LL<<60);
    for (int i = 0; i < iters; ++i)
    {
        auto t0 = clock_type::now();
        f();
        auto t1 = clock_type::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        if (ms < best) best = ms;
    }
    
    std::cout << name << ": " << best << " ms\n";

    fout << name << ": " << best << " ms\n";

    return best;
}

template <class Vec>
static void touch_vec(Vec& v)
{
    // prevent optimizing away + touch some data
    if (v.size() > 0) sink += (std::uint64_t)v[0].a[0];
    if (v.size() > 2) sink += (std::uint64_t)v[v.size()/2].a[0];
    if (v.size() > 1) sink += (std::uint64_t)v[v.size()-1].a[0];
    sink += (std::uint64_t)v.size();
}

int main()
{
    
    const std::size_t N = 1000000;        // elem.
    const std::size_t M = 10000;          // insert/erase ops
    const std::size_t R = 500000;         // resize target

    std::cout << "N=" << N << " M=" << M << " R=" << R << "\n\n";

    bench_ms("std::vector<Big> push_back copy (no reserve)", [&]{
        std::vector<Big> v;
        Big b(7);
        for (std::size_t i = 0; i < N; ++i) v.push_back(b);
        touch_vec(v);
    });

    bench_ms("stl::vector<Big> push_back copy (no reserve)", [&]{
        stl::vector<Big> v;
        Big b(7);
        for (std::size_t i = 0; i < N; ++i) v.push_back(b);
        touch_vec(v);
    });

    std::cout << "\n";

    bench_ms("std::vector<Big> push_back copy (reserve)", [&]{
        std::vector<Big> v;
        v.reserve(N);
        Big b(7);
        for (std::size_t i = 0; i < N; ++i) v.push_back(b);
        touch_vec(v);
    });

    bench_ms("stl::vector<Big> push_back copy (reserve)", [&]{
        stl::vector<Big> v;
        v.reserve(N);
        Big b(7);
        for (std::size_t i = 0; i < N; ++i) v.push_back(b);
        touch_vec(v);
    });

    std::cout << "\n";

    bench_ms("std::vector<Big> push_back move (reserve)", [&]{
        std::vector<Big> v;
        v.reserve(N);
        for (std::size_t i = 0; i < N; ++i) 
        {
            Big b((int)i);
            v.push_back(std::move(b));
        }

        touch_vec(v);
    });

    bench_ms("stl::vector<Big> push_back move (reserve)", [&]{
        stl::vector<Big> v;
        v.reserve(N);
        for (std::size_t i = 0; i < N; ++i) 
        {
            Big b((int)i);
            v.push_back(stl::move(b));
        }
        touch_vec(v);
    });

    std::cout << "\n";

    bench_ms("std::vector<Big> emplace_back (reserve)", [&]{
        std::vector<Big> v;
        v.reserve(N);
        for (std::size_t i = 0; i < N; ++i) v.emplace_back((int)i);
        touch_vec(v);
    });

    bench_ms("stl::vector<Big> emplace_back (reserve)", [&]{
        stl::vector<Big> v;
        v.reserve(N);
        for (std::size_t i = 0; i < N; ++i) v.emplace_back((int)i);
        touch_vec(v);
    });

    std::cout << "\n";

    bench_ms("std::vector<Big> resize grow+shrink", [&]{
        std::vector<Big> v;
        v.reserve(R);
        v.resize(R, Big(3));
        v.resize(R/2);
        touch_vec(v);
    });

    bench_ms("stl::vector<Big> resize grow+shrink", [&]{
        stl::vector<Big> v;
        v.reserve(R);
        v.resize(R, Big(3));
        v.resize(R/2);
        touch_vec(v);
    });

    std::cout << "\n";

    bench_ms("std::vector<Big> clear + refill", [&]{
        std::vector<Big> v;
        v.reserve(N/2);
        for (std::size_t i = 0; i < N/2; ++i) v.emplace_back((int)i);
        v.clear();
        for (std::size_t i = 0; i < N/2; ++i) v.emplace_back((int)i);
        touch_vec(v);
    });

    bench_ms("stl::vector<Big> clear + refill", [&]{
        stl::vector<Big> v;
        v.reserve(N/2);
        for (std::size_t i = 0; i < N/2; ++i) v.emplace_back((int)i);
        v.clear();
        for (std::size_t i = 0; i < N/2; ++i) v.emplace_back((int)i);
        touch_vec(v);
    });

    std::cout << "\n";

    bench_ms("std::vector<Big> insert middle", [&]{
        std::vector<Big> v;
        v.reserve(M + 10);
        for (std::size_t i = 0; i < 10; ++i) v.emplace_back((int)i);

        for (std::size_t i = 0; i < M; ++i)
            v.insert(v.begin() + (v.size()/2), Big((int)i));
        
        touch_vec(v);
    });

    bench_ms("stl::vector<Big> insert middle", [&]{
        stl::vector<Big> v;
        v.reserve(M + 10);
        for (std::size_t i = 0; i < M; ++i) v.emplace_back((int)i);

        for (std::size_t i = 0; i < M; ++i)
            v.insert(v.begin() + (v.size()/2), Big((int)i));
        
        touch_vec(v);
    });

    std::cout << "\n";

    bench_ms("std::vector<Big> erase middle", [&]{
        std::vector<Big> v;
        v.reserve(M);
        
        for (std::size_t i = 0; i < M; ++i) v.emplace_back((int)i);

        while (v.size() > 0)
            v.erase(v.begin() + (v.size()/2));
        
        sink += 1;
    });

    bench_ms("stl::vector<Big> erase middle", [&]{
        stl::vector<Big> v;
        v.reserve(M);

        for (std::size_t i = 0; i < M; ++i) v.emplace_back((int)i);

        while (v.size() > 0)
            v.erase(v.begin() + (v.size()/2));

        sink += 1;
    });

    std::cout << "\nDone. sink=" << sink << "\n";

    fout.close();

    return 0;
}