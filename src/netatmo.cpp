#include "netatmo.hpp"

#include <cstdint>

#include <boost/mp11/list.hpp>
#include <M5Unified.h>

#include "persistence.hpp"

namespace m5meteo {
namespace {

BOOST_DEFINE_FIXED_ENUM_CLASS(Key, std::uint8_t, kClientId, kClientSecret,
                              kDeviceId, kAccessToken, kRefreshToken);

constexpr int kAccessTokenSize = sizeof(NetatmoCredentials::access_token_init);
constexpr int kRefreshTokenSize =
    sizeof(NetatmoCredentials::refresh_token_init);

using KeyValueMap = boost::mp11::mp_list<
    utils::KVPair<Key::kClientId, String>,
    utils::KVPair<Key::kClientSecret, String>,
    utils::KVPair<Key::kDeviceId, String>,
    utils::KVPair<Key::kAccessToken, char[kAccessTokenSize]>,
    utils::KVPair<Key::kRefreshToken, char[kRefreshTokenSize]>>;

using NetatmoPersistence = utils::Persistence<Key, KeyValueMap>;

}  // namespace

bool GetNetatmoData(NetatmoWeatherAPI& netatmo, const NetatmoCredentials& creds,
                    std::uint64_t delay_timezone) {
  NetatmoPersistence store("Netatmo");

  char access_token[kAccessTokenSize];
  char refresh_token[kRefreshTokenSize];

  char previous_access_token[kAccessTokenSize];
  char previous_refresh_token[kRefreshTokenSize];

  store.Get<Key::kAccessToken>(access_token, creds.access_token_init);
  store.Get<Key::kRefreshToken>(refresh_token, creds.refresh_token_init);

  memcpy(previous_access_token, access_token, kAccessTokenSize);
  memcpy(previous_refresh_token, refresh_token, kRefreshTokenSize);

  M5_LOGI("Starting Netatmo weather data request");
  // netatmo.setDebug(true);

  String device_id = store.Get<Key::kDeviceId>(creds.device_id);
  int result = netatmo.getStationsData(access_token, device_id, delay_timezone);
  M5_LOGI("Netatmo API call result: %d", result);

  if (result == EXPIRED_ACCESS_TOKEN || result == INVALID_ACCESS_TOKEN) {
    String client_id = store.Get<Key::kClientId>(creds.client_id);
    String client_secret = store.Get<Key::kClientSecret>(creds.client_secret);

    if (netatmo.getRefreshToken(access_token, refresh_token, client_secret,
                                client_id)) {
      if (strncmp(previous_access_token, access_token, kAccessTokenSize) != 0) {
        store.Put<Key::kAccessToken>(access_token);
        M5_LOGI("NVS : access_token updated");
      } else {
        M5_LOGI("NVS : same access_token");
      }

      if (strncmp(previous_refresh_token, refresh_token, kRefreshTokenSize) !=
          0) {
        store.Put<Key::kRefreshToken>(refresh_token);
        M5_LOGI("NVS : refresh_token updated");
      } else {
        M5_LOGI("NVS : same refresh_token");
      }

      result = netatmo.getStationsData(access_token, device_id, delay_timezone);
      M5_LOGI("Netatmo API call result: %d", result);
    } else {
      M5_LOGI("Failed to refresh Netatmo access token");
    }
  }

  return result == VALID_ACCESS_TOKEN;
}

}  // namespace m5meteo