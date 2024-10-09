#include "rjh/unordered_set.hpp"

#include <catch2/catch_test_macros.hpp>

#include <iostream>

namespace rjh::tests {
TEST_CASE("rjh::unordered_set<int>", "[rjh::unordered_set tests]") {
    unordered_set<int> set;

    REQUIRE(set.empty());
    REQUIRE(set.capacity() == 8);
    REQUIRE(set.begin() == set.end());

    for (auto i = 0; i < 100; i++) {
        set.insert(i);
    }

    REQUIRE(set.size() == 100);
    REQUIRE(*set.begin() == 0);

    for (auto i = 0; i < 200; i++) {
        if (i >= 100) {
            REQUIRE(!set.contains(i));
        } else {
            REQUIRE(set.contains(i));
        }
    }

    auto set1 = set;
    REQUIRE(set1.size() == 100);
    REQUIRE(set1.size() == set.size());
    set.clear();
    REQUIRE(set1.size() == 100);
    REQUIRE(set.empty());
}

TEST_CASE("rjh::unordered_set<std::string>", "[rjh::unordered_set tests]") {
    unordered_set<std::string> set;

    REQUIRE(set.empty());

    set.insert("Ryan");
    set.insert("C++");
    set.insert("unordered_set unordered_set unordered_set unordered_set");

    REQUIRE(set.size() == 3);
    REQUIRE(set.contains("Ryan"));
    REQUIRE(!set.contains(std::string{"A"}));

    auto set1 = set;
    REQUIRE(set1.size() == 3);
    set.clear();
    REQUIRE(set.empty());

    for (auto i = 0; i < 1000; i++) {
        set.insert(std::to_string(i));
    }

    REQUIRE(set.size() == 1000);

    for (auto i = 0; i < 2000; i++) {
        if (i >= 1000) {
            REQUIRE(!set.contains(std::to_string(i)));
        } else {
            REQUIRE(set.contains(std::to_string(i)));
        }
    }
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

    custom_type value {
        .name = "Ryan",
    };

    set.insert(value);
    REQUIRE(set.contains(value));
    REQUIRE(set.size() == 1);
    REQUIRE(set.remove(value));
    REQUIRE(!set.remove(custom_type{}));
    REQUIRE(set.empty());
}
} // namespace rjh::tests
