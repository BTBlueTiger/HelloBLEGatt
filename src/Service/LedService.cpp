#include "../Configurations/BLEConfig.h"
#include "../include/Service/LedService.hpp"
#include "esp_log.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"        
#include <cstring>


LedService* LedService::instance = nullptr;
uint16_t LedService::serviceHandle = 0;

esp_gatt_srvc_id_t LedService::serviceId = {
    .id = {
        .uuid = {
            .len = ESP_UUID_LEN_16,
            .uuid = { .uuid16 = LED_SERVICE_UUID }
        },
        .inst_id = 0x00
    },
    .is_primary = true
};

esp_bt_uuid_t LedService::ledServiceSwitchUuid = {
    .len = ESP_UUID_LEN_16,
    .uuid = { .uuid16 = LED_SERVICE_UUID }
};

esp_attr_value_t LedService::ledServiceSwitchAttribute = {
    .attr_max_len = 1,
    .attr_len     = 1,
    .attr_value   = (uint8_t *)"\x00",
};


LedService* LedService::getInstance() {
    if (!instance) {
        instance = new LedService();
        instance->setInstance(instance);
    }
    return instance;
}

LedService::~LedService() {
    // Destructor logic if needed
}

const char* LedService::getTaskName(int TASK_T) const {
    switch (TASK_T) {
        case SWITCH:
            return "LED Switch";
        default:
            return "Unknown Task";
    }
}

void LedService::handleReadRequest(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param) {
    ESP_LOGI("AbstractService", "Read event received, handle %d", param->read.handle);

    if (handleToCharacteristic.empty()) {
        ESP_LOGW("AbstractService", "No characteristics available for read request");
        return;
    }

    // Iterate through all characteristics in the map
    for (const auto& pair : handleToCharacteristic) {
        uint16_t handle = pair.first; // Key (handle)
        const Characteristic& characteristic = pair.second; // Value (Characteristic object)

        ESP_LOGI("AbstractService", "Sending characteristic handle: %d, description: %s", handle, characteristic.descriptorValue.c_str());

        // Prepare the response
        esp_gatt_rsp_t rsp = {0};
        rsp.attr_value.handle = handle;
        rsp.attr_value.len = characteristic.descriptorValue.size(); // Length of the descriptor value
        memcpy(rsp.attr_value.value, characteristic.descriptorValue.c_str(), rsp.attr_value.len); // Copy the descriptor value

        // Send the response
        esp_err_t send_ret = esp_ble_gatts_send_response(
            gatts_if, param->read.conn_id, param->read.trans_id, ESP_GATT_OK, &rsp
        );

        if (send_ret == ESP_OK) {
            ESP_LOGI("AbstractService", "Descriptor value sent successfully for handle %d", handle);
        } else {
            ESP_LOGE("AbstractService", "Failed to send descriptor value for handle %d, error code = %x", handle, send_ret);
        }
    }
}

void LedService::handleCreateRequest(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
    ESP_LOGI("LedService", "Create event received");
    LedService* service = LedService::getInstance();
    
    /*********************************************/
    /***************    LED SWITCH  **************/
    /*********************************************/
    service->serviceHandle = param->create.service_handle;
    ESP_LOGI("LedService", "Service created, handle %d", service->serviceHandle);
    esp_ble_gatts_start_service(LedService::serviceHandle);




    service->createReadWriteCharacteristic(
        LedService::TASK_T::SWITCH,
        param,
        LedService::ledServiceSwitchUuid,
        LedService::ledServiceSwitchAttribute
    );

    uint16_t handle = taskToHandle[LedService::TASK_T::SWITCH];
    Characteristic* ch = &handleToCharacteristic[handle];

    cJSON* root = cJSON_CreateObject();

    // Add basic values
    cJSON_AddStringToObject(root, "name", "LED Switch");
    cJSON_AddNumberToObject(root, "handle", ch->handle);

    // Add values array
    cJSON* valuesArray = cJSON_CreateIntArray((int[]){0, 1}, 2);
    cJSON_AddItemToObject(root, "values", valuesArray);

    // Add valueDescription array
    cJSON* descriptionArray = cJSON_CreateStringArray((const char*[]){"OFF", "ON"}, 2);
    cJSON_AddItemToObject(root, "valueDescription", descriptionArray);

    // Convert to JSON string
    ch->descriptorValue = cJSON_PrintUnformatted(root);

    // Clean up
    cJSON_Delete(root);
}

void LedService::handleRegisterRequest(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param) {
    ESP_LOGI("LedService", "GATT server registered, status %d, app_id %d, gatts_if %d", param->reg.status, param->reg.app_id, gatts_if);
    esp_ble_gatts_create_service(gatts_if, &LedService::serviceId, 4);
}