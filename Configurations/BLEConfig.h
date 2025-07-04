#ifndef BLE_CONFIG_H
#define BLE_CONFIG_H

// BLE Service and Characteristic UUIDs
#define GATTS_SERVICE_UUID      0x1234
#define GATTS_CHAR_UUID         0x5678
#define GATTS_IF_NONE           0xFF
#define CHAR_UUID               0x5678  

#define LED_SERVICE_UUID        0xFF


// Maximum payload size for notifications and indications
//#define MAX_PAYLOAD_SIZE 20

// BLE Server Configuration
#define BLE_SERVER_NAME "ESP_BLE_SERVER"
#define BLE_SERVER_MAX_CONNECTIONS 4

// MTU size configuration
#define BLE_MTU_SIZE 256

#endif // BLE_CONFIG_H