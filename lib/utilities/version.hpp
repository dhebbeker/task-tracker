#pragma once

#include <optional>
#if __has_include("vcs_identifier_gen.h")
#include "vcs_identifier_gen.h"
constexpr std::optional<const char *> vcsId = VCS_ID;
#else
constexpr std::optional<const char *> vcsId = std::nullopt;
#endif
