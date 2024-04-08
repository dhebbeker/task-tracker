#include <nlohmann/json.hpp>
#include <serial_interface/JsonGenerator.hpp>
#include <serial_protocol/DeletedTaskObject.hpp>
#include <serial_protocol/ProtocolVersionObject.hpp>
#include <serial_protocol/TaskList.hpp>
#include <serial_protocol/TaskObject.hpp>

constexpr int defaultJsonIndent = 4;

template <>
std::string toJsonString<task_tracker_systems::ProtocolVersionObject>(const task_tracker_systems::ProtocolVersionObject &object)
{
    auto jsonObject = nlohmann::json::object();
    jsonObject["major"] = object.major;
    jsonObject["minor"] = object.minor;
    jsonObject["patch"] = object.patch;
    return jsonObject.dump(defaultJsonIndent);
}

namespace task_tracker_systems
{
void to_json(nlohmann::json &jsonObject, const task_tracker_systems::TaskObject &object)
{
    jsonObject["id"] = object.id;
    jsonObject["label"] = object.label;
    jsonObject["duration"] = object.duration;
}
} // namespace task_tracker_systems

template <>
std::string toJsonString<task_tracker_systems::TaskObject>(const task_tracker_systems::TaskObject &object)
{
    auto jsonObject = nlohmann::json::object();
    to_json(jsonObject, object);
    return jsonObject.dump(defaultJsonIndent);
}

template <>
std::string toJsonString<task_tracker_systems::TaskList>(const task_tracker_systems::TaskList &object)
{
    nlohmann::json jsonObject(object.list);
    return jsonObject.dump(defaultJsonIndent);
}

template <>
std::string toJsonString<task_tracker_systems::DeletedTaskObject>(const task_tracker_systems::DeletedTaskObject &object)
{
    auto jsonObject = nlohmann::json::object();
    jsonObject["id"] = object.id;
    return jsonObject.dump(defaultJsonIndent);
}
