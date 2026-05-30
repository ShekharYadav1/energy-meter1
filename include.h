#ifndef INCLUDE_H
#define INCLUDE_H

#include <PubSubClient.h>
#include <WiFiManager.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "MCM_BL0940.h"
#include "config.h"
#include "htmlfile.h"  // Include the HTML file


const String Device_Bot_ID = "2026";  // ← ONLY THIS BOT ID WILL WORK


String BOT_ID = "";


// ========== Load Config from SPIFFS ==========
bool loadConfig() {
    Serial.println("Loading saved configuration...");
    
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS mount failed");
        return false;
    }
    
    if (!SPIFFS.exists("/config.json")) {
        Serial.println("Config file not found");
        SPIFFS.end();
        return false;
    }
    
    File configFile = SPIFFS.open("/config.json", "r");
    if (!configFile) {
        Serial.println("Failed to open config file");
        SPIFFS.end();
        return false;
    }
    
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, configFile);
    configFile.close();
    SPIFFS.end();
    
    if (error) {
        Serial.println("Failed to parse config file");
        return false;
    }
    
    if (doc.containsKey("bot_id")) {
        BOT_ID = doc["bot_id"].as<String>();
        Serial.println("Bot ID loaded: " + BOT_ID);
        
        // Check if saved Bot ID matches hardcoded one
        if (BOT_ID == Device_Bot_ID) {
            communicationActive = true;
            // Create MQTT topics
            mqtt_sub_topic = "mcuSub_" + Device_Bot_ID;
            mqtt_pub_topic = "mcuPub_" + Device_Bot_ID;
          //  sensor_topic = "sensorData_" + Device_Bot_ID;
            energy_topic = "energyData_" + Device_Bot_ID;
            Serial.println("Saved Bot ID matches hardcoded!");
            return true;
        } else {
            Serial.println("Saved Bot ID does NOT match hardcoded!");
            communicationActive = false;
            return false;
        }
    }
    
    return false;
}

// ========== Save Config to SPIFFS ==========
void saveConfig() {
    if (!shouldSaveConfig) return;
    
    Serial.println("Saving configuration...");
    
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS mount failed");
        return;
    }
    
    DynamicJsonDocument doc(1024);
    doc["bot_id"] = BOT_ID;
    
    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
        Serial.println("Failed to open config file for writing");
        SPIFFS.end();
        return;
    }
    
    serializeJson(doc, configFile);
    configFile.close();
    SPIFFS.end();
    
    Serial.println("Configuration saved successfully!");
    shouldSaveConfig = false;
}

// ========== Callback when config is saved ==========
void saveConfigCallback() {
    Serial.println("Configuration changed, saving...");
    shouldSaveConfig = true;
}

