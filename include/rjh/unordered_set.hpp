#ifndef RJH_UNORDERED_SET_HPP
#define RJH_UNORDERED_SET_HPP

#include "detail/hash_table.hpp"

#include <cstddef>
#include <functional>

namespace rjh {
template<typename Key, typename Hash = std::hash<Key>>
class unordered_set {
public:
    using value_type = Key;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using hash_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;

    class iterator final {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = difference_type;
        using value_type = value_type;
        using pointer = value_type*;
        using reference = value_type&;

        using table_iterator = typename detail::hash_table<value_type, hasher>::iterator;

        iterator(table_iterator it) : m_iterator{it} {

        }

        auto operator*() const noexcept -> reference {
            return m_iterator->key;
        }

        auto operator->() const noexcept -> pointer {
            return &m_iterator->key;
        }

        auto operator++() noexcept -> iterator& {
            m_iterator++;
            return *this;
        }

        auto operator++(int) noexcept -> iterator {
            auto temp = *this;
            ++(*this);
            return temp;
        }

        friend auto operator==(const iterator& a, const iterator& b) noexcept -> bool {
            return a.m_iterator == b.m_iterator;
        }

        friend auto operator!=(const iterator& a, const iterator& b) noexcept -> bool {
            return a.m_iterator != b.m_iterator;
        }
    private:
        table_iterator m_iterator;
    };

    class const_iterator final {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = difference_type;
        using value_type = value_type;
        using pointer = value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;

        using table_iterator = typename detail::hash_table<value_type, hasher>::const_iterator;

        const_iterator(table_iterator it) : m_iterator{it} {

        }

        auto operator*() const noexcept -> const_reference {
            return m_iterator->key;
        }

        auto operator->() const noexcept -> pointer {
            return &m_iterator->key;
        }

        auto operator++() noexcept -> iterator& {
            m_iterator++;
            return *this;
        }

        auto operator++(int) noexcept -> iterator {
            auto temp = *this;
            ++(*this);
            return temp;
        }

        friend auto operator==(const iterator& a, const iterator& b) noexcept -> bool {
            return a.m_iterator == b.m_iterator;
        }

        friend auto operator!=(const iterator& a, const iterator& b) noexcept -> bool {
            return a.m_iterator != b.m_iterator;
        }
    private:
        table_iterator m_iterator;
    };

    using iterator = iterator;
    using const_iterator = const_iterator;

    auto add(const_reference key) noexcept -> bool {
        return m_hash_table.add(key);
    }

    template<typename K>
    auto add(const K& key) noexcept -> bool {
        return add(key);
    }

    auto insert(value_type key) noexcept -> void {
        m_hash_table.insert(std::move(key));
    }

    template<typename K>
    auto insert(K&& key) noexcept -> void {
        insert(value_type{std::forward<K>(key)});
    }

    auto remove(const_reference key) noexcept -> bool {
        return m_hash_table.remove(hasher{}(key));
    }

    auto clear() noexcept -> void {
        m_hash_table.clear();
    }

    auto contains(const_reference key) const noexcept -> bool {
        return m_hash_table.contains(hasher{}(key));
    }

    auto empty() const noexcept -> bool {
        return m_hash_table.empty();
    }

    auto capacity() const noexcept -> size_type {
        return m_hash_table.capacity();
    }

    auto size() const noexcept -> size_type {
        return m_hash_table.size();
    }

    auto begin() noexcept -> iterator {
        return m_hash_table.begin();
    }

    auto end() noexcept -> iterator {
        return m_hash_table.end();
    }

private:
    detail::hash_table<value_type, hasher> m_hash_table;
};
} // namespace rjh

#endif // #ifndef RJH_UNORDERED_SET_HPP
