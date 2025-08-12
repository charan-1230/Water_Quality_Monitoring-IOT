# Water Quality Monitoring IoT System

A real-time water quality monitoring system using ESP32 that measures TDS (Total Dissolved Solids) and turbidity levels, with cloud connectivity and alerts.

[Click Here](https://drive.google.com/drive/folders/1M1vC5WxWjOEIWhvPctRmyXq7FNjlsusn?usp=sharing)


## Project Overview

This IoT-based water quality monitoring system continuously monitors water quality parameters and provides real-time data visualization and alerts. The system uses sensors to measure water quality and sends data to ThingSpeak cloud platform for remote monitoring.

## Features

- **Real-time Monitoring**: Continuous measurement of water quality parameters
- **TDS Measurement**: Measures Total Dissolved Solids in water (ppm)
- **Turbidity Detection**: Classifies water as CLEAR, CLOUDY, or DIRTY
- **Cloud Connectivity**: Data upload to ThingSpeak platform via MQTT and HTTP
- **Alert System**: Buzzer alerts when water quality exceeds safe thresholds
- **WiFi Connectivity**: Wireless data transmission
- **Temperature Compensation**: Automatic temperature compensation for accurate TDS readings

## Hardware Components

- **ESP32 Development Board**
- **TDS Sensor** (connected to pin 36)
- **Turbidity Sensor** (connected to pin 34)
- **Buzzer** (connected to pin 13)
- **Power Supply**
- **Jumper Wires**
- **Breadboard/PCB**

## Pin Configuration

| Component | ESP32 Pin |
|-----------|-----------|
| TDS Sensor | GPIO 36 |
| Turbidity Sensor | GPIO 34 |
| Buzzer | GPIO 13 |

## Software Dependencies

### Libraries Required
```cpp
#include <PubSubClient.h>    // MQTT communication
#include <ThingSpeak.h>      // ThingSpeak API
#include <WiFi.h>           // WiFi connectivity
```

## Code Structure

### Main Files
- final_code.ino - Complete system implementation
- tds.ino - TDS sensor testing code
- turbidity.ino - Turbidity sensor testing code

### Key Functions

#### TDS Measurement
- Uses median filtering algorithm for stable readings
- Temperature compensation for accuracy
- Converts analog readings to TDS values in ppm

#### Turbidity Detection
- Maps sensor values to turbidity percentage
- Classifies water quality:
  - **< 20%**: CLEAR
  - **20-50%**: CLOUDY
  - **> 50%**: DIRTY

#### Alert System
- Triggers buzzer when:
  - Turbidity > 20%
  - TDS > 500 ppm

## Setup Instructions

### 1. Hardware Setup
1. Connect TDS sensor to GPIO 36
2. Connect turbidity sensor to GPIO 34
3. Connect buzzer to GPIO 13
4. Power the ESP32 board

### 2. Software Configuration
1. Install required libraries in Arduino IDE
2. Update WiFi credentials:
   ```cpp
   char ssid[] = "Your_WiFi_SSID";
   char pass[] = "Your_WiFi_Password";
   ```
3. Configure ThingSpeak settings:
   ```cpp
   int writeChannelID = YOUR_CHANNEL_ID;
   char writeAPIKey[] = "Your_Write_API_Key";
   ```

### 3. Upload Code
1. Open `final_code.ino`
2. Select ESP32 board in Arduino IDE
3. Upload the code to ESP32

## Data Transmission

### ThingSpeak Integration
- **Channel ID**: YOUR_CHANNEL_ID
- **Field 1**: TDS Values (ppm)
- **Field 2**: Turbidity Percentage
- **Update Interval**: 15 seconds per field

### MQTT Configuration
- **Server**: mqtt3.thingspeak.com
- **Port**: 1883
- **Username**: YOUR_MQTT_USERNAME
- **Password**: YOUR_MQTT_PASSWORD

## Monitoring and Alerts

### Water Quality Thresholds
- **Safe TDS Level**: < 500 ppm
- **Safe Turbidity**: < 20%

### Alert Mechanism
When water quality exceeds safe levels:
1. Buzzer activates for 2 seconds
2. System pauses for 2 seconds
3. Pattern repeats until water quality improves

## Demo Videos 
[Click Here](https://drive.google.com/drive/folders/1M1vC5WxWjOEIWhvPctRmyXq7FNjlsusn?usp=sharing)
- initial_demo.mp4 - Initial system demonstration
- final_demo.mp4 - Complete system functionality

## Documentation
- proposal.pdf - Project proposal document
- presentation.pdf - Project presentation

## Technical Specifications

### TDS Sensor
- **Reference Voltage**: 5.0V
- **Sample Count**: 30 (for individual testing), 1 (for integrated system)
- **Filtering**: Median filtering algorithm
- **Temperature Compensation**: Automatic

### Turbidity Sensor
- **Input Range**: 0-750 (analog reading)
- **Output Range**: 0-100% turbidity
- **Classification**: Clear/Cloudy/Dirty

### System Timing
- **TDS Sampling**: Every 40ms
- **TDS Display**: Every 800ms
- **ThingSpeak Upload**: Every 15 seconds
- **Main Loop Delay**: 1 second

## Troubleshooting

### Common Issues
1. **WiFi Connection Failed**: Check SSID and password
2. **Sensor Readings Inconsistent**: Verify pin connections
3. **ThingSpeak Upload Failed**: Check API key and channel ID
4. **Buzzer Not Working**: Verify GPIO 13 connection

### Serial Monitor Output
Monitor the serial output for:
- WiFi connection status
- Sensor readings
- ThingSpeak upload confirmation
- Water quality classifications

