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

private:
    detail::hash_table<value_type, hasher> m_hash_table;
};
} // namespace rjh

#endif // #ifndef RJH_UNORDERED_SET_HPP
