#ifndef CONFIG_H
#define CONFIG_H



#define FORCE_CONFIG_PIN 0  // GPIO0 (BOOT button) to reset WiFi
#define LED_PIN 2



extern bool ENERGY_MEASURE_flag=0;

volatile int sensorValue = 50;
float energy = 12.5;
unsigned long lastLedToggle = 0;
bool ledState = false;

// Button press variables for WiFi reset
unsigned long buttonPressStart = 0;
bool buttonPressed = false;
bool forceConfig = false;

unsigned long lastWifiDisconnectBlink = 0;
bool webPortalActive = false;
unsigned long lastWebPortalBlink = 0;

// MQTT Broker Configuration
const char *mqtt_broker = "broker.hivemq.com";
const char *mqtt_sub_topic = "mcuSub";
const char *mqtt_pub_topic = "mcuPub";
const char *sensor_topic = "sensorData";
const char *energy_topic = "energyData";
const char *wifi_topic = "wifistatus";
const int mqtt_port = 1883;

// MQTT & WiFi Client Setup
WiFiClient espClient;                  
PubSubClient mqtt_client(espClient); 

#endif