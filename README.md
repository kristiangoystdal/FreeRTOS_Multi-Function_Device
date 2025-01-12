
# FreeRTOS Multi-Function Device

This repository contains the implementation of a multi-function device using **FreeRTOS** on the **mbed LPC1768** platform. The project showcases real-time multitasking with a focus on managing concurrent tasks for embedded systems. Developed as part of the Real-Time Systems course at IST (2024/2025), this project demonstrates expertise in FreeRTOS-based applications.

## Project Overview

The device incorporates multiple features such as temperature monitoring, bubble level visualization, real-time clock management, and an interactive console interface. Optional functionalities like the "Hit Bit" game enhance the scope of this project.

---

## Key Features

### Main Functionalities
- **Temperature Monitoring**:
  - Uses LM75B sensor for periodic and on-demand temperature readings.
  - Visual indicators via RGB LED and data logging with timestamps.
  - Threshold-based alarms and min/max temperature tracking.

- **Bubble Level**:
  - Utilizes the MMA7660 accelerometer for LCD-based graphical output.

- **Real-Time Clock**:
  - Maintains and displays the current time and date.
  - Features an alarm clock with sound notification.

- **User Interface**:
  - Interactive console interface for command execution.
  - Real-time LCD feedback for monitoring data and alarms.

- **Optional Game - Hit Bit**:
  - A game utilizing onboard LEDs and joystick for interactive gameplay.

---

## Directory Structure

- **`controller`**: Libraries and drivers for peripherals like LCD, temperature sensors, accelerometers, and RGB LEDs.
- **`freertos-cm3`**: FreeRTOS core files and configurations for ARM-Cortex M3.
- **`tasks`**: Implementation of task-specific functionalities including alarms, bubble level, Hit Bit, LCD control, and more.
- **`utils`**: Utility functions like atomic operations and date-time handling.
- **`main.cpp`**: Entry point for the application, initializing tasks and FreeRTOS kernel.

---

## Commands and User Interaction

### Console Commands
The console interface supports a wide range of commands for system interaction:

| Command       | Arguments            | Description                                        |
|---------------|----------------------|----------------------------------------------------|
| `rdt`         | -                    | Read date/time.                                   |
| `sd`          | day, month, year     | Set the date.                                     |
| `rc`          | -                    | Read clock.                                       |
| `sc`          | hours, minutes, sec  | Set the clock.                                    |
| `rt`          | -                    | Read temperature.                                 |
| `rmm`         | -                    | Read max and min temperatures.                   |
| `cmm`         | -                    | Clear max and min temperature records.           |
| `rp`          | -                    | Read monitoring parameters.                      |
| `mmp`         | seconds              | Modify monitoring period.                        |
| `mta`         | seconds              | Modify alarm duration.                           |
| ...           | ...                  | See detailed documentation for all commands.     |

### LCD Display
The LCD provides:
- Clock and temperature readings.
- Bubble Level graphical representation.
- Alarm status indicators.

---

## Getting Started

### Prerequisites
- Development environment: [Mbed Online Compiler](https://os.mbed.com/compiler) or [Keil Studio Cloud](https://os.mbed.com/studio).
- Hardware: mbed LPC1768 and mbed Application Board.

### Setup
1. Clone the repository:
   ```bash
   git clone https://github.com/kristiangoystdal/FreeRTOS_Multi-Function_Device.git
   ```
2. Open the project in your development environment.
3. Compile and flash the program onto the mbed LPC1768.

---

## Contributing

Contributions are welcome! Fork this repository and submit a pull request with your changes.

---

## References
- [FreeRTOS Official Documentation](http://www.freertos.org)
- [Mbed LPC1768 Platform](https://os.mbed.com/platforms/mbed-LPC1768)
- [Mbed Application Board](https://os.mbed.com/components/mbed-Application-Board)
