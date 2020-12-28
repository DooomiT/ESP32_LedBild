#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Logging.h>
#include <DataTypes.h>

RGB rgbToByte(RGB_S str){
    RGB tmp;
    tmp.r = byte(atoi(str.r));
    tmp.g = byte(atoi(str.g));
    tmp.b = byte(atoi(str.b));
    return tmp;
}

RGB_S rgbToString(RGB bte){
    RGB_S tmp;
    tmp.r = (char*)bte.r;
    tmp.g = (char*)bte.g;
    tmp.b = (char*)bte.b;
    return tmp;
}

void(* resetFunc) (void) = 0;