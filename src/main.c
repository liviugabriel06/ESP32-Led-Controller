#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h" 

#include "app_settings.h"
#include "led_driver.h"
#include "wifi_udp.h"
#include "serial_cli.h"

void app_main(void) {
    esp_log_level_set("*", ESP_LOG_WARN);

    // Initializam NVS ul + setarile salvate / default
    init_nvs_si_incarca_setari();

    init_led_pwm();
    
    // Initializam reteaua wifi
    initializare_wifi();

    xTaskCreate(task_comenzi_seriala, "task_comenzi_seriala", 4096, NULL, 10, NULL);
    xTaskCreate(task_comenzi_udp, "task_comenzi_udp", 4096, NULL, 10, NULL);
    xTaskCreate(task_control_led, "task_control_led", 2048, NULL, 5, NULL);
}