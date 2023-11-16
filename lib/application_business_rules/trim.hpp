#pragma once
#include <iterator>
#include <locale>

template <typename SequenceT>
SequenceT &ltrim(SequenceT &input, const std::locale &locale = std::locale())
{
    const auto begin_trimmed = std::find_if(
        std::begin(input),
        std::end(input),
        [&](const auto ch) { return !std::isspace(ch, locale); });
    input = SequenceT(begin_trimmed, std::end(input));
    return input;
}

template <typename SequenceT>
SequenceT &rtrim(SequenceT &input, const std::locale &locale = std::locale())
{
    const auto end_trimmed = std::find_if(
        std::rbegin(input),
        std::rend(input),
        [&](const auto ch) { return !std::isspace(ch, locale); });
    input = SequenceT(std::begin(input), end_trimmed.base());
    return input;
}

template <typename SequenceT>
SequenceT &trim(SequenceT &input, const std::locale &locale = std::locale())
{
    return rtrim(ltrim(input));
}