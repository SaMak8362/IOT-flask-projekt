/*
 * =============================================================================
 * SKÚŠKA — Programovanie (varianta B) — ESP32 LED Controller
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
 *          Server vracia JSON { "stav": true/false }. Podľa toho LED zapne/vypne.
 * =============================================================================
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// === Konfigurácia — DOPLŇTE VLASTNÉ HODNOTY ===
const char* WIFI_SSID     = "Halal";           // ← ZMENA NA VLASTNÚ SIEŤ
const char* WIFI_PASSWORD = "cwqo6020";          // ← ZMENA NA VLASTNÉ HESLO
const String API_URL      = "https://vas-app.azurewebsites.net/api/led-stav"; // ← sem dáš svoju Azure URL

const int LED_PIN = 2;

void setup() {
    Serial.begin(115200);
    delay(1000);

    // OPRAVA #1: LED musí byť nastavená ako OUTPUT
    // Chyba bola, že pin bol nastavený ako INPUT, takže LED nereagovala na digitalWrite.
    pinMode(LED_PIN, OUTPUT);

    Serial.print("Pripajam sa k Wi-Fi");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
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
        DeserializationError err = deserializeJson(doc, payload);

        if (!err) {
            // OPRAVA #2: Správny kľúč v JSON-e je "stav", nie "status"
            // Backend vracia {"stav": true/false}, takže doc["status"] vracalo null.
            bool stav = doc["stav"];

            // OPRAVA #3: LED sa má zapnúť/vypnúť podľa stavu zo servera
            // Pôvodne sa vždy nastavovalo LOW (vypnuté) bez ohľadu na stav.
            digitalWrite(LED_PIN, stav ? HIGH : LOW);
            
            Serial.print("LED stav: ");
            Serial.println(stav ? "ZAPNUTA" : "VYPNUTA");
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