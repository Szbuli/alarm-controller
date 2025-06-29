# Sensor Controller

## Main Features

- **15 configurable sensor inputs**  
- **Supported power supply:** 13.8 V nominal  
- **Home Assistant autodiscovery** via MQTT gateway
- **Input types supported:**  
  - Alarm sensors with multi-level signaling (e.g., open/tamper detection)  
  - Pulse counters (e.g., utility meters)

## Hardware

- **Microcontroller:** STM32F103R8T6  
- **Non-volatile memory:** MB85RC256V FRAM  
- **CAN Bus Transceiver:** SN65HVD230  
- **Electrical protection:**  
  - Overvoltage-protected inputs  
  - Overload protection circuitry  
- **Debug Interface:** Serial Wire Debug (SWD)

## Development Tools

- **PCB Design:** Autodesk EAGLE  
- **Firmware:** STM32CubeIDE (C via HAL / LL libraries)

## Setup
1. **Assign the device ID** via MQTT topic: `smarthome/alarm_controller/config`
2. **Configure sensor input types**
-- `1` → Alarm sensor
-- `3` → Pulse counter (normally open)
-- `4` → Pulse counter (normally closed)

## Sensor Connection & Security

To ensure reliable operation and tamper detection, each sensor input must be connected using a resistor for end-of-line (EOL) supervision.
The controller uses a voltage divider circuit to distinguish between different sensor states. The correct resistor value is essential for accurate detection:

- **Switch / Door Contact Sensors:**  
  Use a **40.2 kΩ** resistor in series with the sensor contact.  
- **Alarm Sensors:**  
  - **Normal (closed) signal:** ~40 kΩ total resistance  
  - **Alarm (open) signal:** ~140 kΩ total resistance