#include "wifi_udp.h"
#include "app_settings.h"
#include <stdio.h>
#include <string.h>
#include "serial_cli.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "lwip/sockets.h"

// Setari retea  
#define WIFI_SSID "Nume_Retea"
#define WIFI_PASS "Parola_Retea"
#define UDP_PORT 3333

// Functie prin care ne conectam la wifi 
static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
        printf("[WIFI] Se reincearca conexiunea...\n");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        printf("\n[WIFI] Conectat! IP-ul tau este: " IPSTR "\n", IP2STR(&event->ip_info.ip));
        printf("[WIFI] Poti trimite comenzi UDP la acest IP pe portul %d\n", UDP_PORT);
    }
}

void initializare_wifi(void) {
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, NULL);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();
}

// Asteapta comenzi prin packet sender
void task_comenzi_udp(void *pvParameters) {
    char rx_buffer[128];
    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(UDP_PORT);

    while (1) {
        // Facem un socket de tip UDP
        int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
        if (sock < 0) { vTaskDelay(1000 / portTICK_PERIOD_MS); continue; }
        
        // Legam socketul de adresa
        if (bind(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
            close(sock); vTaskDelay(1000 / portTICK_PERIOD_MS); continue;
        }

        while (1) {
            struct sockaddr_storage source_addr;
            socklen_t socklen = sizeof(source_addr);

            // Asteapta un mesaj UDP
            int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);

            if (len > 0) {
                rx_buffer[len] = 0; 
                printf("\n[UDP] Comanda primita: %s", rx_buffer);
                
                char comanda = rx_buffer[0];
                switch (comanda) {
                    case '1': 
                        uint32_t d = DEFAULT_DUTY, f = DEFAULT_FREQ;
                        sscanf(rx_buffer, "1 %lu %lu", &d, &f);
                        aplica_setari_comanda_1(d, f);
                        break;
                    case '2': proceseaza_comanda_2(); break;
                    case '3': proceseaza_comanda_3(); break;
                    case '4': proceseaza_comanda_4(); break;
                    case '5': proceseaza_comanda_5(); break;
                    default:  printf("\n>> [UDP] Comanda necunoscuta!\n"); break;
                }
            }
        }
    }
}
