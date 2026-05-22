#include <ESP8266WiFi.h>      // Library to handle WiFi connections
#include <PubSubClient.h>     // Library for MQTT protocol support
#include "MCM_BL0940.h"

BL0940 bl0940;

#define LED_PIN 2        // Change if your board uses another pin
//#define SENSOR_PIN 3     // sensor data
#define ENERGY_MEASURE 1

volatile int sensorValue = 50;
       float energy = 12.5;


const char* ssid = "motog96";
const char* password = "yadav1234";

// ---------- MQTT Broker Configuration ----------
const char *mqtt_broker = "broker.emqx.io";                            // Public MQTT broker address broker.hivemq.com
const char *mqtt_sub_topic = "mcuSub";                                    // Topic the MCU subscribes to
const char *mqtt_pub_topic = "mcuPub";                                   // Topic where the MCU publishes
const char *sensor_topic = "sensorData";
const char *energy_topic = "energyData";
const char *wifi_topic = "wifistatus";
const int mqtt_port = 1883;                                             // MQTT port


// ---------- MQTT & WiFi Client Setup ----------
WiFiClient espClient;                  
PubSubClient mqtt_client(espClient);  



// ---------- Connect to WiFi Function ----------

 void connectToWiFi()
  {
    WiFi.mode(WIFI_STA);                   // Station mode, connect to router
    WiFi.begin(ssid, password);   

  Serial.print("Connecting to WiFi");
   
 

  while (WiFi.status() != WL_CONNECTED) {
    
  digitalWrite(LED_PIN,HIGH);
  delay(80);
  digitalWrite(LED_PIN,0);
  delay(80);
  Serial.print(".");
 
  
  }
  Serial.println();
  Serial.println("WiFi Connected");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_PIN,1);

 }

// ---------- Connect to MQTT Broker Function ----------

void connectToMQTTBroker() {                 
  
  while (!mqtt_client.connected()) {
    String client_id = "esp8266-client-" + String(WiFi.macAddress());
    Serial.printf("Connecting to MQTT Broker as %s...\n", client_id.c_str());

    if (mqtt_client.connect(client_id.c_str()))

    {
      mqtt_client.publish(wifi_topic,"ON");
      Serial.println("Connected to MQTT broker");
      mqtt_client.subscribe(mqtt_sub_topic);         // subscribe mcu from server
    }
     else 
    {
        mqtt_client.publish(wifi_topic,"OF");
      Serial.print("Failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


// ---------- MQTT Callback Function ----------
void mqttCallback(char *topic, byte *payload, unsigned int length)    // when broker publish data
{
  String message;

  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Received: ");
  Serial.println(message);

  if (message == "1") {
    digitalWrite(LED_PIN, LOW);   // LED ON
    mqtt_client.publish(mqtt_pub_topic,"ON");   // ACK from mcu to server
  }

  if (message == "0") {
    digitalWrite(LED_PIN, HIGH);  // LED OFF
    mqtt_client.publish(mqtt_pub_topic,"OFF");
  }
}
void sensorTask()
{
  static unsigned long lastPublish = 0;

  if (millis() - lastPublish > 2000)
  {
    lastPublish = millis();

  float voltage;
  bl0940.getVoltage( &voltage );
  Serial.printf("%.2f [V]\n", voltage );

  float current;
  bl0940.getCurrent( &current );
  Serial.printf("%.4f [A]\n", current );

  float activePower;
  bl0940.getActivePower( &activePower );
  Serial.printf("%.2f [W]\n", activePower );

  float activeEnergy;
  bl0940.getActiveEnergy( &activeEnergy );
  Serial.printf("%.6f [kWh]\n", activeEnergy );

  float powerFactor;
  bl0940.getPowerFactor( &powerFactor );
  Serial.printf("%.1f [%%]\n", powerFactor );

  float temperature;
  bl0940.getTemperature( &temperature );
  Serial.printf("%.1f [deg C]\n", temperature );

  Serial.println("");
  delay(1000);

    mqtt_client.publish(sensor_topic, Sdata.c_str());
    mqtt_client.publish(energy_topic, Edata.c_str());

    Serial.print("Sensor Data : ");
   
  }
}


void setup() {

Serial.begin(115200);
pinMode(LED_PIN,OUTPUT);
digitalWrite(LED_PIN, HIGH);
mqtt_client.setServer(mqtt_broker, mqtt_port);  //Configures MQTT broker details.
mqtt_client.setCallback(mqttCallback);          //When MQTT message arrives,automatically call mqttCallback()


 //bl0940.begin(Serial1, RX, TX); // RX pin - TX pin 
  bl0940.Reset();
  bl0940.setFrequency(60); //50[Hz]
  bl0940.setUpdateRate(800); //400[ms]

}

void loop() {

    if (WiFi.status() != WL_CONNECTED) 
    {
    connectToWiFi();
     }

    if (!mqtt_client.connected())
     {
    connectToMQTTBroker();
    }
 mqtt_client.loop();
   


   
#if ENERGY_MEASURE
  sensorTask();
#endif

}