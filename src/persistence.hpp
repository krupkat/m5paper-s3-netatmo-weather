#pragma once

#include <cstdint>
#include <type_traits>

#include <boost/describe/enum.hpp>
#include <boost/describe/enum_to_string.hpp>
#include <boost/mp11/algorithm.hpp>
#include <boost/mp11/map.hpp>
#include <Preferences.h>
#include <WString.h>

namespace m5meteo::utils {

template <auto key, typename value_type>
using KVPair = boost::mp11::mp_list<std::integral_constant<decltype(key), key>,
                                    value_type>;

template <typename TKeyType, typename TKeyValueMap>
class Persistence {
 public:
  explicit Persistence(const char* name) { nvs_.begin(name, false); }
  ~Persistence() { nvs_.end(); }

  Persistence(const Persistence&) = delete;
  Persistence& operator=(const Persistence&) = delete;
  Persistence(Persistence&&) = delete;
  Persistence& operator=(Persistence&&) = delete;

  template <TKeyType key>
  using ValueType = boost::mp11::mp_at_c<
      boost::mp11::mp_map_find<TKeyValueMap,
                               std::integral_constant<TKeyType, key>>,
      1>;

  template <TKeyType key,
            typename = std::enable_if_t<not std::is_array_v<ValueType<key>>>>
  auto Get(const ValueType<key>& default_value) -> ValueType<key> {
    if (Init<key>(default_value) == InitResult::kSuccess) {
      return default_value;
    }
    const char* key_name = boost::describe::enum_to_string(key, nullptr);
    if constexpr (std::is_same_v<ValueType<key>, String>) {
      return nvs_.getString(key_name);
    }
  }

  template <TKeyType key,
            typename = std::enable_if_t<std::is_array_v<ValueType<key>>>>
  void Get(ValueType<key>& dest, const ValueType<key>& default_value) {
    if (Init<key>(default_value) == InitResult::kSuccess) {
      memcpy(dest, default_value, sizeof(ValueType<key>));
      return;
    }
    const char* key_name = boost::describe::enum_to_string(key, nullptr);
    nvs_.getString(key_name, dest, sizeof(ValueType<key>));
  }

  template <TKeyType key>
  void Put(const ValueType<key>& value) {
    const char* key_name = boost::describe::enum_to_string(key, nullptr);
    if constexpr (std::is_same_v<ValueType<key>, String> or
                  std::is_array_v<ValueType<key>>) {
      nvs_.putString(key_name, value);
    }
  }

 private:
  enum class InitResult : std::uint8_t { kSuccess, kAlreadyInitialized };

  template <TKeyType key>
  InitResult Init(const ValueType<key>& default_value) {
    const char* key_name = boost::describe::enum_to_string(key, nullptr);
    if (!nvs_.isKey(key_name)) {
      Put<key>(default_value);
      return InitResult::kSuccess;
    }
    return InitResult::kAlreadyInitialized;
  }

  Preferences nvs_;
};

}  // namespace m5meteo::utils