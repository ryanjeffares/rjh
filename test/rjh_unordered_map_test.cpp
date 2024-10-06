#include "rjh/unordered_map.hpp"

#include <catch2/catch_test_macros.hpp>

#include <optional>
#include <string>
#include <vector>

namespace rjh::tests {
TEST_CASE("rjh::unordered_map<int, int>", "[rjh::unordered_map tests]") {
    unordered_map<int, int> map;

    REQUIRE(map.empty());
    REQUIRE(map.capacity() == 8);

    for (auto i = 0; i < 10; i++) {
        map.add(i, i * 2);
    }

    REQUIRE(map.size() == 10);
    REQUIRE(map.capacity() == 16);

    for (auto i = 0; i < 10; i++) {
        const auto f = map.find(i);
        REQUIRE(f.has_value());
        REQUIRE(*f == i * 2);
    }

    REQUIRE(!map.find(20).has_value());

    auto f = map.find(5);
    f->get() = 5;
    REQUIRE(*map.find(5) == 5);

    REQUIRE(map.remove(5));
    REQUIRE(!map.remove(20));
    REQUIRE(!map.add(0, 0));
    REQUIRE(map.size() == 9);

    map[9] = 9;
    map[20] = 20;

    REQUIRE(map[9] == 9);
    REQUIRE(map[20] == 20);
    REQUIRE(map[40] == 0);
}

TEST_CASE("rjh::unordered_map<std::string, std::optional<int>>", "[rjh::unordered_map tests]") {
    unordered_map<std::string, std::optional<int>> map;

    REQUIRE(map.add("Ryan", 25));
    REQUIRE(map.size() == 1);
    REQUIRE(!map.add(std::string{"Ryan"}, std::nullopt));
    REQUIRE(map.find("Ryan")->get().has_value());
    REQUIRE(map.find("Ryan")->get() == 25);
    REQUIRE(*map.find("Ryan")->get() == 25);

    map.find("Ryan")->get() = 10;
    REQUIRE(map.find("Ryan")->get() == 10);

    for (auto i = 0; i < 20; i++) {
        REQUIRE(map.add(std::to_string(i), i));
    }

    REQUIRE(map.size() == 21);

    for (auto i = 0; i < 20; i++) {
        REQUIRE(map.find(std::to_string(i))->get() == i);
    }

    map["A"] = 0;
    map["B"] = std::nullopt;

    REQUIRE(map["A"] == 0);
    REQUIRE(map.find("A")->get() == 0);
    REQUIRE(map.find("B")->get() == std::nullopt);

    map.clear();
    REQUIRE(map.empty());

    auto map1 = map;
    REQUIRE(map1.size() == map.size());
    auto map2 = std::move(map1);
    REQUIRE(map2.size() == map.size());
    REQUIRE(map1.empty());
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

    custom_type value {
        .name = "Ryan",
    };

    map.insert(value, "Ryan");
    REQUIRE(map[value] == "Ryan");
    REQUIRE(map[custom_type{}].empty());
    map.remove(value);
    REQUIRE(map.size() == 1);
}
} // namespace rjh::tests
