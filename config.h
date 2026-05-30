#ifndef CONFIG_H
#define CONFIG_H

#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

#define FORCE_CONFIG_PIN 0  // GPIO0 (BOOT button) to reset WiFi
#define LED_PIN 2
// ========== MQTT Broker Configuration (DEFINITIONS) ==========
const char *mqtt_broker = "broker.hivemq.com";
const int mqtt_port = 1883;

// ========== Global Variables (DEFINITIONS with initial values) ==========
bool ENERGY_MEASURE_flag = 0;
volatile int sensorValue = 50;
float energy = 12.5;
bool shouldSaveConfig = false;
bool webPortalActive = false;
bool communicationActive = false;

// ========== MQTT Topics (DEFINITIONS - empty strings) ==========
String mqtt_sub_topic;
String mqtt_pub_topic;
String energy_topic;



// ========== WiFi Clients (DEFINITIONS) ==========
WiFiClient espClient;
PubSubClient mqtt_client(espClient);

// ========== Button & LED Variables (DEFINITIONS) ==========
unsigned long buttonPressStart = 0;
bool buttonPressed = false;
bool forceConfig = false;
unsigned long lastLedToggle = 0;
bool ledState = false;

// ========== FUNCTION DECLARATIONS ==========
bool loadConfig();
void saveConfig();
void saveConfigCallback();
bool checkBotID(String entered_id);
void connectToWiFi();
void ConfigMode();
void handleLEDToggle();
void connectToMQTTBroker();
void mqttCallback(char *topic, byte *payload, unsigned int length);
void sensorTask();

// ========== FUNCTION DEFINITIONS ==========

#endif