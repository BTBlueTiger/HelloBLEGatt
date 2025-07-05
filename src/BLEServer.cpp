#include "../include/BLEServer.hpp"
#include "../include/BLEUtils.hpp"
#include "../include/Service/LedService.hpp"
#include "../include/Service/ServiceHandler.hpp"
#include "esp_log.h"
#include "esp_bt_defs.h"
#include "nvs_flash.h"

#define GATTS_TAG "BLE_SERVER"

struct gatts_profile_inst {
    esp_gatts_cb_t gatts_cb;
    uint16_t gatts_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t service_handle;
    esp_gatt_srvc_id_t service_id;
    uint16_t char_handle;
    esp_bt_uuid_t char_uuid;
    esp_gatt_perm_t perm;
    esp_gatt_char_prop_t property;
    uint16_t descr_handle;
    esp_bt_uuid_t descr_uuid;
};

// Static member definitions
char test_device_name[ESP_BLE_ADV_NAME_LEN_MAX] = "ESP_GATTS_DEMO";
uint16_t BLEServer::led_char_handle = 0;
esp_gatt_if_t BLEServer::global_if = 0;
std::function<void(const esp_ble_gatts_cb_param_t& param)> BLEServer::writeCallback;

static uint8_t adv_service_uuid128[32] = {
    0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,
    (uint8_t)(LED_SERVICE_UUID & 0xFF), (uint8_t)((LED_SERVICE_UUID >> 8) & 0xFF)

};

#define PROFILE_NUM 1




// Initialize the profile table
static struct gatts_profile_inst gl_profile_tab[PROFILE_NUM] = {
    {
        .gatts_cb = BLEServer::gattsEventHandler, /* GATT server event handler */
        .gatts_if = ESP_GATT_IF_NONE,       /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
    }
};



// Advertising data
static esp_ble_adv_params_t adv_params = {
    .adv_int_min = 0x20,
    .adv_int_max = 0x40,
    .adv_type = ADV_TYPE_IND,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .channel_map = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

static esp_ble_adv_data_t adv_data = {
    .set_scan_rsp = false,
    .include_name = true,
    .include_txpower = false,
    .min_interval = 0x0006, //slave connection min interval, Time = min_interval * 1.25 msec
    .max_interval = 0x0010, //slave connection max interval, Time = max_interval * 1.25 msec
    .appearance = 0x00,
    .manufacturer_len = 0, //TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data =  NULL, //&test_manufacturer[0],
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = sizeof(adv_service_uuid128),
    .p_service_uuid = (uint8_t *)&adv_service_uuid128,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

BLEServer::BLEServer() = default;
BLEServer::~BLEServer() = default;

esp_err_t BLEServer::startServer(std::function<void(const esp_ble_gatts_cb_param_t& param)> cb)
{
    writeCallback = cb;

    ESP_LOGI(GATTS_TAG, "Starting BLE Server...");

    esp_err_t ret;

    // NVS initialization
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize and enable BT controller
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_BLE));

    // Init and enable Bluedroid stack
    ESP_ERROR_CHECK(esp_bluedroid_init());
    ESP_ERROR_CHECK(esp_bluedroid_enable());

    // Register callbacks
    ESP_LOGI(GATTS_TAG, "Registering GATT server callback...");

    ESP_ERROR_CHECK(esp_ble_gatts_register_callback(ServiceHandler::callback));

    ESP_ERROR_CHECK(esp_ble_gap_register_callback(gapEventHandler));
    ESP_LOGI(GATTS_TAG, "Registering GATT callback...");
    ESP_ERROR_CHECK(esp_ble_gatts_app_register(PROFILE_A_APP_ID));

    // Set device name
    ESP_ERROR_CHECK(esp_ble_gap_set_device_name(test_device_name));

    // Configure advertising data
    ESP_ERROR_CHECK(esp_ble_gap_config_adv_data(&adv_data));

    // Set MTU
    ESP_ERROR_CHECK(esp_ble_gatt_set_local_mtu(500));

    return ESP_OK;
}

void BLEServer::gapEventHandler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch (event)
    {
    case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
        ESP_LOGI(GATTS_TAG, "Advertising data set complete. Starting advertising...");
        ESP_ERROR_CHECK(esp_ble_gap_start_advertising(&adv_params));
        break;
    default:
        break;
    }
}
esp_attr_value_t attrVal = {
    .attr_max_len = 1,
    .attr_len     = 1,
    .attr_value   = (uint8_t *)"\x00",
};

void BLEServer::gattsEventHandler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    LedServicecallBack(event, gatts_if, param);
    /*
    if(event == ESP_GATTS_REG_EVT) {
            ESP_LOGI(GATTS_TAG, "GATT server register, status %d, app_id %d, gatts_if %d", param->reg.status, param->reg.app_id, gatts_if);
            gl_profile_tab[PROFILE_A_APP_ID].service_id.is_primary = true;
            gl_profile_tab[PROFILE_A_APP_ID].service_id.id.inst_id = 0x00;
            gl_profile_tab[PROFILE_A_APP_ID].service_id.id.uuid.len = ESP_UUID_LEN_16;
            gl_profile_tab[PROFILE_A_APP_ID].service_id.id.uuid.uuid.uuid16 = led_service_uuid;
            esp_ble_gatts_create_service(gatts_if, &gl_profile_tab[PROFILE_A_APP_ID].service_id, GATTS_NUM_HANDLE_TEST_A);
    } else if(event == ESP_GATTS_CREATE_EVT) {
        gl_profile_tab[PROFILE_A_APP_ID].service_handle = param->create.service_handle;
        gl_profile_tab[PROFILE_A_APP_ID].char_uuid.len = ESP_UUID_LEN_16;
        gl_profile_tab[PROFILE_A_APP_ID].char_uuid.uuid.uuid16 = led_char_uuid;
    
        esp_ble_gatts_start_service(gl_profile_tab[PROFILE_A_APP_ID].service_handle);
    
        a_property = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE | ESP_GATT_CHAR_PROP_BIT_NOTIFY;
        esp_err_t add_char_ret = esp_ble_gatts_add_char(gl_profile_tab[PROFILE_A_APP_ID].service_handle,
                                                        &gl_profile_tab[PROFILE_A_APP_ID].char_uuid,
                                                        ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                                                        a_property,
                                                        &attrVal, NULL);
        if (add_char_ret == ESP_OK) {
            led_char_handle = param->add_char.attr_handle; // Assign the characteristic handle
            ESP_LOGI(GATTS_TAG, "Characteristic handle assigned: %d", led_char_handle);
        } else {
            ESP_LOGE(GATTS_TAG, "Failed to add characteristic, error code = %x", add_char_ret);
        }
    } else if (event == ESP_GATTS_WRITE_EVT) {
        ESP_LOGI(GATTS_TAG, "Write event received, conn_id %d, handle %d, value len %d",
                    param->write.conn_id, param->write.handle, param->write.len);

        // Check if the write is for the characteristic we are interested in
        if (param->write.handle == 42) {
            // Call the user-defined write callback if set
            if (BLEServer::writeCallback) {
                BLEServer::writeCallback(*param);
            }
        } else {
            ESP_LOGW(GATTS_TAG, "Write to an unknown handle.");

        // Save the characteristic handle for later use
        BLEServer::led_char_handle = param->add_char.attr_handle;

        // Start advertising after adding the characteristic
        esp_ble_gap_start_advertising(&adv_params);
        }
    }
        */
}