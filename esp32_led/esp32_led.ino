/*
 * =============================================================================
 * SKÚŠKA — Programovanie (varianta B) — ESP32 LED Controller [VERZIA 2]
 * =============================================================================
 *
 * Tento súbor obsahuje 3 ZÁMERNÉ CHYBY.
 * Vašou úlohou je nájsť ich, opraviť a nad opraveným riadkom napísať:
 *     // OPRAVA #N: <vlastnými slovami prečo to bola chyba>
 *
 * Hint: žiadna chyba NIE JE v konfigurácii Wi-Fi alebo URL — tie nahraďte
 *       vlastnými hodnotami a viac sa o ne nestarajte.
 *
 * Hardvér: ESP32 + LED na pin 2 (môže byť aj zabudovaná LED)
 * Logika:  ESP32 každé 2 s číta stav LED zo servera (váš Flask na Azure).
 *          Server vracia JSON { "led": true/false }. Podľa toho LED zapne/vypne.
 * =============================================================================
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// === Konfigurácia — DOPLŇTE VLASTNÉ HODNOTY ===
const char* WIFI_SSID     = "Halal";
const char* WIFI_PASSWORD = "cwqo6020";
const String API_URL      = "hhttps://kalk-hvgwb2eechdvc3ca.polandcentral-01.azurewebsites.net/api/led-stav";
const int LED_PIN = 2;

void setup() {
    Serial.begin(115200);
    delay(1000);

    pinMode(LED_PIN, OUTPUT);

    Serial.print("Pripajam sa k Wi-Fi");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    //while (WiFi.status() == WL_CONNECTED) {   ked je pripojene tak čaka treba ked nie je pripojene tak čaka
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" pripojene!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Wi-Fi odpojene, cakam...");
        delay(2000);
        return;
    }

    HTTPClient http;
    http.begin(API_URL);
    int httpCode = http.GET();

    if (httpCode == 200) {
        String payload = http.getString();
        Serial.print("Odpoved: ");
        Serial.println(payload);

        JsonDocument doc;
        //DeserializationError err = deserializeJson(doc, API_URL); nemože to byt cez URL adresu
        DeserializationError err = deserializeJson(doc, payload);

        if (!err) {
            bool stav = doc["led"];
            //digitalWrite(LED_PIN, stav ? LOW : HIGH);  musí to byť preobratene lebo inakšie  keď sa zapneme na stranke tak sa vypne
            digitalWrite(LED_PIN, stav ? HIGH: LOW);
        } else {
            Serial.print("Chyba parsovania JSON: ");
            Serial.println(err.c_str());
        }
    } else {
        Serial.print("HTTP chyba: ");
        Serial.println(httpCode);
    }

    http.end();
    delay(2000);
}
