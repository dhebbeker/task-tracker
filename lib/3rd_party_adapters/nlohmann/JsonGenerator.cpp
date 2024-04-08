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
    jsonObject = {object.duration, object.label, object.id};
}
} // namespace task_tracker_systems

template <>
std::string toJsonString<task_tracker_systems::TaskObject>(const task_tracker_systems::TaskObject &object)
{
    auto jsonObject = nlohmann::json::object();
    to_json(jsonObject, object);
    return jsonObject.dump(defaultJsonIndent);
}

typedef struct
{
    unsigned int id;
    std::string label;
    std::chrono::seconds::rep duration;
} my_struct_t;

void to_json(nlohmann::json &j, const my_struct_t &ms)
{
    j = {ms.id, ms.label, ms.duration};
}

template <>
std::string toJsonString<task_tracker_systems::TaskList>(const task_tracker_systems::TaskList &object)
{
    // nlohmann::json jsonObject = {{"array", object.list}};
    // return jsonObject.dump(defaultJsonIndent);

    my_struct_t struct_array[3];

    task_tracker_systems::TaskObject array[3];

    nlohmann::json result = {{"array", array}};
    return "";
}

template <>
std::string toJsonString<task_tracker_systems::DeletedTaskObject>(const task_tracker_systems::DeletedTaskObject &object)
{
    auto jsonObject = nlohmann::json::object();
    jsonObject["id"] = object.id;
    return jsonObject.dump(defaultJsonIndent);
}
