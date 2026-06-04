#pragma once
#include <stdint.h>
#include "esp_err.h"

// Setarile default pentru LED
#define DEFAULT_DUTY 50      
#define DEFAULT_FREQ 2       
#define DEFAULT_MODE 0       

// Structura pentru setarile LED-ului
typedef struct {
    uint32_t duty;
    uint32_t freq;
    uint32_t mode; 
} led_config_t;

// Variabila globala
extern led_config_t setari_led;

// Functii 
void init_nvs_si_incarca_setari(void);
void aplica_setari_comanda_1(uint32_t duty, uint32_t freq);
void proceseaza_comanda_2(void);
void proceseaza_comanda_3(void);
void proceseaza_comanda_4(void);
void proceseaza_comanda_5(void);