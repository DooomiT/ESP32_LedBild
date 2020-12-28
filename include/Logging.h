#pragma once
#include <Arduino.h>
#include <DataTypes.h>

// logger settings
bool logging = true;
bool debugging = true;
bool error = true;

void serialLog(String log_string, LogState log_state){
    switch(log_state){
        case LOG: if(logging){Serial.print("LOG: " + log_string);} break;
        case DEBUG: if(debugging){Serial.print("DEBUG: " + log_string);} break;
        case ERROR: if(error){Serial.print("ERROR: " + log_string);} break;
    }
}