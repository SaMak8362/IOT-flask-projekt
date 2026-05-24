"""
Backend pre ESP32 LED ovládanie.
Nasaďte na Azure App Service. URL potom vložte do esp32_led.ino do premennej API_URL.

Endpointy:
  GET  /api/led-stav     -> {"stav": true/false}     (toto číta ESP32)
  POST /api/led-prepni   -> {"stav": true/false}     (toto volá tlačidlo na frontende)
  GET  /                 -> frontend.html (tlačidlo + zobrazenie stavu)
"""

from flask import Flask, jsonify, send_from_directory

app = Flask(__name__)

stav_led = False  # držaný len v pamäti — pre skúšku stačí


@app.route("/api/led-stav")
def led_stav():
    return jsonify({"stav": stav_led})


@app.route("/api/led-prepni", methods=["POST"])
def led_prepni():
    global stav_led
    stav_led = not stav_led
    return jsonify({"stav": stav_led})


@app.route("/")
def index():
    return send_from_directory(".", "frontend.html")


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8000, debug=True)
