#pragma once

#include <cstdint>

#include <WString.h>

namespace m5meteo {

struct NetworkConfig {
  String ssid;
  String password;
  int connection_attempts;
  std::uint32_t connection_delay;
};

struct NtpConfig {
  String timezone;
  String ntp_server_1;
  String ntp_server_2;
  String ntp_server_3;
};

bool ConnectToWiFi(const NetworkConfig& config);
void SetupNTPSync(const NtpConfig& ntp_config);
}  // namespace m5meteo