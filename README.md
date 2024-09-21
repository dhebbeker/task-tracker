# Task Tracker

## About

This repository contains the software and hardware sources for a task time recording device.

The purpose of this device is to provide a tool to track time which is used on tasks, in a handy way.

The user shall be able to start and stop the time recording for tasks using a simple press.
Implementation as a device - in contrast to pure software - shall provide the user a distraction free and permanently visible, dedicated interface.
The system shall offer to track a flexible number of tasks.
For quick access, a small number of tasks can be defined as favorites. 
Tasks shall be identifiable using at least a short label.
It is not a goal to display a full task description to the user.

In particular the objectives of the device are:

 - quick interaction with an easy human-machine interface
 - permanent feedback on which task is active at the moment
 - provide digital interface to recorded times (TBD: through WiFi, Bluetooth, wired serial interface, ...)

## Deployment

### Generate the documentation

Documentation may be generated with [Doxygen](https://www.doxygen.org/).
Simply call in the root directory of this repository:

    doxygen

The HTML documentation will be written to [`doc/html/index.xhtml`](doc/html/index.xhtml).

### Build and Upload the Software

The project consists of a software and a hardware device.
The target system to run the software on is the device developed in this project.
Alternatively the software may be executed in a simulation environment.

This project uses [PlatformIO](https://platformio.org/) as build system and package manager.
PlatformIO may be used via a GUI (PlatformIO IDE) or command line interface (PlatformIO Core). The project configuration ([`platformio.ini`](platformio.ini)) is part of this repository.

In order to use the software (some call it "firmware"), the following steps are required:

1. Build (the default configuration of) the project.
   For example via:
   ```
   platformio run
   ```
2. [Connect](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/establish-serial-connection.html) the device with the host system (where you build the software).
3. Upload the built binary to the device.
   For example via:
   ```
   platformio run --target upload
   ```

Please refer to the [PIO documentation](https://docs.platformio.org/) for detailed instructions.

### Simulate

The Visual Studio Code extension [Wokwi Simulator](https://marketplace.visualstudio.com/items?itemName=wokwi.wokwi-vscode) may be used to simulate the software on the device.
The project configuration for the simulator is provided as [`wokwi.toml`](wokwi_files/wokwi.toml).

Note, that you must build the software (using the default build configuration) before it can be simulated.

Please refer to the documentation of the simulator for instructions on how to simulate the device.

#### Using serial port with the simulator

Before version 2.1.0 the Wokwi extension provided an integrated serial monitor.
Since version 2.1.0 it uses VS Code's built-in terminal instead.
That terminal does not provide a prompt for entering, reviewing and modifying a message before sending it.
See also [this issue report](https://github.com/wokwi/wokwi-features/issues/698).

The simulator also [provides a RFC2217 TCP server](https://docs.wokwi.com/vscode/project-config#serial-port-forwarding) as an alternative.
Serial port forwarding is enabled for the simulator per configuration.
This can be used to connect via a Telnet client to the serial port of the simulated device.

For example one may use a [PuTTY](https://www.putty.org/) SSH Client with the following settings:

- Host Name: `localhost`
- Port: `4000`
- Connection type: Telnet

For testing with the Task Tracker App (or any other programm which supports a serial interface),
one can forward the RFC2217 TCP connection to a pseudo terminal.
For example using [socat](http://www.dest-unreach.org/socat/):

    socat -d -d pty,raw,echo=0,link=/tmp/tcp_tty tcp:localhost:4000

Now you can connect to the serial terminal using these settings:

- path to serial line: `/tmp/tcp_tty`
- speed: `115200` baud
- data bits: `8`
- stop bits: `1`
- parity: none
- flow control: none

### Debugging

For debugging you need to use the `debug` [configuration](https://docs.platformio.org/en/latest/projectconf/build_configurations.html#build-configurations) for building. One can achieve this via:

- command line: `pio run --target debug`
- PlatformIO IDE: select as project task 'Advanced' → 'Pre-Debug'

#### Interpreting runtime exceptions

The ESP32 does output a backtrace to serial interface in case of fatal runtime exceptions.
The backtrace is hard to read, but using a backtrace interpreter it can give valuable hints for error analysis.

##### Backtrace interpreter for PlatformIO

[A filter to the serial monitor of PlatformIO IDE](https://github.com/platformio/platform-espressif32/issues/105#issuecomment-945158769) has been [configured](https://github.com/Task-Tracker-Systems/Task-Tracker-Device/pull/35).
It is a [built-in filter to PlatformIO](https://docs.platformio.org/en/latest/core/userguide/device/cmd_monitor.html#built-in-filters).
This way input on the PlatformIO serial monitor will be interpreted live.

##### Separate backtrace interpreter

In case the serial interface input is not read by PlatformIO serial monitor one can use [this python script](https://github.com/me21/EspArduinoExceptionDecoder).

This is especially useful when simulating the microcontroller or device.

### Unit testing

The project is setup for running unit tests:

    platformio test --verbose --environment native

## Contribute

Please refer to [`CONTRIBUTING.md`](CONTRIBUTING.md).
