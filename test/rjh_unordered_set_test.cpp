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

#include "rjh/unordered_set.hpp"

#include <catch2/catch_test_macros.hpp>

#include <iostream>

namespace rjh::tests {
TEST_CASE("rjh::unordered_set<int>", "[rjh::unordered_set tests]") {
    unordered_set<int> set;


}

TEST_CASE("rjh::unordered_set<std::string>", "[rjh::unordered_set tests]") {
    unordered_set<std::string> set;

}

TEST_CASE("rjh::unordered_set<custom_type>", "[rjh::unordered_set tests]") {
    struct custom_type {
        std::string name;
        std::vector<std::string> data;

        auto operator==(const custom_type& other) const noexcept -> bool {
            return name == other.name;
        }
    };

    struct custom_type_hasher {
        [[nodiscard]] auto operator()(const custom_type& value) const noexcept -> std::size_t {
            return std::hash<std::string>{}(value.name);
        }
    };

    unordered_set<custom_type, custom_type_hasher> set;

}
} // namespace rjh::tests
