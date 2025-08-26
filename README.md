# ESP32 Netatmo Weather Monitor with E-Ink Display

A Netatmo weather station monitoring application for ESP32 devices with e-ink displays using M5Stack libraries. This application connects to your personal Netatmo weather station to display real-time indoor and outdoor environmental data.

## Hardware Requirements

- ESP32-S3 development board (or compatible M5Paper/M5PaperS3)
- E-ink display compatible with M5GFX library
- WiFi connection
- Netatmo Weather Station with modules

## Software Dependencies

- M5Unified library for hardware abstraction
- NetatmoWeatherAPI library for Netatmo integration
- WiFi and HTTPClient libraries (built-in ESP32)
- esp-boost library for extra plumbing

## Configuration

### WiFi Setup
Edit the WiFi credentials in `src/main.cpp`

### Netatmo API Setup

To get your Netatmo credentials:

1. Go to [Netatmo Developer Portal](https://dev.netatmo.com/)
2. Create an application to get Client ID and Client Secret
3. Create a personal access token
4. Find your device ID in the Netatmo app or web interface
5. Fill in the credentials in `src/main.cpp`

## Building and Uploading

1. Install PlatformIO
2. Clone this repository
3. Build and upload:

   ```bash
   pio run
   pio run -t upload
   ```

## Usage

1. Power on the device
2. The device will attempt to connect to the specified WiFi network
3. Once connected, it will synchronize time with NTP servers
4. Netatmo weather data will be fetched and displayed
5. The device shuts down for 5 minutes to conserve battery
6. It automatically wakes up and refreshes data every 5 minutes

## Getting Started (Quick Commands)

```bash
pio project init --board esp32-s3-devkitm-1 --ide vim
pio pkg install
pio run
pio run -t compiledb
pio run -t upload
pio device monitor
```

## Links

- [Netatmo Developer Portal](https://dev.netatmo.com/)
- [NetatmoWeatherAPI Library](https://github.com/LArtisanDuDev/NetatmoWeatherAPI)
- [Original Inspiration](https://github.com/LArtisanDuDev/Lilygo_T5_Epaper_Netatmo_Weather)
- [RTC smooth restart](https://github.com/felmue/MyM5StackExamples/blob/main/M5PaperS3/RTCClockNonFlickering/RTCClockNonFlickering.ino)
- [M5Stack Documentation](https://docs.m5stack.com/en/arduino/m5unified/intro_vscode)

## License

This project is open source. Feel free to modify and distribute.
