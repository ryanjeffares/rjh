#ifndef RJH_UNORDERED_MAP_HPP
#define RJH_UNORDERED_MAP_HPP

#include "detail/hash_table.hpp"

#include <functional>
#include <utility>

namespace rjh {
template<typename First, typename Second>
struct pair {
    First first;
    Second second;
};

template<typename Key, typename Value, typename Hash = std::hash<Key>>
class unordered_map {
public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = pair<key_type, mapped_type>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using reference = value_type&;
    using const_reference = const value_type&;

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

    auto find(const key_type& key) noexcept -> std::optional<std::reference_wrapper<mapped_type>> {
        auto entry = m_hash_table.find(hasher{}(key));
        if (entry) {
            return entry->get().second;
        } else {
            return {};
        }
    }

    auto find(const key_type& key) const noexcept -> std::optional<std::reference_wrapper<const mapped_type>> {
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

private:
    struct hash {
        [[nodiscard]]
        auto operator()(const value_type& pair) const noexcept -> std::size_t {
            return hasher{}(pair.first);
        }
    };

    detail::hash_table<value_type, hash> m_hash_table;
}; // class unordered_map
} // namespace rjh

#endif // #ifndef RJH_UNORDERED_MAP_HPP
