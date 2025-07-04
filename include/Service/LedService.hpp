#ifndef LED_SERVICE_HPP
#define LED_SERVICE_HPP
#include "AbstractService.hpp"
#include "esp_gatts_api.h" // Include the header for GATT server API

class LedService : public AbstractService {
public:
    // Override methods from IService#
    static LedService* getInstance();


    enum TASK_T {
        SWITCH = 1, // Task for LED characteristic
    };

    const char* getTaskName(int task_t) const override;
    static esp_gatt_srvc_id_t serviceId; // Declaration of the static member
    static uint16_t serviceHandle; // Handle for the LED service
    static LedService* instance;  // Static instance pointer


    void handleReadRequest(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param) override;
    void handleCreateRequest(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param) override;
    void handleRegisterRequest(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param) override;

    static esp_bt_uuid_t ledServiceSwitchUuid; // Static UUID for LED service
    static esp_attr_value_t ledServiceSwitchAttribute; // Static attribute value for LED switch characteristic
    
private:
    LedService() = default;
    ~LedService() override;
    LedService(const LedService&) = delete; // Disable copy constructor
    LedService& operator=(const LedService&) = delete; // Disable assignment operator
    LedService(LedService&&) = delete; // Disable move constructor
    LedService& operator=(LedService&&) = delete; // Disable move assignment operator
};

static void LedServicecallBack(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    LedService* service =  LedService::getInstance();
    if(service == nullptr) {
        ESP_LOGE("LedService", "Service instance is null");
        return;
    }
    switch (event) {
        case ESP_GATTS_REG_EVT: {
            ESP_LOGI("LedService", "GATT server registered, status %d, app_id %d, gatts_if %d", param->reg.status, param->reg.app_id, gatts_if);
            esp_ble_gatts_create_service(gatts_if, &LedService::serviceId, 4);
            break;
        }

        case ESP_GATTS_READ_EVT: {
            service->handleReadRequest(gatts_if, param);
            break;
        }

        case ESP_GATTS_WRITE_EVT: {
            ESP_LOGI("LedService", "Write event received, conn_id %d, handle %d, value len %d",
                     param->write.conn_id, param->write.handle, param->write.len);
            // Handle write event
            break;
        }

        case ESP_GATTS_CREATE_EVT: {
            service->handleCreateRequest(gatts_if, param);
            break;
        }

        case ESP_GATTS_MTU_EVT: {
            ESP_LOGI("LedService", "MTU event received, MTU size %d", param->mtu.mtu);
            break;
        }

        case ESP_GATTS_CONF_EVT: {
            ESP_LOGI("LedService", "Confirmation event received");
            break;
        }

        case ESP_GATTS_UNREG_EVT: {
            ESP_LOGI("LedService", "Unregister event received");
            break;
        }

        case ESP_GATTS_ADD_INCL_SRVC_EVT: {
            ESP_LOGI("LedService", "Included service added");
            break;
        }

        case ESP_GATTS_ADD_CHAR_EVT: {
            ESP_LOGI("LedService", "Characteristic added, handle %d", param->add_char.attr_handle);
            break;
        }

        case ESP_GATTS_ADD_CHAR_DESCR_EVT: {
            ESP_LOGI("LedService", "Characteristic descriptor added, handle %d", param->add_char_descr.attr_handle);
            break;
        }

        case ESP_GATTS_DELETE_EVT: {
            ESP_LOGI("LedService", "Service deleted");
            break;
        }

        case ESP_GATTS_START_EVT: {
            ESP_LOGI("LedService", "Service started");
            break;
        }

        case ESP_GATTS_STOP_EVT: {
            ESP_LOGI("LedService", "Service stopped");
            break;
        }

        case ESP_GATTS_CONNECT_EVT: {
            ESP_LOGI("LedService", "Client connected, conn_id %d", param->connect.conn_id);
            break;
        }

        case ESP_GATTS_DISCONNECT_EVT: {
            ESP_LOGI("LedService", "Client disconnected, conn_id %d", param->disconnect.conn_id);
            break;
        }

        case ESP_GATTS_OPEN_EVT: {
            ESP_LOGI("LedService", "Open event received");
            break;
        }

        case ESP_GATTS_CANCEL_OPEN_EVT: {
            ESP_LOGI("LedService", "Cancel open event received");
            break;
        }

        case ESP_GATTS_CLOSE_EVT: {
            ESP_LOGI("LedService", "Close event received");
            break;
        }

        case ESP_GATTS_LISTEN_EVT: {
            ESP_LOGI("LedService", "Listen event received");
            break;
        }

        case ESP_GATTS_CONGEST_EVT: {
            ESP_LOGI("LedService", "Congestion event received, congested %d", param->congest.congested);
            break;
        }

        case ESP_GATTS_RESPONSE_EVT: {
            ESP_LOGI("LedService", "Response event received");
            break;
        }

        case ESP_GATTS_CREAT_ATTR_TAB_EVT: {
            ESP_LOGI("LedService", "Attribute table created");
            break;
        }

        case ESP_GATTS_SET_ATTR_VAL_EVT: {
            ESP_LOGI("LedService", "Attribute value set");
            break;
        }

        case ESP_GATTS_SEND_SERVICE_CHANGE_EVT: {
            ESP_LOGI("LedService", "Service change sent");
            break;
        }

        default: {
            ESP_LOGI("LedService", "Unhandled event: %d", event);
            break;
        }
    }
}

// Static instance initialization

#endif // LED_SERVICE_HPP