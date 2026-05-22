#include"include.h"



void setup() {

Serial.begin(115200);
pinMode(LED_PIN,OUTPUT);
digitalWrite(LED_PIN, LOW);

mqtt_client.setServer(mqtt_broker, mqtt_port);  //Configures MQTT broker details.
mqtt_client.setCallback(mqttCallback);          //When MQTT message arrives,automatically call mqttCallback()

}

void loop() {

  checkButtonForReset();  // Check if button held for 5 seconds
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