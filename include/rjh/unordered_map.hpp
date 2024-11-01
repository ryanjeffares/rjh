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

#ifndef RJH_UNORDERED_MAP_HPP
#define RJH_UNORDERED_MAP_HPP

#include "detail/hash_table.hpp"

#include <cstddef>
#include <functional>
#include <iterator>
#include <utility>

namespace rjh {
template<
    typename Key,
    typename Value,
    concepts::hash_function_object<Key> Hash = std::hash<Key>,
    concepts::key_equal_function_object<Key> KeyEqual = std::equal_to<Key>
>
class unordered_map {
public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = std::pair<key_type, mapped_type>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using reference = value_type&;
    using const_reference = const value_type&;

    static constexpr bool transparent_hash_eq = concepts::is_transparent<hasher> && concepts::is_transparent<key_equal>;

private:
    struct pair_hash {
        using is_transparent = void;

        [[nodiscard]]
        auto operator()(const_reference pair) const noexcept -> size_type {
            return hasher{}(pair.first);
        }

        [[nodiscard]]
        auto operator()(const key_type& key) const noexcept -> size_type {
            return hasher{}(key);
        }

        template<typename K>
        [[nodiscard]]
        auto operator()(const K& key) const noexcept -> size_type {
            return hasher{}(key);
        }
    };

    struct pair_key_equal {
        using is_transparent = void;

        [[nodiscard]]
        auto operator()(const_reference a, const_reference b) const noexcept -> bool {
            return key_equal{}(a.first, b.first);
        }

        [[nodiscard]]
        auto operator()(const_reference a, const key_type& b) const noexcept -> bool {
            return key_equal{}(a.first, b);
        }

        [[nodiscard]]
        auto operator()(const key_type& a, const_reference b) const noexcept -> bool {
            return key_equal{}(a, b.first);
        }

        template<typename K>
        [[nodiscard]]
        auto operator()(const_reference a, const K& b) const noexcept -> bool {
            return key_equal{}(a.first, b);
        }

        template<typename K>
        [[nodiscard]]
        auto operator()(const K& a, const_reference b) const noexcept -> bool {
            return key_equal{}(a, b.first);
        }

        template<typename K>
        [[nodiscard]]
        auto operator()(const key_type& a, const K& b) const noexcept -> bool {
            return key_equal{}(a, b);
        }

        template<typename K>
        [[nodiscard]]
        auto operator()(const K& a, const key_type& b) const noexcept -> bool {
            return key_equal{}(a, b);
        }
    };

    using hash_table = detail::hash_table<value_type, pair_hash, pair_key_equal>;

public:

    template<typename T, typename It>
    class raw_iterator final {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type  = difference_type;
        using value_type = T;
        using pointer = value_type*;
        using const_pointer = const pointer;
        using reference  = value_type&;
        using const_reference = const value_type&;
        using key_type = std::conditional_t<std::is_const_v<T>, const key_type, key_type>;
        using key_type_reference = key_type&;
        using const_key_type_reference = const key_type&;
        using mapped_type = std::conditional_t<std::is_const_v<T>, const mapped_type, mapped_type>;
        using mapped_type_reference = mapped_type&;
        using const_mapped_type_reference = const mapped_type&;
        using table_iterator = It;

        raw_iterator(table_iterator it) : m_iterator{it} {

        }

        auto operator*() const noexcept -> const_reference {
            return m_iterator->key;
        }

        auto operator->() const noexcept -> const_pointer {
            return &m_iterator->key;
        }

        auto key() const noexcept -> const_key_type_reference {
            return m_iterator->key.first;
        }

        auto value() const noexcept -> mapped_type_reference {
            return m_iterator->key.second;
        }

        auto operator++() noexcept -> raw_iterator& {
            m_iterator++;
            return *this;
        }

        auto operator++(int) noexcept -> raw_iterator {
            auto temp = *this;
            ++(*this);
            return temp;
        }

        friend auto operator==(const raw_iterator& a, const raw_iterator& b) noexcept -> bool {
            return a.m_iterator == b.m_iterator;
        }

        friend auto operator!=(const raw_iterator& a, const raw_iterator& b) noexcept -> bool {
            return a.m_iterator != b.m_iterator;
        }

    private:
        table_iterator m_iterator;
    };

    using iterator = raw_iterator<value_type, typename hash_table::iterator>;
    using const_iterator = raw_iterator<const value_type, typename hash_table::const_iterator>;

    auto insert(const_reference pair) noexcept -> std::pair<iterator, bool> {
        return m_hash_table.insert(pair);
    }

    auto insert(value_type&& pair) noexcept -> std::pair<iterator, bool> {
        return m_hash_table.insert(std::move(pair));
    }

    template<typename Pair> requires std::constructible_from<value_type, Pair&&>
    auto insert(Pair&& pair) noexcept -> std::pair<iterator, bool> {
        return m_hash_table.insert(std::forward<Pair>(pair));
    }

    auto find(const key_type& key) noexcept -> iterator {
        return m_hash_table.find(key);
    }

    auto find(const key_type& key) const noexcept -> const_iterator {
        return m_hash_table.find(key);
    }

    template<typename K> requires transparent_hash_eq
    auto find(const K& key) noexcept -> iterator {
        return m_hash_table.find(key);
    }

    template<typename K> requires transparent_hash_eq
    auto find(const K& key) const noexcept -> iterator {
        return m_hash_table.find(key);
    }

    auto contains(const key_type& key) const noexcept -> bool {
        return m_hash_table.contains(key);
    }

    template<typename K> requires transparent_hash_eq
    auto contains(const K& key) const noexcept -> bool {
        return m_hash_table.contains(key);
    }

    [[nodiscard]] auto empty() const noexcept -> bool {
        return m_hash_table.empty();
    }

    [[nodiscard]] auto capacity() const noexcept -> size_type {
        return m_hash_table.capacity();
    }

    [[nodiscard]] auto size() const noexcept -> size_type {
        return m_hash_table.size();
    }

    [[nodiscard]] auto begin() noexcept -> iterator {
        return m_hash_table.begin();
    }

    [[nodiscard]] auto begin() const noexcept -> const_iterator {
        return m_hash_table.begin();
    }

    [[nodiscard]] auto cbegin() const noexcept -> const_iterator {
        return m_hash_table.cbegin();
    }

    [[nodiscard]] auto end() noexcept -> iterator {
        return m_hash_table.end();
    }

    [[nodiscard]] auto end() const noexcept -> const_iterator {
        return m_hash_table.end();
    }

    [[nodiscard]] auto cend() const noexcept -> const_iterator {
        return m_hash_table.cend();
    }

private:
    hash_table m_hash_table;
}; // class unordered_map
} // namespace rjh

#endif // #ifndef RJH_UNORDERED_MAP_HPP
