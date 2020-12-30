#include <Arduino.h>
#include "ESP8266WiFi.h"
#include <DataTypes.h>
#include <Logging.h>
#include <MemoryHandling.h>
#include <WirelessHandling.h>
#include <Helpers.h>

/*
  IDEA:
  Basic credentials definable
  Credentials changable

  Connects on startup to wifi 
  credentials are saved in memory!

  Resets if processing dosn't work

  goes to powersaving mode when no calls are received for 1 min

  ...
*/

WifiCredentials credentials; 
const long initial_timeout_time = 2000;
State system_state = STATE_UNDEFINED;

void setup() {
    Serial.begin(115200);
    serialLog("Starting Setup\n", LOGSTATE_LOG);
    Serial.setTimeout(initial_timeout_time);
    // initialisation 
    String incoming = Serial.readString();
    if(incoming == "INIT DATA"){
        serialInitCredentials(&system_state);
    }
    // Setup Connection
    credentials = getCredentials();
    serialLog("Connecting to" + String(credentials.ssid) + "\n", LOGSTATE_LOG);
    WiFi.begin(credentials.ssid, credentials.password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        serialLog("Waiting for Connection\n", LOGSTATE_LOG);
    }
    system_state = STATE_WIFI_LISTENING;
    serialLog("Connected\n", LOGSTATE_LOG);
    server.begin();
}

void loop() {
    // put your main code here, to run repeatedly:
    switch(system_state){
        case STATE_UNDEFINED: {
            break;
        }
        case STATE_WIFI_LISTENING: {
            if(!WiFiStateHandler(&system_state)){
                system_state = STATE_SYSTEM_ERROR;
            }
            break;
        }
        case STATE_CREDENTIAL_MANAGEMENT: {
            credentials = getCredentials();
            WiFi.begin(credentials.ssid, credentials.password);
            while (WiFi.status() != WL_CONNECTED) {
                delay(500);
                serialLog("Waiting for Connection\n", LOGSTATE_LOG);
            }
            system_state = STATE_WIFI_LISTENING;
            serialLog("Connected\n", LOGSTATE_LOG);
            server.begin();
            break;
        }
        case STATE_SYSTEM_ERROR: {
            serialLog("Reset", LOGSTATE_ERROR);
            resetFunc();
            break;
        }
    }
}