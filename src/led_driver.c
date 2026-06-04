#include "led_driver.h"
#include "app_settings.h" 
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

#define LED_PIN 4

void init_led_pwm(void) {
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = LEDC_TIMER_13_BIT, 
        .freq_hz          = setari_led.freq == 0 ? 1 : setari_led.freq,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .timer_sel      = LEDC_TIMER_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = LED_PIN,
        .duty           = 0, 
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);
}


void task_control_led(void *pvParameters) {
    uint32_t ultima_frecventa = 0;
    uint32_t ultimul_duty = 0;
    uint32_t ultimul_mod = 99; 

    while (1) {
        if (setari_led.freq != ultima_frecventa && setari_led.freq > 0) {
            ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0, setari_led.freq);
            ultima_frecventa = setari_led.freq;
        }
        
        if (setari_led.mode == 1) {
            uint32_t duty_calculat = (setari_led.duty * 8191) / 100;
            if (duty_calculat != ultimul_duty || ultimul_mod != setari_led.mode) {
                ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty_calculat);
                ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
                ultimul_duty = duty_calculat;
            }
        } else {
            if (ultimul_mod != setari_led.mode) {
                ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
                ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
                ultimul_duty = 0;
            }
        }
        
        ultimul_mod = setari_led.mode;
        vTaskDelay(50 / portTICK_PERIOD_MS); 
    }
}