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

#ifndef RJH_HASH_TABLE_HPP
#define RJH_HASH_TABLE_HPP

#include "../concepts.hpp"

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <functional>
#include <iterator>
#include <optional>
#include <vector>

namespace rjh::detail {
template<
    typename Key,
    concepts::hash_function_object<Key> Hash = std::hash<Key>,
    concepts::key_equal_function_object<Key> KeyEqual = std::equal_to<Key>
>
class hash_table final {
public:
    using value_type = Key;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using hash_type = std::size_t;
    using key_equal = KeyEqual;
    using reference = value_type&;
    using const_reference = const value_type&;

    struct bucket {
        value_type key{};
        hash_type hash{};
        bool occupied{false};
        size_type distance{0};
    };

    template<typename T>
    class raw_iterator final {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = difference_type;
        using value_type = T;
        using pointer = value_type*;
        using reference = value_type&;

        raw_iterator(pointer ptr, pointer end)
            : m_pointer{ptr}
            , m_end{end} {

        }

        auto operator*() const noexcept -> reference {
            return *m_pointer;
        }

        auto operator->() const noexcept -> pointer {
            return m_pointer;
        }

        auto operator++() noexcept -> raw_iterator& {
            do {
                m_pointer++;
            } while (m_pointer != m_end && !m_pointer->occupied);
            return *this;
        }

        auto operator++(int) noexcept -> raw_iterator {
            auto temp = *this;
            do {
                ++(*this);
            } while (m_pointer != m_end && !m_pointer->occupied);
            return temp;
        }

        friend auto operator==(const raw_iterator& a, const raw_iterator& b) noexcept -> bool {
            return a.m_pointer == b.m_pointer;
        }

        friend auto operator!=(const raw_iterator& a, const raw_iterator& b) noexcept -> bool {
            return a.m_pointer != b.m_pointer;
        }

    private:
        pointer m_pointer, m_end;
    };

    using iterator = raw_iterator<bucket>;
    using const_iterator = raw_iterator<const bucket>;

    hash_table() : m_size{0}, m_buckets{s_initial_capacity} {

    }

    ~hash_table() = default;

    hash_table(const hash_table&) = default;
    hash_table(hash_table&&) = default;
    hash_table& operator=(const hash_table&) = default;
    hash_table& operator=(hash_table&&) = default;

    auto insert(const_reference key) noexcept -> std::pair<iterator, bool> {
        if (find(key) != end()) {
            return {end(), false};
        }

        const auto hash = m_hasher(key);
        return insert({
            .key = key,
            .hash = hash,
            .occupied = true,
        });
    }

    auto insert(value_type&& key) noexcept -> std::pair<iterator, bool> {
        if (find(key) != end()) {
            return {end(), false};
        }

        const auto hash = m_hasher(key);
        return insert({
            .key = std::move(key),
            .hash = hash,
            .occupied = true,
        });
    }

    template<typename K> requires std::constructible_from<value_type, K&&>
    auto insert(K&& key) noexcept -> std::pair<iterator, bool> {
        return insert(std::forward<K>(key));
    }

    auto find(const_reference key) noexcept -> iterator {
        const auto hash = m_hasher(key);
        auto index = hash % capacity();

        while (m_buckets[index].occupied) {
            const auto& entry = m_buckets[index];
            if (m_key_equal(entry.key, key)) {
                return iterator{
                    m_buckets.begin().operator->() + index,
                    m_buckets.end().operator->()
                };
            }
            index = (index + 1) % capacity();
        }

        return end();
    }

    auto find(const_reference key) const noexcept -> const_iterator {
        const auto hash = m_hasher(key);
        auto index = hash % capacity();

        while (m_buckets[index].occupied) {
            const auto& entry = m_buckets[index];
            if (m_key_equal(entry.key, key.key)) {
                return const_iterator{
                    m_buckets.begin().operator->() + index,
                    m_buckets.end().operator->()
                };
            }
            index = (index + 1) % capacity();
        }

        return end();
    }

    template<typename K> requires concepts::is_transparent<hasher> && concepts::is_transparent<key_equal>
    auto find(const K& key) noexcept -> iterator {
        const auto hash = m_hasher(key);
        auto index = hash % capacity();

        while (m_buckets[index].occupied) {
            const auto& entry = m_buckets[index];
            if (m_key_equal(entry.key, key)) {
                return iterator{
                    m_buckets.begin().operator->() + index,
                    m_buckets.end().operator->(),
                };
            }
            index = (index + 1) % capacity();
        }

        return end();
    }

    template<typename K> requires concepts::is_transparent<hasher> && concepts::is_transparent<key_equal>
    auto find(const K& key) const noexcept -> const_iterator {
        const auto hash = m_hasher(key);
        auto index = hash % capacity();

        while (m_buckets[index].occupied) {
            const auto& entry = m_buckets[index];
            if (m_key_equal(entry.key, key)) {
                return const_iterator{
                    m_buckets.begin().operator->() + index,
                    m_buckets.end().operator->(),
                };
            }
            index = (index + 1) % capacity();
        }

        return end();
    }

