#include "Protocol.hpp"
#include "command_line_interpreter.hpp"
#include "serial_port.hpp"
#include <array>

namespace cli = command_line_interpreter;

// --------------------------
// --- define commands ------
// --------------------------
#include "JsonGenerator.hpp"
#include <serial_interface/TaskIterator.hpp>
#include <serial_protocol/ProtocolVersionObject.hpp>
#include <serial_protocol/TaskList.hpp>
#include <serial_protocol/TaskObject.hpp>
#include <string>
#include <tasks/Task.hpp>

using namespace task_tracker_systems;

constexpr int defaultJsonIndent = 4;

// command for info
static const auto info = []() {
    constexpr ProtocolVersionObject version = {.major = 0, .minor = 1, .patch = 0};
    serial_port::cout << toJsonString(version) << std::endl;
};
static const auto infoCmd = cli::makeCommand("info", std::function(info));

// command for list
static const auto list = []() { std::vector<TaskObjectIterator> itList;
std::for_each(std::cbegin(device::tasks), std::cend(device::tasks), [&itList](auto it){
    itList.emplace_back(it);
});
                                        serial_port::cout << toJsonString(itList) << std::endl; };
static const auto listCmd = cli::makeCommand("list", std::function(list));

// command for edit
static const auto edit = [](const TaskId id, const std::basic_string<ProtocolHandler::CharType> label, const Task::Duration::rep duration) {
    try
    {
        auto &task = device::tasks.at(id);
        task.setLabel(label);
        task.setRecordedDuration(std::chrono::seconds(duration));
        const TaskObject taskObject = {.id = id, .label = task.getLabel(), .duration = task.getRecordedDuration().count()};
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

static const std::array<const cli::BaseCommand<char> *, 3> commands = {&listCmd, &editCmd, &infoCmd};

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
