#include"include.h"


void setup() {
    Serial.begin(115200);
    Serial.println("\n\nESP32 Energy Meter Starting...");
    
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
    
    // Initialize SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
    }
    
    // Try to load existing config first
    if (!loadConfig()) {
        Serial.println("No config found, will request Bot ID during WiFi setup");
    }
    
    // Connect to WiFi (will ask for Bot ID if not saved)
    connectToWiFi();
    
    // Setup MQTT
    mqtt_client.setServer(mqtt_broker, mqtt_port);
    mqtt_client.setCallback(mqttCallback);
    
    Serial.println("Setup complete!");
}

void loop() {

   ConfigMode(); 
   handleLEDToggle();

    if (WiFi.status() != WL_CONNECTED) 
    {
    connectToWiFi();
     }

    if (!mqtt_client.connected())
     {
    connectToMQTTBroker();
    }
 mqtt_client.loop();
 
if(ENERGY_MEASURE_flag)
 {  
  sensorTask();
 }
     delay(10);  // Small delay to prevent watchdog starvation


}