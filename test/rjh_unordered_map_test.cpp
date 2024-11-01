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

#include "rjh/unordered_map.hpp"

#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace rjh::tests {
TEST_CASE("rjh::unordered_map<int, int>", "[rjh::unordered_map tests]") {

    struct string_hash
    {
        using hash_type = std::hash<std::string_view>;
        using is_transparent = void;

        std::size_t operator()(const char* str) const        { return hash_type{}(str); }
        std::size_t operator()(std::string_view str) const   { return hash_type{}(str); }
        std::size_t operator()(std::string const& str) const { return hash_type{}(str); }
    };

    unordered_map<std::string, int, string_hash, std::equal_to<>> map;
    map.contains(std::string_view{""});
}

TEST_CASE("rjh::unordered_map<std::string, std::optional<int>>", "[rjh::unordered_map tests]") {
    unordered_map<std::string, std::optional<int>> map;

}

TEST_CASE("rjh::unordered_map<custom_type, std::string>", "[rjh::unordered_map tests]") {
    struct custom_type {
        std::string name;
        std::vector<std::string> data;
    };

    struct custom_type_hasher {
        [[nodiscard]] auto operator()(const custom_type& value) const noexcept -> std::size_t {
            return std::hash<std::string>{}(value.name);
        }
    };

    unordered_map<custom_type, std::string, custom_type_hasher> map;

}
} // namespace rjh::tests
