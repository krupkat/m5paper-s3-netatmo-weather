#include "network.hpp"

#include <cstdint>

#include <Arduino.h>
#include <boost/mp11/list.hpp>
#include <M5Unified.h>
#include <WiFi.h>

#include "persistence.hpp"

namespace m5meteo {
namespace {

BOOST_DEFINE_FIXED_ENUM_CLASS(Key, std::uint8_t, kSsid, kPassphrase);

using KeyValueMap =
    boost::mp11::mp_list<utils::KVPair<Key::kSsid, String>,
                         utils::KVPair<Key::kPassphrase, String>>;

using NetworkPersistence = utils::Persistence<Key, KeyValueMap>;

}  // namespace

bool ConnectToWiFi(const NetworkConfig& config) {
  NetworkPersistence store("Network");
  String ssid = store.Get<Key::kSsid>(config.ssid);
  String password = store.Get<Key::kPassphrase>(config.password);

  M5_LOGI("Starting WiFi connection process");
  M5_LOGI("SSID: %s", ssid.c_str());

  WiFi.begin(ssid, password);
  M5_LOGI("WiFi.begin() called");

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED &&
         attempts < config.connection_attempts) {
    M5.delay(config.connection_delay);
    attempts++;
    M5_LOGI("WiFi connection attempt %d, status: %d", attempts, WiFi.status());
  }

  if (auto status = WiFi.status(); status == WL_CONNECTED) {
    M5_LOGI("WiFi connected successfully!");
    M5_LOGI("IP address: %s", WiFi.localIP().toString().c_str());
    M5_LOGI("Signal strength (RSSI): %d dBm", WiFi.RSSI());
  } else {
    M5_LOGI("WiFi connection failed after %d attempts", attempts);
    M5_LOGI("Final WiFi status: %d", status);
    return false;
  }

  return true;
}

void SetupNTPSync(const NtpConfig& ntp_config) {
  M5_LOGI("Configuring NTP time synchronization");
  M5_LOGI("NTP server 1: %s", ntp_config.ntp_server_1.c_str());
  M5_LOGI("NTP server 2: %s", ntp_config.ntp_server_2.c_str());
  M5_LOGI("NTP server 3: %s", ntp_config.ntp_server_3.c_str());
  M5_LOGI("Timezone: %s", ntp_config.timezone.c_str());
  configTzTime(ntp_config.timezone.c_str(), ntp_config.ntp_server_1.c_str(),
               ntp_config.ntp_server_2.c_str(),
               ntp_config.ntp_server_3.c_str());
  M5_LOGI("NTP time configuration completed");
}

}  // namespace m5meteo