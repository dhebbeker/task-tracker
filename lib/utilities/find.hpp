#pragma once

#include <cstddef>
#include <optional>
#include <utility>

template <typename T1, typename T2, std::size_t N>
constexpr std::optional<T2> find_second_for_first(const std::pair<T1, T2> (&arr)[N], const T1 &first_value)
{
    for (const auto &pair : arr)
    {
        if (pair.first == first_value)
        {
            return pair.second;
        }
    }
    return std::nullopt;
}

template <typename T1, typename T2, std::size_t N>
constexpr std::optional<std::size_t> find_index_for_first(const std::pair<T1, T2> (&arr)[N], const T1 &first_value)
{
    for (std::size_t i = 0; i < N; ++i)
    {
        if (arr[i].first == first_value)
        {
            return i;
        }
    }
    return std::nullopt;
}
