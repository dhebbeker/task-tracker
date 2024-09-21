#pragma once
#include <cstdint>

namespace task_tracker_systems
{
/**
 * Protocol Version Object
 */
struct ProtocolVersionObject
{
    std::uint8_t major;
    std::uint8_t minor;
    std::uint16_t patch;
};
} // namespace task_tracker_systems
