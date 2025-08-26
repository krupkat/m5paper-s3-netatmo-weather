#pragma once

#include "netatmo.hpp"
#include "network.hpp"

namespace m5meteo {

struct AppConfig {
  float text_scale;
  NetworkConfig network_config;
  NetatmoCredentials netatmo_credentials;
  NtpConfig ntp_config;
  std::uint32_t serial_baudrate;
  int refresh_interval_sec;
};

}  // namespace m5meteo