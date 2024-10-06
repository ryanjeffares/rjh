#include <rjh/unordered_set.hpp>
#include <rjh/unordered_map.hpp>

#include <benchmark/benchmark.h>

#include <unordered_map>
#include <unordered_set>
#include <string>

static auto benchmark_std_unordered_set_adding_ints(benchmark::State& state) -> void {
    for (auto _ : state) {
        std::unordered_set<int> set;
        for (auto i = 0; i < 1000000; i++) {
            set.insert(i);
        }
    }
}

BENCHMARK(benchmark_std_unordered_set_adding_ints);

static auto benchmark_rjh_unordered_set_adding_ints(benchmark::State& state) -> void {
    for (auto _ : state) {
        rjh::unordered_set<int> set;
        for (auto i = 0; i < 1000000; i++) {
            set.insert(i);
        }
    }
}

BENCHMARK(benchmark_rjh_unordered_set_adding_ints);

static auto benchmark_std_unordered_set_adding_strings(benchmark::State& state) -> void {
    for (auto _ : state) {
        std::unordered_set<std::string> set;
        for (auto i = 0; i < 1000000; i++) {
            set.insert(std::to_string(i));
        }
    }
}

BENCHMARK(benchmark_std_unordered_set_adding_strings);

static auto benchmark_rjh_unordered_set_adding_strings(benchmark::State& state) -> void {
    for (auto _ : state) {
        rjh::unordered_set<std::string> set;
        for (auto i = 0; i < 1000000; i++) {
            set.insert(std::to_string(i));
        }
    }
}

BENCHMARK(benchmark_rjh_unordered_set_adding_strings);

static auto benchmark_std_unordered_map_adding_ints(benchmark::State& state) -> void {
    for (auto _ : state) {
        std::unordered_map<int, int> map;
        for (auto i = 0; i < 1000000; i++) {
            map.emplace(i, i * 2);
        }
    }
}

BENCHMARK(benchmark_std_unordered_map_adding_ints);

static auto benchmark_rjh_unordered_map_adding_ints(benchmark::State& state) -> void {
    for (auto _ : state) {
        rjh::unordered_map<int, int> map;
        for (auto i = 0; i < 1000000; i++) {
            map.insert(i, i * 2);
        }
    }
}

BENCHMARK(benchmark_rjh_unordered_map_adding_ints);

static auto benchmark_std_unordered_map_adding_strings(benchmark::State& state) -> void {
    for (auto _ : state) {
        std::unordered_map<std::string, std::string> map;
        for (auto i = 0; i < 1000000; i++) {
            map.emplace(std::to_string(i), std::to_string(i * 2));
        }
    }
}

BENCHMARK(benchmark_std_unordered_map_adding_strings);

static auto benchmark_rjh_unordered_map_adding_strings(benchmark::State& state) -> void {
    for (auto _ : state) {
        rjh::unordered_map<std::string, std::string> map;
        for (auto i = 0; i < 1000000; i++) {
            map.insert(std::to_string(i), std::to_string(i * 2));
        }
    }
}

BENCHMARK(benchmark_rjh_unordered_map_adding_strings);

BENCHMARK_MAIN();
