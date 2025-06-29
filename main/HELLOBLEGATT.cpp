#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_main.h"    
#include "driver/gpio.h"

#include "../include/BLEServer.hpp"
#include "../include/BLEUtils.hpp"


extern "C" {

#define BLINK_GPIO GPIO_NUM_17
BLEServer bleServer;

int ledLevel = 0; // LED level variable



void initLed()
{
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}

 void onCharacteristicWrite(const esp_ble_gatts_cb_param_t& param) {
    if (param.write.handle == BLEServer::led_char_handle) {
        if (!param.write.is_prep) {
            uint8_t value = param.write.value[0]; // Read the value written to the characteristic
            ESP_LOGI("BLE", "LED Level set to: %d", value);
            gpio_set_level(BLINK_GPIO, value); // Set the LED level (1 = ON, 0 = OFF)
        }
    }
}

void app_main(void)
{
    initLed();
    bleServer.startServer(onCharacteristicWrite);
}




} // extern "C"