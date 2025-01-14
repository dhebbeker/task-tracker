#include "Protocol.hpp"
#include "command_line_interpreter.hpp"
#include "serial_port.hpp"
#include <array>

namespace cli = command_line_interpreter;

// --------------------------
// --- define commands ------
// --------------------------
#include "JsonGenerator.hpp"
#include <serial_protocol/DeletedTaskObject.hpp>
#include <serial_protocol/ProtocolVersionObject.hpp>
#include <serial_protocol/TaskList.hpp>
#include <serial_protocol/TaskObject.hpp>
#include <string>
#include <tasks/Task.hpp>

using namespace task_tracker_systems;

// command for info
static const auto info = []() {
    constexpr ProtocolVersionObject version = {.major = 0, .minor = 1, .patch = 0};
    serial_port::cout << toJsonString(version) << std::endl;
};
static const auto infoCmd = cli::makeCommand("info", std::function(info));

// command for list
static const auto list = []() { serial_port::cout << toJsonString(device::tasks) << std::endl; };
static const auto listCmd = cli::makeCommand("list", std::function(list));

// command for edit
static const auto edit = [](const TaskId id, const std::basic_string<ProtocolHandler::CharType> label, const Task::Duration::rep duration) {
    try
    {
        auto &task = device::tasks.at(id);
        task.setLabel(label);
        task.setRecordedDuration(std::chrono::seconds(duration));
        const TaskObject taskObject = {.id = id, .label = task.getLabel(), .duration = task.getLastRecordedDuration().count()};
        serial_port::cout << toJsonString(taskObject) << std::endl;
    }
    catch (std::out_of_range &e)
    {
        serial_port::cout << "ERROR: Task not found." << std::endl;
    }
};
static const cli::Option<TaskId> id = {.labels = {"--id"}, .defaultValue = 0};
static const cli::Option<std::basic_string<ProtocolHandler::CharType>> label = {.labels = {"--name"}, .defaultValue = "foo"};
static const cli::Option<Task::Duration::rep> duration = {.labels = {"--duration"}, .defaultValue = 0};
static const auto editCmd = cli::makeCommand("edit", std::function(edit), std::make_tuple(&id, &label, &duration));

// command for create/add
static const auto add = [](const TaskId id, const std::basic_string<ProtocolHandler::CharType> label, const Task::Duration::rep duration) {
    try
    {
        const auto &[element, created] = device::tasks.try_emplace(id, label, std::chrono::seconds(duration));
        const auto &task = element->second;
        const TaskObject taskObject = {.id = element->first, .label = task.getLabel(), .duration = task.getLastRecordedDuration().count()};
        serial_port::cout << toJsonString(taskObject) << std::endl;
        if (!created)
        {
            serial_port::cout << "ERROR: Task with the specified ID already exists." << std::endl;
        }
    }
    catch (std::out_of_range &e)
    {
        serial_port::cout << "ERROR: Task not found." << std::endl;
    }
};
static const auto addCmd = cli::makeCommand("add", std::function(add), std::make_tuple(&id, &label, &duration));

// command for delete/remove
static const auto del = [](const TaskId id) {
    const bool deleted = device::tasks.erase(id) > 0;
    const DeletedTaskObject taskObject{.id = id};
    serial_port::cout << toJsonString(taskObject) << std::endl;
    if (!deleted)
    {
        serial_port::cout << "ERROR: No task deleted." << std::endl;
    }
};
static const auto delCmd = cli::makeCommand("delete", std::function(del), std::make_tuple(&id));

static const std::array<const cli::BaseCommand<char> *, 5> commands = {&listCmd, &editCmd, &infoCmd, &addCmd, &delCmd};

bool ProtocolHandler::execute(const CharType *const commandLine)
{
    for (const auto command : commands)
    {
        try
        {
            const bool executed = command->execute(commandLine);
            if (executed)
            {
                return executed;
            }
        }
        catch (const std::runtime_error &e)
        {
            serial_port::cout << e.what() << std::endl;
            serial_port::cout << command->generateHelpMessage() << std::endl;
        }
    }

    // if we arrive here, no command has been executed
    if (!commands.empty())
    {
        serial_port::cout << "No command has been executed!" << std::endl
                          << "Possible commands are:" << std::endl;
        for (const auto command : commands)
        {
            serial_port::cout << "\t " << command->commandName << std::endl;
        }
    }
    return false;
}
