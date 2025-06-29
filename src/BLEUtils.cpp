#include "../include/BLEUtils.hpp"
#include <string>
#include <sstream>

std::string uuidToString(const esp_bt_uuid_t& uuid) {
    std::ostringstream oss;
    if (uuid.len == ESP_UUID_LEN_16) {
        oss << "0000" << std::hex << uuid.uuid.uuid16 << "-0000-1000-8000-00805f9b34fb";
    } else if (uuid.len == ESP_UUID_LEN_32) {
        oss << std::hex << uuid.uuid.uuid32 << "-0000-1000-8000-00805f9b34fb";
    } else if (uuid.len == ESP_UUID_LEN_128) {
        for (int i = 0; i < 16; ++i) {
            oss << std::hex << static_cast<int>(uuid.uuid.uuid128[i]);
            if (i == 1 || i == 3 || i == 5 || i == 7 || i == 9 || i == 11 || i == 13 || i == 15) {
                oss << "-";
            }
        }
    }
    return oss.str();
}

void handleBLEEvent(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param) {
    switch (event) {
        case ESP_GATTS_CONNECT_EVT:
            // Handle connection event
            break;
        case ESP_GATTS_DISCONNECT_EVT:
            // Handle disconnection event
            break;
        case ESP_GATTS_WRITE_EVT:
            // Handle write event
            break;
        case ESP_GATTS_READ_EVT:
            // Handle read event
            break;
        // Add more cases as needed
        default:
            break;
    }
}