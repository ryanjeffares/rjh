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

#ifndef RJH_UNORDERED_SET_HPP
#define RJH_UNORDERED_SET_HPP

#include "detail/hash_table.hpp"

#include <cstddef>
#include <functional>
#include <iterator>
#include <utility>

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

    template<typename T, typename It>
    class raw_iterator final {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type  = difference_type;
        using value_type = T;
        using pointer = const value_type*;
        using reference = const value_type&;
        using table_iterator = It;

        raw_iterator(table_iterator it) : m_iterator{it} {

        }

        auto operator*() const noexcept -> reference {
            return m_iterator->key;
        }

        auto operator->() const noexcept -> pointer {
            return &m_iterator->key;
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

    using iterator = raw_iterator<value_type, typename detail::hash_table<value_type, hasher>::iterator>;
    using const_iterator = raw_iterator<value_type, typename detail::hash_table<value_type, hasher>::const_iterator>;

    auto clear() noexcept -> void {
        m_hash_table.clear();
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

    auto begin() const noexcept -> const_iterator {
        return m_hash_table.begin();
    }

    auto cbegin() const noexcept -> const_iterator {
        return m_hash_table.cbegin();
    }

    auto end() noexcept -> iterator {
        return m_hash_table.end();
    }

    auto end() const noexcept -> const_iterator {
        return m_hash_table.end();
    }

    auto cend() const noexcept -> const_iterator {
        return m_hash_table.cend();
    }

private:
    detail::hash_table<value_type, hasher> m_hash_table;
};
} // namespace rjh

#endif // #ifndef RJH_UNORDERED_SET_HPP
