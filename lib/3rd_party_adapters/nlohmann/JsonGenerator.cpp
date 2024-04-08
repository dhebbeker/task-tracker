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

void to_json(nlohmann::json &jsonObject, const task_tracker_systems::TaskObject &object)
{
    jsonObject["duration"] = object.duration;
    jsonObject["id"] = object.id;
    jsonObject["label"] = object.label;
}

template <>
std::string toJsonString<task_tracker_systems::TaskObject>(const task_tracker_systems::TaskObject &object)
{
    auto jsonObject = nlohmann::json::object();
    to_json(jsonObject, object);
    return jsonObject.dump(defaultJsonIndent);
}

typedef struct
{
    uint8_t a;
    bool c;
    float e;
} my_struct_t;

void to_json(nlohmann::json &j, const my_struct_t &ms)
{
    j = {ms.a, ms.c, ms.e};
}

template <>
std::string toJsonString<task_tracker_systems::TaskList>(const task_tracker_systems::TaskList &object)
{
    // nlohmann::json jsonObject = {{"array", object.list}};
    // return jsonObject.dump(defaultJsonIndent);

    my_struct_t struct_array[] = {
        {1, true, 1.10},
        {2, false, 2.20},
        {3, true, 3.30}};

    nlohmann::json result = {{"array", struct_array}};
    return "";
}

template <>
std::string toJsonString<task_tracker_systems::DeletedTaskObject>(const task_tracker_systems::DeletedTaskObject &object)
{
    auto jsonObject = nlohmann::json::object();
    jsonObject["id"] = object.id;
    return jsonObject.dump(defaultJsonIndent);
}
