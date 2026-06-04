#include "app_settings.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

#define STORAGE_NAMESPACE "led_storage"

led_config_t setari_led;

// Salveaza setarile curente in flash
static esp_err_t salveaza_setari_nvs() {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) return err;

    nvs_set_u32(my_handle, "duty", setari_led.duty);
    nvs_set_u32(my_handle, "freq", setari_led.freq);
    nvs_set_u32(my_handle, "mode", setari_led.mode);
    
    err = nvs_commit(my_handle);
    nvs_close(my_handle);
    return err;
}

void init_nvs_si_incarca_setari(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &my_handle);
    
    if (err == ESP_OK) {
        nvs_get_u32(my_handle, "duty", &setari_led.duty);
        nvs_get_u32(my_handle, "freq", &setari_led.freq);
        nvs_get_u32(my_handle, "mode", &setari_led.mode);
        nvs_close(my_handle);
    } else {
        setari_led.duty = DEFAULT_DUTY;
        setari_led.freq = DEFAULT_FREQ;
        setari_led.mode = DEFAULT_MODE;
        salveaza_setari_nvs();
    }
    printf("[NVS] Setari curente: Mode=%lu, Duty=%lu%%, Freq=%luHz\n", 
            (unsigned long)setari_led.mode, (unsigned long)setari_led.duty, (unsigned long)setari_led.freq);
}

// Suprascriem setarile cu cele primite
void aplica_setari_comanda_1(uint32_t duty, uint32_t freq) {
    if (duty > 100) duty = 100;
    if (freq == 0) freq = 1;
    setari_led.duty = duty;
    setari_led.freq = freq;
    salveaza_setari_nvs();
    printf("\n>> [SISTEM] Setari actualizate: Duty=%lu%%, Freq=%luHz\n", setari_led.duty, setari_led.freq);
}

// Aplicam setarile dafault
void proceseaza_comanda_2(void) {
    setari_led.duty = DEFAULT_DUTY;
    setari_led.freq = DEFAULT_FREQ;
    salveaza_setari_nvs();
    printf("\n>> [SISTEM] Resetat la Default!\n");
}

// Pornim modul de blink + salvare flash
void proceseaza_comanda_3(void) {
    setari_led.mode = 1;
    salveaza_setari_nvs();
    printf("\n>> [SISTEM] LED on!\n");
}

// Oprim blink-ul + salvare flash
void proceseaza_comanda_4(void) {
    setari_led.mode = 0;
    salveaza_setari_nvs();
    printf("\n>> [SISTEM] LED off!\n");
}

// Resetare placuta 
void proceseaza_comanda_5(void) {
    printf("\n>> [SISTEM] Restartare in curs...\n");
    fflush(stdout);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    esp_restart();
}