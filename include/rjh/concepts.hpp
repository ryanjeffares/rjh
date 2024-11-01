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

#ifndef RJH_CONCEPTS_HPP
#define RJH_CONCEPTS_HPP

#include <concepts>
#include <type_traits>

namespace rjh::concepts {
template<typename T, typename K>
concept hash_function_object = requires(T object, K key) {
    { object(key) } -> std::same_as<std::size_t>;
};

template<typename T, typename K>
concept key_equal_function_object = requires(T object, K key) {
    { object(key, key) } -> std::same_as<bool>;
};

template<typename T>
concept is_transparent = requires {
    typename T::is_transparent;
};
} // namespace rjh::concepts

#endif // #ifndef RJH_CONCEPTS_HPP
