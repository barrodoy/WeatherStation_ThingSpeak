#include "main_utils.h"

void setup() {
  Serial.begin(115200);
  ThingSpeak.begin(client);
  connectToWiFi(); // Connect to WiFi network
  setI2C();
  initSensors();
  temperature = readTemperature();
  humidity = readHumidity();
  pressure = readPressure();
  wind_speed = readWindSpeed();
  wind_direction = readWindDir();
  uploadData();
}

void loop() {
  readSensorData(temperature, humidity, pressure);
  readWindData(wind_direction ,wind_speed);

  if (sensors_flag && wind_dir_flag && wind_speed_flag) {
    if (!wifi_flag) {
      connectToWiFi();
      if (wifi_flag) {
        uploadData();
        disconnectFromWiFi();
      }
    } else {
      uploadData();
      disconnectFromWiFi();
    }
    resetFlags();
  }

  if (millis() - last_serial_print_time >= serial_print_interval) {
    last_serial_print_time = millis();
    printSensorData(temperature, humidity, pressure, wind_speed, wind_direction);
    printWiFiStatus();
  }
}