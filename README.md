# ESP 32 Advanced LED Controller (ESP-IDF & FreeRTOS)

Acest proiect reprezintă un sistem embedded complet dezvoltat pentru microcontrollerele ESP32, folosind framework-ul oficial **ESP-IDF** și sistemul de operare în timp real **FreeRTOS**.

Aplicația controlează un LED prin semnale PWM, permițând ajustarea dinamică a parametrilor (Duty Cycle, Frecvență, Moduri de funcționare) prin două interfețe complet independente: un meniu interactiv (CLI) pe portul Serial și o conexiune de rețea prin pachete UDP. Toate preferințele utilizatorului sunt salvate în memoria nevolatilă (NVS) pentru a persista la repornire.

## 🚀 Funcționalități Principale

* **Arhitectură Multi-Threading (FreeRTOS):** Proiectul rulează 3 task-uri concurente (Serial CLI, UDP Listener, LED Controller) fără a bloca procesorul.
* **Control PWM Hardware (LEDC):** Generare precisă de semnale PWM folosind driverele hardware native ESP32.
* **Memorie Nevolatilă (NVS):** Salvarea și recuperarea automată a ultimelor setări (Duty, Frecvență, Mod) la fiecare ciclu de power-on.
* **Comunicație Wireless (UDP/Wi-Fi):** Conectare la rețea Wi-Fi (Mod Station) și ascultare pe un socket UDP (Port 3333) pentru comenzi de la distanță.
* **Interfață Command-Line (CLI):** Un meniu text interactiv accesibil via UART (Serial Monitor) cu protecție la input-uri greșite (Buffer Overflow, caractere invalide).

## 📂 Structura Proiectului

Codul este modularizat la standarde profesionale de C, separând logica de rețea, hardware și persistența datelor:

* `src/main.c` - Entry point-ul aplicației; inițializează componentele și pornește task-urile FreeRTOS.
* `src/app_settings.c` - Gestionează partiția NVS și logica de procesare a comenzilor.
* `src/led_driver.c` - Configurează timer-ul și canalul PWM (LEDC) și aplică fizic setările pe hardware.
* `src/wifi_udp.c` - Gestionează stiva LwIP, conexiunea Wi-Fi Station și socket-ul UDP.
* `src/serial_cli.c` - Desenează interfața interactivă în terminal și procesează input-ul utilizatorului.

## 🛠️ Cerințe și Instalare

Acest proiect a fost dezvoltat folosind **PlatformIO** (sau direct prin extensia ESP-IDF din VS Code).

1. Clonează acest repository:
   ```bash
   git clone [https://github.com/USER_TA/NUME_PROIECT.git](https://github.com/USER_TA/NUME_PROIECT.git)
   ```
2. Deschide proiectul in VS Code cu PlatformIO instalat.

3. Configureaza datele retelei Wi-Fi in src/wifi_udp.c:
    ```bash
    #define WIFI_SSID "Nume_Retea"
    #define WIFI_PASS "Parola_Retea"
    ```

4. Pentru o compilare corecta asigura-te ca ai urmatoarea setare in platformio.ini: monitor_speed = 115200.

5. Compileaza si uploadeaza codul (Build & Upload).

## 💻 Mod de Utilizare
    Proiectul are 5 comenzi principale (atat din terminal Serial, cat si prin retea):
1. Setează Duty și Frecvență: Ajustează intensitatea (0-100%) și viteza de clipire (Hz).
    Exemplu utilizare via UDP: Trimițând textul 1 80 5 din aplicația Packet Sender către IP-ul ESP-ului.

2. Reset la Default: Revine la setările sigure de fabrică (50%, 2Hz).

3. Mod BLINK: Pornește generarea semnalului PWM.

4. Mod OFF: Oprește semnalul și stinge LED-ul.

5. Restart ESP: Execută un software reset controlat (esp_restart).

## 📡 Testare prin Rețea (UDP)
    După conectarea plăcuței la Wi-Fi, terminalul va afișa adresa IP alocată (ex: 192.168.1.100). Poți folosi un utilitar precum Packet Sender de pe PC sau telefon:

-Adresa: IP-ul afișat în terminal
-Port: 3333
-Protocol: UDP
-ASCII: 3 (pentru a porni LED-ul, de exemplu)