#ifndef ISERVICE_HPP
#define ISERVICE_HPP

#include <cstdint>
#include <unordered_map>
#include <functional> // Include the header defining std::function
#include "esp_gatts_api.h" // Include the header defining esp_gatts_cb_event_t
#include "esp_log.h"
#include <string>
#include "cJSON.h"


class AbstractService; // Forward declaration of AbstractService
struct Characteristic {
    esp_bt_uuid_t uuid; // Correct type for a characteristic UUID
    uint16_t handle; // Handle for the characteristic
    esp_gatt_perm_t perm; // Permissions for the characteristic
    esp_gatt_char_prop_t property; // Properties of the characteristic
    esp_attr_value_t atttrValue; // Attribute value for the characteristic
    esp_attr_control_t attrControl; // Attribute control for the characteristic
    std::string descriptorValue; // Descriptor value for the characteristic    
    AbstractService* service; // Pointer to the service this characteristic belongs to
};

class AbstractService {
public:
    virtual ~AbstractService() = default;    

    // Handle for the GATT service
    virtual const char* getTaskName(int TASK_T) const = 0;
    
    virtual void handleRegisterRequest(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param)
    {
        ESP_LOGI("AbstractService", "Register event received");
    }

    virtual void handleReadRequest(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param)
    {
        ESP_LOGI("AbstractService", "Read event received");
    }

    virtual void handleCreateRequest(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param)
    {
        ESP_LOGI("AbstractService", "Create event received");
    }

    // ESP_GATTS_WRITE_EVT
    virtual void handleWriteRequest(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param)
    {
        ESP_LOGI("AbstractService", "Write event received");
    }

    virtual void handleMTURequest(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param)
    {
        ESP_LOGI("AbstractService", "MTU event received");
    }

    virtual void handleConfirmRequest(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param)
    {
        ESP_LOGI("AbstractService", "Confirm event received");
    }
    
    virtual void handleUnregRequest(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param)
    {
        ESP_LOGI("AbstractService", "Unregister event received");
    }

    virtual void handleDeleteRequest(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param)
    {
        ESP_LOGI("AbstractService", "Delete event received");
    }

    virtual void handleStartRequest(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param)
    {
        ESP_LOGI("AbstractService", "Start event received");
    }

    virtual void handleStopRequest(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param)
    {
        ESP_LOGI("AbstractService", "Stop event received");
    }

    virtual void handleConnectRequest(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param)
    {
        ESP_LOGI("AbstractService", "Connect event received");
    }

    virtual void handleDisconnectRequest(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param)
    {
        ESP_LOGI("AbstractService", "Disconnect event received");
    }
    
    virtual void handleOpenRequest(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param)
    {
        ESP_LOGI("AbstractService", "Open event received");
    }

    virtual void handleCloseRequest(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param)
    {
        ESP_LOGI("AbstractService", "Close event received");
    }

    virtual void handleCongestRequest(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param)
    {
        ESP_LOGI("AbstractService", "Congest event received");
    }

    virtual void handleResponseRequest(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param)
    {
        ESP_LOGI("AbstractService", "Response event received");
    }
    
    void handleEvent(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param) 
    {
        // Check if the event is in the map and call the corresponding function
        switch (event) {
            case ESP_GATTS_REG_EVT: {
                ESP_LOGI("LedService", "GATT server registered, status %d, app_id %d, gatts_if %d", param->reg.status, param->reg.app_id, gatts_if);
                instance->handleRegisterRequest(gatts_if, param);
                break;
            }
    
            case ESP_GATTS_READ_EVT: {
                instance->handleReadRequest(gatts_if, param);
                break;
            }
    
            case ESP_GATTS_WRITE_EVT: {
                ESP_LOGI("LedService", "Write event received, conn_id %d, handle %d, value len %d",
                         param->write.conn_id, param->write.handle, param->write.len);
                // Handle write event
                break;
            }
    
            case ESP_GATTS_CREATE_EVT: {
                instance->handleCreateRequest(gatts_if, param);
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

    virtual Characteristic createReadWriteCharacteristic(uint16_t task, esp_ble_gatts_cb_param_t* param, const esp_bt_uuid_t uuid, const esp_attr_value_t& attrValue) 
    {

        Characteristic newCharacteristic;
        newCharacteristic.handle = param->create.service_handle; // Use the handle from the event parameter
        newCharacteristic.uuid = uuid;
        newCharacteristic.atttrValue = attrValue;        
        //newCharacteristic.descriptorValue = descriptorValue; // Use the task name as the descriptor value
        
        handleToCharacteristic[newCharacteristic.handle] = newCharacteristic; // Store the characteristic in the map
        newCharacteristic.service = this; // Set the service pointer
        taskToHandle[task] = newCharacteristic.handle; // Map the task to the characteristic handle

        ESP_LOGI("AbstractService", "Creating characteristic with UUID: handle: %d", 
                newCharacteristic.handle);

        ESP_ERROR_CHECK(esp_ble_gatts_add_char(newCharacteristic.handle, 
            &newCharacteristic.uuid, 
            ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, 
            ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE | ESP_GATT_CHAR_PROP_BIT_NOTIFY, 
            &newCharacteristic.atttrValue, 
        NULL)
        );

        return newCharacteristic;
    }

    void setServiceHandle(uint16_t handle) 
    {
        serviceHandle = handle;
    }

    // not mutable
    esp_gatt_srvc_id_t serviceId;
    
    std::unordered_map<uint16_t, uint16_t> taskToHandle; // Map to store characteristics by task
    std::unordered_map<uint16_t, Characteristic> handleToCharacteristic; // Map to store characteristics by handle

    void setInstance(AbstractService* service) {
        instance = service; // Set the static instance pointer to the provided service
    }

    AbstractService* instance = nullptr; // Pointer to the instance of the service

    // Handle for the GATT service
    uint16_t serviceHandle;
    
};



#endif // BLE_SERVER_H