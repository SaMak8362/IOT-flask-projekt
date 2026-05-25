"""
Backend pre ESP32 LED ovládanie — VERZIA 2.
Nasaďte na Azure App Service. URL potom vložte do esp32_led.ino do premennej API_URL.

Endpointy:
  GET  /api/svetlo          -> {"led": true/false}     (toto číta ESP32)
  POST /api/svetlo/prepni   -> {"led": true/false}     (toto volá tlačidlo na frontende)
  GET  /                    -> frontend.html (tlačidlo + zobrazenie stavu)
"""

from flask import Flask, jsonify, send_from_directory

app = Flask(__name__)

stav_led = False


@app.route("/api/svetlo")
def svetlo():
    return jsonify({"led": stav_led})


@app.route("/api/svetlo/prepni", methods=["POST"])
def prepni():
    global stav_led
    stav_led = not stav_led
    return jsonify({"led": stav_led})


@app.route("/")
def index():
    return send_from_directory(".", "frontend.html")


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8000, debug=True)