    auto contains(const_reference key) const noexcept -> bool {
        return find(key) != end();
    }

    template<typename K> requires concepts::is_transparent<hasher> && concepts::is_transparent<key_equal>
    auto contains(const K& key) const noexcept -> bool {
        return find(key) != end();
    }

    auto remove(const_reference key) noexcept -> bool {
        auto index = m_hasher(key) % capacity();

        while (m_buckets[index].occupied) {
            auto& entry = m_buckets[index];
            if (m_key_equal(entry.key, key)) {
                entry = {};
                index = (index + 1) % capacity();
                while (m_buckets[index].occupied && m_buckets[index].distance > 0) {
                    std::swap(entry, m_buckets[index]);
                    index = (index + 1) % capacity();
                }
                m_size--;
                return true;
            }
            index = (index + 1) % capacity();
        }

        return false;
    }

    template<typename K> requires concepts::is_transparent<hasher> && concepts::is_transparent<key_equal>
    auto remove(K&& key) noexcept -> bool {
        auto index = m_hasher(key) % capacity();

        while (m_buckets[index].occupied) {
            auto& entry = m_buckets[index];
            if (m_key_equal(entry.key, key)) {
                entry = {};
                index = (index + 1) % capacity();
                while (m_buckets[index].occupied && m_buckets[index].distance > 0) {
                    std::swap(entry, m_buckets[index]);
                    index = (index + 1) % capacity();
                }
                m_size--;
                return true;
            }
            index = (index + 1) % capacity();
        }

        return false;
    }

    auto clear() noexcept -> void {
        std::fill(m_buckets.begin(), m_buckets.end(), bucket{});
        m_size = 0;
    }

    auto empty() const noexcept -> bool {
        return size() == 0;
    }

    auto capacity() const noexcept -> size_type {
        return m_buckets.size();
    }

    auto size() const noexcept -> size_type {
        return m_size;
    }

    auto begin() noexcept -> iterator {
        if (empty()) {
            return end();
        }

        iterator it{
            m_buckets.begin().operator->(),
            m_buckets.end().operator->()
        };

        if (!it->occupied) {
            it++;
        }

        return it;
    }

    auto begin() const noexcept -> const_iterator {
        if (empty()) {
            return end();
        }

        const_iterator it{
            m_buckets.begin().operator->(),
            m_buckets.end().operator->()
        };

        if (!it->occupied) {
            it++;
        }

        return it;
    }

    auto cbegin() const noexcept -> const_iterator {
        return begin();
    }

    auto end() noexcept -> iterator {
        return iterator{
            m_buckets.end().operator->(),
            m_buckets.end().operator->(),
        };
    }

    auto end() const noexcept -> const_iterator {
        return const_iterator{
            m_buckets.end().operator->(),
            m_buckets.end().operator->(),
        };
    }

    auto cend() const noexcept -> const_iterator {
        return end();
    }

private:
    auto insert(bucket&& entry) noexcept -> std::pair<iterator, bool> {
        check_load();
        auto index = entry.hash % capacity();

        while (m_buckets[index].occupied) {
            auto& old = m_buckets[index];
            if (entry.distance > old.distance) {
                std::swap(entry, old);
            }
            entry.distance++;
            index = (index + 1) % capacity();
        }

        m_buckets[index] = std::move(entry);
        m_size++;
        return {iterator{m_buckets.data() + index, m_buckets.end().operator->()}, true};
    }

    auto check_load() noexcept -> void {
        if (static_cast<float>(size()) / static_cast<float>(capacity()) >= s_grow_factor) {
            grow_and_rehash();
        }
    }

    auto grow_and_rehash() noexcept -> void {
        std::vector<bucket> buckets;
        buckets.reserve(m_size);
        for (auto& bucket : m_buckets) {
            if (bucket.occupied) {
                bucket.distance = 0;
                buckets.emplace_back(std::move(bucket));
            }
        }

        std::fill(m_buckets.begin(), m_buckets.end(), bucket{});
        m_buckets.resize(capacity() * 2);

        for (auto& bucket : buckets) {
            auto index = bucket.hash % capacity();

            while (m_buckets[index].occupied) {
                auto& old = m_buckets[index];

                if (bucket.distance > old.distance) {
                    std::swap(bucket, old);
                }

                bucket.distance++;
                index = (index + 1) % capacity();
            }

            m_buckets[index] = std::move(bucket);
        }
    }

    static constexpr size_type s_initial_capacity = 8;
    static constexpr float s_grow_factor = 0.75f;

    size_type m_size;
    std::vector<bucket> m_buckets;

    hasher m_hasher;
    key_equal m_key_equal;
};
} // namespace rjh::detail


#endif // #ifndef RJH_HASH_TABLE_HPP
