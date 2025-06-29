#ifndef BLE_CONSTANTS_H
#define BLE_CONSTANTS_H

#include <stdio.h>
#include <stdlib.h>

// Maximum payload size for BLE notifications and indications
const size_t MAX_PAYLOAD_SIZE = 20;

// Error codes for BLE operations
enum class BLEErrorCode {
    SUCCESS = 0,
    FAILURE = -1,
    INVALID_PARAMETER = -2,
    NOT_CONNECTED = -3,
    TIMEOUT = -4
};

#define PROFILE_A_APP_ID 0
#define GATTS_NUM_HANDLE_TEST_A     4

// UUIDs for BLE services and characteristics
const char* const SERVICE_UUID = "0000FF01-0000-1000-8000-00805F9B34FB";
const char* const CHARACTERISTIC_UUID = "0000FF02-0000-1000-8000-00805F9B34FB";
constexpr uint16_t primary_service_uuid = ESP_GATT_UUID_PRI_SERVICE; // value = 0x2800
constexpr uint16_t led_service_uuid = 0x00FF; // Or use a full 128-bit one if needed
constexpr uint16_t character_declaration_uuid = ESP_GATT_UUID_CHAR_DECLARE; // value = 0x2803
constexpr uint8_t char_prop_write = ESP_GATT_CHAR_PROP_BIT_WRITE;
constexpr uint16_t led_char_uuid = 0xFF01;

static esp_gatt_char_prop_t a_property = 0;

#define GATTS_DEMO_CHAR_VAL_LEN_MAX 0x40

#endif // BLE_CONSTANTS_H