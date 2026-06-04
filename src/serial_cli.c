#include "serial_cli.h"
#include "app_settings.h" 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Afisam meniul
void afiseaza_meniu(void) {
    printf("       === MENIU ===         \n");
    printf(" [1] Seteaza Duty si Frecventa\n");
    printf(" [2] Reset la Default (%d%%, %dHz)\n", DEFAULT_DUTY, DEFAULT_FREQ);
    printf(" [3] LED on\n");
    printf(" [4] LED off\n");
    printf(" [5] Restart ESP\n");
    printf("----------------------------------\n");
    printf(" >> Alege comanda: ");
    fflush(stdout);
}

// Citim numarul de la tastatura
static uint32_t citeste_numar_consola(void) {
    char buffer[16];
    int length = 0;
    memset(buffer, 0, sizeof(buffer));

    while (1) {
        int c = fgetc(stdin);
        
        if (c == EOF || c == 0xFF) { 
            vTaskDelay(10 / portTICK_PERIOD_MS); 
            continue; 
        }

        if (c == '\n' || c == '\r') { 
            if (length > 0) break; 
            continue; 
        }

        if (c >= '0' && c <= '9' && length < 15) {
            buffer[length++] = (char)c;
            putchar(c); 
            fflush(stdout);
        }
    }
    printf("\n");
    return atoi(buffer);
}

// Functie while(1) pentru comenzi de pe portul UART
void task_comenzi_seriala(void *pvParameters) {
    vTaskDelay(500 / portTICK_PERIOD_MS);
    
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    afiseaza_meniu();

    while (1) {
        int c = fgetc(stdin);
        
        if (c != EOF && c != 0xFF) {
            
            if (c == '\n') continue;

            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); 
            
            if (c == '\r') {
                afiseaza_meniu();
            } else {
                switch (c) {
                    case '1': 
                        printf("\n=== SETARE DUTY SI FRECVENTA ===\n");

                            printf("Introduceti noul Duty (0-100%%): "); 
                            fflush(stdout);
                            uint32_t d = citeste_numar_consola();
                        
                                printf("Introduceti noua Frecventa (Hz): "); 
                            fflush(stdout);
                            uint32_t f = citeste_numar_consola();
                        
                        aplica_setari_comanda_1(d, f);
                        break;
                    case '2': 
                        proceseaza_comanda_2(); 
                        break;
                    case '3': 
                        proceseaza_comanda_3(); 
                        break;
                    case '4': 
                        proceseaza_comanda_4(); 
                        break;
                    case '5': 
                        proceseaza_comanda_5(); 
                        break;
                    default:  
                        printf("\n>> [SISTEM] Comanda '%c' este invalida!\n", c); 
                        break;
                }
                afiseaza_meniu();
            }
        }
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}