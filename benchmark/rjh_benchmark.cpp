/*
 * Copyright 2024 Ryan Jeffares (ryan.jeffares.business@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the “Software”), to deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright
 * notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <rjh/unordered_map.hpp>
#include <rjh/unordered_set.hpp>

#include <benchmark/benchmark.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

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
        auto it = map.begin();
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
