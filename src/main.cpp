/**
 * \file
 */

#include <chrono>
#include <serial_interface/Protocol.hpp>
#include <serial_interface/serial_port.hpp>
#include <tasks/Task.hpp>
#include <thread>
#include <user_interaction/Menu.hpp>
#include <user_interaction/Presenter.hpp>
#include <user_interaction/ProcessHmiInputs.hpp>
#include <user_interaction/display_factory_interface.hpp>
#include <user_interaction/keypad_factory_interface.hpp>
#include <user_interaction/statusindicators_factory_interface.hpp>

void setup()
{
    serial_port::initialize();
    serial_port::cout << "\x1b[20h"; // Tell the terminal to use CR/LF for newlines instead of just CR.
    static constexpr const auto programIdentificationString = __FILE__ " compiled at " __DATE__ " " __TIME__;
    serial_port::cout << std::endl
                      << " begin program '" << programIdentificationString << std::endl;
    serial_port::setCallbackForLineReception([](const serial_port::String &commandLine) {
        ProtocolHandler::execute(commandLine.c_str());
    });
}

void loop()
{
    static Menu singleMenu(board::getDisplay());
    static Presenter presenter(singleMenu, board::getStatusIndicators());
    static auto &keypad = board::getKeypad();
    static ProcessHmiInputs processHmiInputs(presenter, keypad);

    for (auto task : device::tasks)
    {
        serial_port::cout << task.second.getLabel() << " : " << std::boolalpha << task.second.isRunning()
                          << std::noboolalpha << "   with " << task.second.getRecordedDuration().count() << " s" << std::endl;
    }
    serial_port::cout << "_\r" << std::endl;
    serial_port::cout << "Buttons: \t";
    for (std::size_t i = static_cast<std::size_t>(KeyId::LEFT); i <= static_cast<std::size_t>(KeyId::TASK4); i++)
    {
        serial_port::cout << std::boolalpha << keypad.isKeyPressed(static_cast<KeyId>(i)) << std::noboolalpha << " \t|";
    }
    serial_port::cout << std::endl;

    std::this_thread::yield();
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);

    presenter.loop();
}
