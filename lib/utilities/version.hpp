#pragma once

#include <optional>
#if __has_include("vcs_identifier_gen.hpp")
#include "vcs_identifier_gen.hpp"
constexpr std::optional<const char *> vcsId = VCS_ID;
#else
constexpr std::optional<const char *> vcsId = std::nullopt;
#endif
