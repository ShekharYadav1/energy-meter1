#ifndef INCLUDE_H
#define INCLUDE_H

#include <WiFi.h>
#include <PubSubClient.h>
#include "MCM_BL0940.h"
#include "config.h"
#include <WiFiManager.h>
#include <esp_task_wdt.h>



// ========== Web Portal Callback ==========
// void configModeCallback(WiFiManager *myWiFiManager) {
//     Serial.println("\nAP Mode: Connect to ESP32-EnergyMeter (password: 12345678)");
//     Serial.println("Open browser to: 192.168.4.1");
//     webPortalActive = true;
// }

// ========== WiFi Connection ==========
void connectToWiFi() {
    WiFiManager wm;
   // wm.setAPCallback(configModeCallback);
    
    if (!wm.autoConnect("ESP32-EnergyMeter", "12345678")) {
        Serial.println("Failed to connect, restarting...");
        ESP.restart();
    }
    
    webPortalActive = false;
    Serial.println("\nWiFi Connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    digitalWrite(LED_PIN, HIGH);
    ledState = true;
   
}

// ========== Button to Reset WiFi (Hold 5 seconds) ==========
void checkButtonForReset() {
  
    pinMode(FORCE_CONFIG_PIN, INPUT_PULLUP);
    
    if (digitalRead(FORCE_CONFIG_PIN) == LOW) {
        if (!buttonPressed) {
            buttonPressed = true;
            buttonPressStart = millis();
            Serial.println("Hold button for 5 seconds to reset WiFi...");
        } else if (millis() - buttonPressStart >= 5000 && !forceConfig) {
            forceConfig = true;
            Serial.println("Resetting WiFi credentials...");
            
            for (int i = 0; i < 5; i++) {
                digitalWrite(LED_PIN, LOW);
                delay(200);
                digitalWrite(LED_PIN, HIGH);
                delay(200);
            }
            
            WiFiManager wm;
            wm.resetSettings();
            Serial.println("Credentials cleared! Restarting...");
            delay(1000);
            ESP.restart();
        }
    } else {
        buttonPressed = false;
        forceConfig = false;
    }
}

// ========== LED Toggle (2 seconds when WiFi connected) ==========
void handleLEDToggle() {
    if (!webPortalActive && WiFi.status() == WL_CONNECTED) {
        if (millis() - lastLedToggle > 2000) {
            lastLedToggle = millis();
            ledState = !ledState;
            digitalWrite(LED_PIN, ledState);
        }
    }
}

// ========== MQTT Connection ==========
void connectToMQTTBroker() {
    while (!mqtt_client.connected()) {
        String client_id = "esp32-" + String(WiFi.macAddress());
        
        if (mqtt_client.connect(client_id.c_str())) {
            mqtt_client.subscribe(mqtt_sub_topic);
            Serial.println("MQTT Connected");
            ENERGY_MEASURE_flag = 1;
        } else {
            Serial.print("MQTT Failed, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(" retry in 5 sec");
            delay(5000);
        }
    }
  
}

// ========== MQTT Callback ==========
void mqttCallback(char *topic, byte *payload, unsigned int length) {
    String message;
    for (int i = 0; i < length; i++) message += (char)payload[i];
    
    Serial.print("Received: ");
    Serial.println(message);
    
    if (message == "1") {
        digitalWrite(LED_PIN, LOW);
        ledState = false;
        mqtt_client.publish(mqtt_pub_topic, "ON");
    }
    if (message == "0") {
        digitalWrite(LED_PIN, HIGH);
        ledState = true;
        mqtt_client.publish(mqtt_pub_topic, "OFF");
    }
}

// ========== Sensor Task ==========
void sensorTask() {
    static unsigned long lastPublish = 0;
    
    if (millis() - lastPublish > 2000) {
        lastPublish = millis();
        
        String Sdata = String(sensorValue);
        String Edata = String(energy, 2);
        
        if (mqtt_client.connected()) {
            mqtt_client.publish(sensor_topic, Sdata.c_str());
            mqtt_client.publish(energy_topic, Edata.c_str());
            Serial.print("Published: ");
            Serial.print(Sdata);
            Serial.print(", ");
            Serial.println(Edata);
        }
        
        sensorValue++;
        energy += 0.1;
    }
   
}

#endif