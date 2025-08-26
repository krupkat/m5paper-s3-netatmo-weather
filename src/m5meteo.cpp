#include "m5meteo.hpp"

#include <M5Unified.hpp>

#include "netatmo.hpp"
#include "network.hpp"

namespace {

void UpdateDisplay(bool time_synced, struct tm* timeinfo, float text_scale,
                   const NetatmoWeatherAPI& netatmo) {
  M5.Display.setTextColor(lgfx::colors::TFT_BLACK);

  M5.Display.setTextSize(3 * text_scale);
  M5.Display.setCursor(0, 0);
  M5.Display.println("Netatmo Weather");

  M5.Display.setTextSize(2 * text_scale);
  if (time_synced) {
    constexpr int kTimeinfoYearOffset = 1900;
    M5.Display.printf("Time: %02d:%02d:%02d\n", timeinfo->tm_hour,
                      timeinfo->tm_min, timeinfo->tm_sec);
    M5.Display.printf("Date: %02d/%02d/%04d\n", timeinfo->tm_mday,
                      timeinfo->tm_mon + 1,
                      timeinfo->tm_year + kTimeinfoYearOffset);
  } else {
    M5.Display.println("Time: Not synced");
  }

  int battery_level = M5.Power.getBatteryLevel();
  if (battery_level >= 0) {
    M5.Display.printf("Battery: %d%%\n", battery_level);
  } else {
    M5.Display.println("Power: External/Unknown");
  }

  M5.Display.setTextSize(3 * text_scale);
  if (netatmo.NAMain.name.length() > 0) {
    M5.Display.printf("Living Room: %s C\n",
                      netatmo.NAMain.temperature.c_str());
    M5.Display.printf(" %s ppm\n", netatmo.NAMain.co2.c_str());
  }
  if (netatmo.NAModule4[0].name.length() > 0) {
    M5.Display.printf("Bedroom:     %s C\n",
                      netatmo.NAModule4[0].temperature.c_str());
    M5.Display.printf(" %s ppm\n", netatmo.NAModule4[0].co2.c_str());
  }
  if (netatmo.NAModule1.name.length() > 0 and
      netatmo.NAModule1.temperature != "null") {
    M5.Display.printf("Outdoor:     %s C\n",
                      netatmo.NAModule1.temperature.c_str());
  }
}

}  // namespace

namespace m5meteo {

void Setup(const m5meteo::AppConfig& config) {
  auto cfg = M5.config();
  cfg.serial_baudrate = config.serial_baudrate;
  cfg.clear_display = false;
  cfg.internal_imu = false;
  M5.begin(cfg);

  if (m5meteo::ConnectToWiFi(config.network_config)) {
    m5meteo::SetupNTPSync(config.ntp_config);

    NetatmoWeatherAPI netatmo;
    bool success = m5meteo::GetNetatmoData(netatmo, config.netatmo_credentials);

    struct tm timeinfo;
    bool time_synced = getLocalTime(&timeinfo);

    M5.Display.clearDisplay();
    M5.Display.setEpdMode(lgfx::epd_mode::epd_fastest);
    M5.Display.setRotation(1);
    // M5.Display.setFont(&fonts::FreeMonoBold24pt7b);

    if (success) {
      UpdateDisplay(time_synced, &timeinfo, config.text_scale, netatmo);
    } else {
      M5.Display.setTextSize(2 * config.text_scale);
      M5.Display.setCursor(0, 0);
      M5.Display.println("Netatmo API Error");
      M5.Display.println("Check credentials");
    }
  } else {
    M5.Display.setTextSize(2 * config.text_scale);
    M5.Display.setCursor(0, 0);
    M5.Display.println("WiFi Connection Failed");
  }

  M5.Power.timerSleep(config.refresh_interval_sec);
}

}  // namespace m5meteo