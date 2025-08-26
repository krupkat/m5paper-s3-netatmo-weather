#pragma once

#include <cstdint>

#include <NetatmoWeatherAPI.h>

namespace m5meteo {

struct NetatmoCredentials {
  String client_id;
  String client_secret;
  String device_id;

  char access_token_init[58];
  char refresh_token_init[58];
};

bool GetNetatmoData(NetatmoWeatherAPI& netatmo, const NetatmoCredentials& creds,
                    std::uint64_t delay_timezone = 0);

}  // namespace m5meteo