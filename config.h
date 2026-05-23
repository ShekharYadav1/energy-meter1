#ifndef CONFIG_H
#define CONFIG_H

#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

#define FORCE_CONFIG_PIN 0  // GPIO0 (BOOT button) to reset WiFi
#define LED_PIN 2

// Default values
extern bool ENERGY_MEASURE_flag;
extern volatile int sensorValue;
extern float energy;
extern bool shouldSaveConfig;

// MQTT Broker Configuration
const char *mqtt_broker = "broker.hivemq.com";
const int mqtt_port = 1883;

// MQTT Topics (will be created dynamically with Bot ID)
extern String mqtt_sub_topic;
extern String mqtt_pub_topic;
extern String sensor_topic;
extern String energy_topic;

// Telegram Bot ID (will be stored)
extern String bot_id;

// MQTT & WiFi Client Setup
extern WiFiClient espClient;
extern PubSubClient mqtt_client;

#endif