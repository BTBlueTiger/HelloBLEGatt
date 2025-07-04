#ifndef BLE_SERVER_H
#define BLE_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"
#include "sdkconfig.h"
#include <unordered_map>
#include <utility>
#include <functional>
#include <memory>

#include "../Configurations/BLEConfig.h"
#include "../Configurations/BLEConstants.h"

// Forward declaration of AbstractService
class AbstractService;


class BLEServer {
public:
    BLEServer();
    ~BLEServer();
    
    void addServices();

    static esp_err_t startServer(std::function<void(const esp_ble_gatts_cb_param_t& param)> writeCallback);
    void sendNotification(uint16_t conn_id, const uint8_t *data, size_t length);

    static uint16_t led_char_handle;
    static std::function<void(const esp_ble_gatts_cb_param_t& param)> writeCallback;

    
    esp_gatt_if_t gatts_if;
    uint16_t service_handle;
    uint16_t char_handle;
    esp_bt_uuid_t char_uuid;
    esp_gatt_char_prop_t property;
    
    static esp_bt_controller_config_t bt_cfg;
    
    static esp_gatt_if_t global_if;


    static void gattsEventHandler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if,
        esp_ble_gatts_cb_param_t* param);

    static void gapEventHandler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t* param);



};

#endif // BLE_SERVER_H