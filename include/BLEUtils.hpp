#ifndef BLEUTILS_H
#define BLEUTILS_H

#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gatt_common_api.h"
#include <stdint.h>
#include <string>
#include "esp_bt_defs.h"

// Function to convert a UUID from string format to esp_bt_uuid_t
esp_bt_uuid_t uuidStringToBtUuid(const std::string& uuidStr);

// Function to convert esp_bt_uuid_t to string format
std::string btUuidToString(const esp_bt_uuid_t& uuid);

// Function to handle BLE events
void handleBLEEvent(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

#endif // BLEUTILS_H