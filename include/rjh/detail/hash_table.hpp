#ifndef RJH_HASH_TABLE_HPP
#define RJH_HASH_TABLE_HPP

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <functional>
#include <optional>
#include <vector>

namespace rjh::detail {
template<typename T, typename K>
concept Hasher = requires(T value, K key) {
    { value(key) } -> std::same_as<std::size_t>;
};

template<typename Key, Hasher<Key> Hash = std::hash<Key>>
class hash_table final {
public:
    using size_type = std::size_t;
    using hash_type = std::size_t;

    hash_table() : m_size{0}, m_buckets{s_initial_capacity} {

    }

    ~hash_table() = default;

    hash_table(const hash_table&) = default;
    hash_table(hash_table&&) = default;
    hash_table& operator=(const hash_table&) = default;
    hash_table& operator=(hash_table&&) = default;

    auto add(const Key& key) noexcept -> bool {
        const auto hash = Hash{}(key);
        auto index = hash % capacity();

        bucket entry {
            .key = key,
            .hash = hash,
            .occupied = true,
        };

        while (m_buckets[index].occupied) {
            auto& old = m_buckets[index];
            if (entry.hash == old.hash) {
                return false;
            }

            if (entry.distance > old.distance) {
                std::swap(entry, old);
            }

            entry.distance++;
            index = (index + 1) % capacity();
        }

        m_buckets[index] = std::move(entry);
        m_size++;
        check_load();

        return true;
    }

    auto insert(Key key) noexcept -> void {
        const auto hash = Hash{}(key);
        auto index = hash % capacity();

        bucket entry {
            .key = std::move(key),
            .hash = hash,
            .occupied = true,
        };

        while (m_buckets[index].occupied) {
            auto& old = m_buckets[index];
            if (entry.hash == old.hash) {
                std::swap(entry, old);
                return;
            }

            if (entry.distance > old.distance) {
                std::swap(entry, old);
            }

            entry.distance++;
            index = (index + 1) % capacity();
        }

        m_buckets[index] = std::move(entry);
        m_size++;
        check_load();
    }

    auto remove(hash_type hash) noexcept -> bool {
        auto index = hash % capacity();

        while (m_buckets[index].occupied) {
            auto& entry = m_buckets[index];

            if (entry.hash == hash) {
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

    auto contains(hash_type hash) const noexcept -> bool {
        auto index = hash % capacity();

        while (m_buckets[index].occupied) {
            const auto& entry = m_buckets[index];
            if (entry.hash == hash) {
                return true;
            }
            index = (index + 1) % capacity();
        }

        return false;
    }

    auto find(hash_type hash) noexcept -> std::optional<std::reference_wrapper<Key>> {
        auto index = hash % capacity();

        while (m_buckets[index].occupied) {
            auto& entry = m_buckets[index];
            if (entry.hash == hash) {
                return m_buckets[index].key;
            }
            index = (index + 1) % capacity();
        }

        return {};
    }

    auto find(hash_type hash) const noexcept -> std::optional<std::reference_wrapper<const Key>> {
        auto index = hash % capacity();

        while (m_buckets[index].occupied) {
            auto& entry = m_buckets[index];
            if (entry.hash == hash) {
                return m_buckets[index].key;
            }
            index = (index + 1) % capacity();
        }

        return {};
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

private:
    struct bucket {
        Key key{};
        hash_type hash{};
        bool occupied{false};
        size_type distance{0};
    };

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
};
}

#endif // #ifndef RJH_HASH_TABLE_HPP
