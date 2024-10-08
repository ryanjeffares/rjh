#ifndef RJH_UNORDERED_MAP_HPP
#define RJH_UNORDERED_MAP_HPP

#include "detail/hash_table.hpp"

#include <cstddef>
#include <functional>
#include <iterator>
#include <utility>

namespace rjh {
template<typename T>
using ref = std::reference_wrapper<T>;

template<typename Key, typename Value, typename Hash = std::hash<Key>>
class unordered_map {
public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = std::pair<key_type, mapped_type>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using reference = value_type&;
    using const_reference = const value_type&;

private:
    struct pair_hash {
        [[nodiscard]]
        auto operator()(const value_type& pair) const noexcept -> std::size_t {
            return hasher{}(pair.first);
        }
    };

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

    using iterator = raw_iterator<value_type, typename detail::hash_table<value_type, pair_hash>::iterator>;
    using const_iterator = raw_iterator<const value_type, typename detail::hash_table<value_type, pair_hash>::const_iterator>;

    auto add(const_reference pair) noexcept -> bool {
        return m_hash_table.add(pair);
    }

    auto add(const key_type& key, const value_type& value) noexcept -> bool {
        return add({key, value});
    }

    template<typename K, typename V>
    auto add(const K& key, const V& value) noexcept -> bool {
        return add({key, value});
    }

    auto insert(value_type pair) noexcept -> void {
        m_hash_table.insert(std::move(pair));
    }

    auto insert(key_type key, mapped_type value) noexcept -> void {
        insert({std::move(key), std::move(value)});
    }

    template<typename K, typename V>
    auto insert(K&& key, V&& value) noexcept -> void {
        insert({std::forward<K>(key), std::forward<V>(value)});
    }

    auto remove(const key_type& key) noexcept -> bool {
        return m_hash_table.remove(hasher{}(key));
    }

    auto find(const key_type& key) noexcept -> std::optional<ref<mapped_type>> {
        auto entry = m_hash_table.find(hasher{}(key));
        if (entry) {
            return entry->get().second;
        } else {
            return {};
        }
    }

    auto find(const key_type& key) const noexcept -> std::optional<ref<const mapped_type>> {
        auto entry = m_hash_table.find(hasher{}(key));
        if (entry) {
            return entry->get().second;
        } else {
            return {};
        }
    }

    auto operator[](const key_type& key) noexcept -> mapped_type& {
        if (!contains(key)) {
            insert(key, mapped_type{});
        }

        return find(key)->get();
    }

    auto operator[](const key_type& key) const noexcept -> const mapped_type& {
        if (!contains(key)) {
            insert(key, mapped_type{});
        }

        return find(key)->get();
    }

    auto clear() noexcept -> void {
        m_hash_table.clear();
    }

    auto contains(const Key& key) const noexcept -> bool {
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
    detail::hash_table<value_type, pair_hash> m_hash_table;
}; // class unordered_map
} // namespace rjh

#endif // #ifndef RJH_UNORDERED_MAP_HPP
