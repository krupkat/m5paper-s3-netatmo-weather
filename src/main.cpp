#include "config.hpp"
#include "m5meteo.hpp"

#if __has_include("private_config.hpp")
#include "private_config.hpp"
#endif

void setup() {
#ifdef HAS_PRIVATE_CONFIG
  const m5meteo::AppConfig& config = m5meteo::kPrivateConfig;
#else
  m5meteo::AppConfig config = {
      .text_scale = 2.7,
      .network_config = {.ssid = "",
                         .password = "",
                         .connection_attempts = 20,
                         .connection_delay = 500},
      .netatmo_credentials =
          {.client_id = "",
           .client_secret = "",
           .device_id = "",
           .access_token_init =
               "000000000000000000000000000000000000000000000000000000000",
           .refresh_token_init =
               "000000000000000000000000000000000000000000000000000000000"},
      .ntp_config = {.timezone = "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00",
                     .ntp_server_1 = "pool.ntp.org",
                     .ntp_server_2 = "time.nist.gov",
                     .ntp_server_3 = "time.google.com"},
      .serial_baudrate = 115200,
      .refresh_interval_sec = 5 * 60,
  };
#endif

  m5meteo::Setup(config);
}

void loop() {}
