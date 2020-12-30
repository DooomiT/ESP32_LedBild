#pragma once
#include <Arduino.h>
#include <DataTypes.h>

// logger settings
bool logging = true;
bool debugging = true;
bool error = true;

void serialLog(String log_string, LogState log_state){
    switch(log_state){
        case LOGSTATE_LOG: if(logging){Serial.print("LOGSTATE_LOG: " + log_string);} break;
        case LOGSTATE_DEBUG: if(debugging){Serial.print("LOGSTATE_DEBUG: " + log_string);} break;
        case LOGSTATE_ERROR: if(error){Serial.print("LOGSTATE_ERROR: " + log_string);} break;
    }
}