#ifndef RJH_UNORDERED_MAP_HPP
#define RJH_UNORDERED_MAP_HPP

#include "detail/hash_table.hpp"

#include <concepts>
#include <functional>
#include <unordered_map>
#include <utility>

namespace rjh {
template<typename Key, typename Value, typename Hash = std::hash<Key>>
class unordered_map {
public:
    struct pair {
        Key first;
        Value second;
    };

    using size_type = std::size_t;
    using value_type = pair;

    auto add(const value_type& pair) noexcept -> bool {
        return m_hash_table.add(pair);
    }

    auto add(const Key& key, const Value& value) noexcept -> bool {
        return add({key, value});
    }

    template<typename K, typename V>
    auto add(const K& key, const V& value) noexcept -> bool {
        return add({key, value});
    }

    auto insert(value_type pair) noexcept -> void {
        m_hash_table.insert(std::move(pair));
    }

    auto insert(Key key, Value value) noexcept -> void {
        insert({std::move(key), std::move(value)});
    }

    template<typename K, typename V>
    auto insert(K key, V value) noexcept -> void {
        insert({std::move(key), std::move(value)});
    }

    auto remove(const Key& key) noexcept -> bool {
        return m_hash_table.remove(Hash{}(key));
    }

    auto find(const Key& key) noexcept -> std::optional<std::reference_wrapper<Value>> {
        auto entry = m_hash_table.find(Hash{}(key));
        if (entry) {
            return entry->get().second;
        } else {
            return {};
        }
    }

    auto find(const Key& key) const noexcept -> std::optional<std::reference_wrapper<const Value>> {
        auto entry = m_hash_table.find(Hash{}(key));
        if (entry) {
            return entry->get().second;
        } else {
            return {};
        }
    }

    auto operator[](const Key& key) noexcept -> Value& {
        if (!contains(key)) {
            insert(key, Value{});
        }

        return find(key)->get();
    }

    auto operator[](const Key& key) const noexcept -> const Value& {
        if (!contains(key)) {
            insert(key, Value{});
        }

        return find(key)->get();
    }

    auto clear() noexcept -> void {
        m_hash_table.clear();
    }

    auto contains(const Key& key) const noexcept -> bool {
        return m_hash_table.contains(Hash{}(key));
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

private:
    struct hash {
        [[nodiscard]] auto operator()(const value_type& pair) const noexcept -> std::size_t {
            return Hash{}(pair.first);
        }
    };

    detail::hash_table<value_type, hash> m_hash_table;
}; // class unordered_map
} // namespace rjh

#endif // #ifndef RJH_UNORDERED_MAP_HPP