// ========== Simple Webpage for Bot ID Only (Using HTML from separate file) ==========
// ========== Combined WiFi + Bot ID Configuration Webpage ==========
void startConfigWebpage() {
    webPortalActive = true;
    WiFiServer server(80);
    server.begin();
    Serial.println("🌐 Configuration Webpage started - Go to: http://" + WiFi.localIP().toString());
    
    while (webPortalActive) {
        WiFiClient client = server.available();
        
        if (client) {
            String request = "";
            while (client.connected() && !client.available()) {
                delay(1);
            }
            
            while (client.available()) {
                request += (char)client.read();
            }
            
            // Handle form submission
            if (request.indexOf("POST /submit") != -1) {
                String submitted_bot_id = "";
                String submitted_ssid = "";
                String submitted_password = "";
                
                // Parse Bot ID
                int idStart = request.indexOf("bot_id=");
                if (idStart != -1) {
                    idStart += 7;
                    int idEnd = request.indexOf("&", idStart);
                    if (idEnd == -1) idEnd = request.indexOf(" ", idStart);
                    if (idEnd == -1) idEnd = request.indexOf("\r\n", idStart);
                    submitted_bot_id = request.substring(idStart, idEnd);
                    submitted_bot_id.trim();
                }
                
                // Parse SSID
                int ssidStart = request.indexOf("ssid=");
                if (ssidStart != -1) {
                    ssidStart += 5;
                    int ssidEnd = request.indexOf("&", ssidStart);
                    if (ssidEnd == -1) ssidEnd = request.indexOf(" ", ssidStart);
                    if (ssidEnd == -1) ssidEnd = request.indexOf("\r\n", ssidStart);
                    submitted_ssid = request.substring(ssidStart, ssidEnd);
                    submitted_ssid.trim();
                    // URL decode
                    submitted_ssid.replace("%20", " ");
                }
                
                // Parse Password
                int pwdStart = request.indexOf("password=");
                if (pwdStart != -1) {
                    pwdStart += 9;
                    int pwdEnd = request.indexOf(" ", pwdStart);
                    if (pwdEnd == -1) pwdEnd = request.indexOf("\r\n", pwdStart);
                    submitted_password = request.substring(pwdStart, pwdEnd);
                    submitted_password.trim();
                }
                
                Serial.println("\n📝 Configuration received:");
                Serial.println("   SSID: " + submitted_ssid);
                Serial.println("   Bot ID: " + submitted_bot_id);
                
                // Verify Bot ID
                if (submitted_bot_id == Device_Bot_ID) {
                    // MATCH! Save configuration
                    BOT_ID = submitted_bot_id;
                    communicationActive = true;
                    shouldSaveConfig = true;
                    saveConfig();
                    
                    // Create MQTT topics
                    mqtt_sub_topic = "mcuSub_" + Device_Bot_ID;
                    mqtt_pub_topic = "mcuPub_" + Device_Bot_ID;
                    energy_topic = "energyData_" + Device_Bot_ID;
                    
                    // Send success response
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println();
                    client.println(processHTML(successPageHTML, Device_Bot_ID));
                    client.stop();
                    
                    // Try to connect to WiFi with the provided credentials
                    WiFi.begin(submitted_ssid.c_str(), submitted_password.c_str());
                    int attempts = 0;
                    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
                        delay(500);
                        attempts++;
                        Serial.print(".");
                    }
                    
                    webPortalActive = false;
                    break;
                } else {
                    // Bot ID mismatch
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println();
                    client.println(processHTML(errorPageHTML, Device_Bot_ID));
                    client.stop();
                }
            }
            
            // Send configuration webpage
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            client.println(processHTML(configPageHTML, Device_Bot_ID));
            client.stop();
        }
        delay(10);
    }
    
    server.stop();
    webPortalActive = false;
}

