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
State system_state = UNDEFINED;

void setup() {
    Serial.begin(115200);
    serialLog("Starting Setup\n", LOG);
    Serial.setTimeout(initial_timeout_time);
    // initialisation 
    String incoming = Serial.readString();
    if(incoming == "INIT DATA"){
        serialInitCredentials(&system_state);
    }
    // Setup Connection
    credentials = getCredentials();
    serialLog("Connecting to" + String(credentials.ssid) + "\n", LOG);
    WiFi.begin(credentials.ssid, credentials.password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        serialLog("Waiting for Connection\n", LOG);
    }
    system_state = WIFI_LISTENING;
    serialLog("Connected\n", LOG);
    server.begin();
}

void loop() {
    // put your main code here, to run repeatedly:
    switch(system_state){
        case UNDEFINED: {
            break;
        }
        case WIFI_LISTENING: {
            if(!WiFiStateHandler(&system_state)){
                system_state = SYSTEM_ERROR;
            }
            break;
        }
        case CREDENTIAL_MANAGEMENT: {
            break;
        }
        case SYSTEM_ERROR: {
            serialLog("Reset", ERROR);
            resetFunc();
            break;
        }
    }
}