// ========== WiFi Connection with Bot ID on SAME PAGE ==========
void connectToWiFi() {

    WiFiManager wm;
    
    // Create label text without concatenation
    char botLabel[50];
    snprintf(botLabel, sizeof(botLabel), "Bot ID - %s", Device_Bot_ID.c_str());
    
    // Create custom parameter for Bot ID (appears on same page as WiFi)
    WiFiManagerParameter custom_bot_id("bot_id", botLabel, "", 50);
    
    // Add the parameter to WiFiManager
    wm.addParameter(&custom_bot_id);
    
    // Set save config callback
    wm.setSaveConfigCallback(saveConfigCallback);
    
    // Set custom HTML for better UX
    std::vector<const char *> menu = {"wifi", "exit"};
    wm.setMenu(menu);
    
    // Try to auto-connect, if fails start config portal
    if (!wm.autoConnect("EnergyMeter", "12345678")) {
        Serial.println("Failed to connect, restarting...");
        ESP.restart();
    }
    
    // Get the entered Bot ID from the webpage
    String entered_bot_id = "";
    if (custom_bot_id.getValue() != NULL && strlen(custom_bot_id.getValue()) > 0) {
        entered_bot_id = String(custom_bot_id.getValue());
        Serial.print(" Entered Bot ID: ");
        Serial.println(entered_bot_id);
        Serial.print(" Hardcoded Bot ID: ");
        Serial.println(Device_Bot_ID);
        
        // ========== COMPARE HARDCODED BOT ID ==========
        if (entered_bot_id == Device_Bot_ID) {
            // MATCH! Save and continue
            BOT_ID = entered_bot_id;
            saveConfig();
            communicationActive = true;
            Serial.println(" Bot ID MATCHED! Communication ACTIVE");
           
             handleLEDToggle();

            // Create MQTT topics
            mqtt_sub_topic = "mcuSub_" + Device_Bot_ID;
            mqtt_pub_topic = "mcuPub_" + Device_Bot_ID;
           // sensor_topic = "sensorData_" + Device_Bot_ID;
            energy_topic = "energyData_" + Device_Bot_ID;
        } else {
            // MISMATCH! Start Bot ID only webpage
            communicationActive = false;
            Serial.println("BOT ID MISMATCH!");
            Serial.print("   Expected: ");
            Serial.println(Device_Bot_ID);
            Serial.print("   Received: ");
          //  Serial.println(entered_bot_id);
          //  Serial.println("\n🌐 Opening Bot ID verification webpage...");
        // Serial.println("\n🔴 Clearing WiFi credentials and restarting AP mode...");
          
          //  handleLEDToggle();
           // delay(1000);

          // Clear saved WiFi credentials
           wm.resetSettings();
    
            // Start simple Bot ID webpage
           // startConfigWebpage();
            
            // After webpage, check if communication is active
            if (!communicationActive) {
                Serial.println("Still no valid Bot ID. Restarting...");
                delay(2000);
                ESP.restart();
            }
        }
    } else {
        // No Bot ID entered, try to load from storage
        if (!loadConfig()) {
            Serial.println("No valid Bot ID configured! Starting Bot ID webpage...");
            startConfigWebpage();
            if (!communicationActive) {
                Serial.println("Restarting...");
                delay(2000);
                ESP.restart();
            }
        }
    }
    
    digitalWrite(LED_PIN, LOW);
    Serial.println("\n WiFi Connected!");
    Serial.print("📡 IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print(" SSID: ");
    Serial.println(WiFi.SSID());
    
    if (communicationActive) {
        Serial.print(" Active Bot ID: ");
        Serial.println(Device_Bot_ID);
        Serial.println(" Communication ACTIVE!");
        Serial.print("Publishing to: ");
      //  Serial.println(sensor_topic);
    }
}

// ========== Button to Reset WiFi (Hold 5 seconds) ==========
void ConfigMode() {
    static unsigned long buttonPressStart = 0;
    static bool buttonPressed = false;
    static bool resetTriggered = false;
    
    
    pinMode(FORCE_CONFIG_PIN, INPUT_PULLUP);
    
    if (digitalRead(FORCE_CONFIG_PIN) == LOW) {
        if (!buttonPressed) {
            buttonPressed = true;
            buttonPressStart = millis();
            resetTriggered = false;
            Serial.println(" Button PRESSED! Hold for 5 seconds to reset...");
        } else if (!resetTriggered && (millis() - buttonPressStart >= 5000)) {
            resetTriggered = true;
            Serial.println("\n RESETTING ALL SETTINGS! ");
            
            for (int i = 0; i < 10; i++) {
                digitalWrite(LED_PIN, LOW);
                delay(100);
                digitalWrite(LED_PIN, HIGH);
                delay(100);
            }
            
            // Clear SPIFFS config
            if (SPIFFS.begin(true)) {
                SPIFFS.remove("/config.json");
                SPIFFS.end();
            }
            
            WiFiManager wm;
            wm.resetSettings();
            Serial.println("All settings cleared! Restarting...");
            delay(1000);
            ESP.restart();
        }
    } else {
        if (buttonPressed && !resetTriggered && (millis() - buttonPressStart < 5000)) {
            Serial.println("Button released - Reset cancelled");
        }
        buttonPressed = false;
        resetTriggered = false;
    }
}

// ========== LED Toggle ==========
void handleLEDToggle() {
    // LED Toggle variables


    if (WiFi.status() == WL_CONNECTED && communicationActive) {
        if (millis() - lastLedToggle > 2000) {
            lastLedToggle = millis();
            ledState = !ledState;
            digitalWrite(LED_PIN, ledState);
        }
    }
}

// ========== MQTT Connection ==========
void connectToMQTTBroker() {
    if (!communicationActive) {
        static unsigned long lastPrint = 0;
        if (millis() - lastPrint > 5000) {
            lastPrint = millis();
            Serial.println("Communication inactive - Waiting for valid Bot ID...");
            Serial.print("   Bot ID must be: ");
            Serial.println(Device_Bot_ID);
        }
        return;
    }
    
    if (!mqtt_client.connected()) {
        String client_id = "esp32-" + String(WiFi.macAddress()) + "-" + Device_Bot_ID;
        
        if (mqtt_client.connect(client_id.c_str())) {
            mqtt_client.subscribe(mqtt_sub_topic.c_str());
            Serial.println("MQTT Connected!");
            Serial.print("Subscribed to: ");
            Serial.println(mqtt_sub_topic);
            ENERGY_MEASURE_flag = 1;
        } else {
            Serial.print(" MQTT Failed, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(" retry in 5 sec");
            delay(5000);
        }
    }
}

// ========== MQTT Callback ==========
void mqttCallback(char *topic, byte *payload, unsigned int length) {
    if (!communicationActive) return;
    
    String message;
    for (int i = 0; i < length; i++) message += (char)payload[i];
    
    Serial.print("Received on ");
    Serial.print(topic);
    Serial.print(": ");
    Serial.println(message);
    
    if (message == "1") {
        digitalWrite(LED_PIN, LOW);
        ledState = false;
        mqtt_client.publish(mqtt_pub_topic.c_str(), "ON");
        Serial.println("Published ON to: " + mqtt_pub_topic);
    }
    if (message == "0") {
        digitalWrite(LED_PIN, HIGH);
        ledState = true;
        mqtt_client.publish(mqtt_pub_topic.c_str(), "OFF");
        Serial.println(" Published OFF to: " + mqtt_pub_topic);
    }
}

// ========== Sensor Task ==========
void  sensorTask() {
    static unsigned long lastPublish = 0;
    
    if (!communicationActive) return;
    
    if (millis() - lastPublish > 2000) {
        lastPublish = millis();
        
        // ========== SIMULATED VALUES (Replace with actual sensor readings) ==========
        float voltage = 220.5;           // Voltage in Volts
        float current = 2.3;             // Current in Amps
        float powerFactor = 0.95;        // Power Factor (0-1)
        float temperature = 35.5;        // Temperature in Celsius
        float power = voltage * current * powerFactor;  // Power in Watts
        // ===========================================================================
        
        // Convert to Strings for MQTT publishing
        String voltageStr = String(voltage, 1);
        String currentStr = String(current, 2);
        String powerFactorStr = String(powerFactor, 2);
        String temperatureStr = String(temperature, 1);
        String powerStr = String(power, 1);
        String energyStr = String(energy, 2);
        
        if (mqtt_client.connected()) {
            // ========== PUBLISH TO INDIVIDUAL TOPICS ==========
            // Each parameter gets its own topic with Bot ID
            mqtt_client.publish(("voltage_" + Device_Bot_ID).c_str(), voltageStr.c_str());
            mqtt_client.publish(("current_" + Device_Bot_ID).c_str(), currentStr.c_str());
            mqtt_client.publish(("power_" + Device_Bot_ID).c_str(), powerStr.c_str());
            mqtt_client.publish(("powerfactor_" + Device_Bot_ID).c_str(), powerFactorStr.c_str());
            mqtt_client.publish(("temperature_" + Device_Bot_ID).c_str(), temperatureStr.c_str());
          //  mqtt_client.publish(sensor_topic.c_str(), sensorStr.c_str());
          //  mqtt_client.publish(energy_topic.c_str(), energyStr.c_str());
            
            // ========== OR PUBLISH AS JSON (Single Topic) ==========
            // Create JSON string with all data
            String jsonData = "{";
            jsonData += "\"voltage\":" + voltageStr + ",";
            jsonData += "\"current\":" + currentStr + ",";
            jsonData += "\"power\":" + powerStr + ",";
            jsonData += "\"powerFactor\":" + powerFactorStr + ",";
            jsonData += "\"temperature\":" + temperatureStr + ",";
            jsonData += "\"energy\":" + energyStr;
            jsonData += "\"status\":\"Alive\"";
            jsonData += "}";
            
            // Publish JSON to single topic
            mqtt_client.publish(("data_" + Device_Bot_ID).c_str(), jsonData.c_str());
            
            // ========== SERIAL PRINT OUTPUT ==========
            Serial.println("\n📤 MQTT Published - Bot ID: " + Device_Bot_ID);
            Serial.println("   Voltage: " + voltageStr + " V");
            Serial.println("   Current: " + currentStr + " A");
            Serial.println("   Power: " + powerStr + " W");
            Serial.println("   Power Factor: " + powerFactorStr);
            Serial.println("   Temperature: " + temperatureStr + " °C");
            Serial.println("    Energy: " + energyStr + " kWh");
        }
        
        // Update simulated values (remove this when using real sensors)
        energy += 0.1;
    }
}

#endif