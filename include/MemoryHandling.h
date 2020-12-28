#pragma once
#include <Arduino.h>
#include <EEPROM.h>
#include <Logging.h>
#include <DataTypes.h>
#include <Helpers.h>

WifiCredentials getCredentials(){
    uint addr = 0;
    struct { 
        char ssid[32] = "";
        char password[32] = "";
    } data;
    serialLog("get Wifi Credentials from EEPROM", LOG);
    EEPROM.begin(512);
    EEPROM.get(addr,data);
    WifiCredentials tmp;
    tmp.ssid = data.ssid;
    tmp.password = data.password;
    serialLog("received ssid: " + String(tmp.ssid) + ", password: " + String(tmp.password) + "\n", DEBUG);
    return tmp;
}

bool setCredentials(WifiCredentials new_credentials){
    serialLog("set Wifi Credentials on EEPROM", LOG);
    uint addr = 0;
    EEPROM.put(addr, new_credentials);
    EEPROM.commit();
    serialLog("set ssid: " + String(new_credentials.ssid) + ", password: " + String(new_credentials.password) + "\n", DEBUG);
    return true;
}

bool serialInitCredentials(State *sys_state){
    *sys_state = SERIAL_INIT;
    String incoming = Serial.readString();
    WifiCredentials tmp;
    char delimiter = ';';
    int delimiterIndex = incoming.indexOf(delimiter);
    incoming.substring(0, delimiterIndex).toCharArray(tmp.ssid, 32);
    incoming.substring(delimiterIndex + 1).toCharArray(tmp.password, 32);
    if(setCredentials(tmp)){
        serialLog("Succsessfully initialised credentials\n", LOG);
        return true;
    }
    return false;
}

bool setRGBMem(RGB rgb){
    RGB_S tmp = rgbToString(rgb);
    serialLog("save RGB values on EEPROM", LOG);
    uint addr = 1;
    EEPROM.put(addr, tmp);
    EEPROM.commit();
    serialLog("Saved RGB:" + String(tmp.r) + "," + String(tmp.g) + "," + String(tmp.b) + "\n", LOG);
    return true;
}

RGB getRGBMem(){
    uint addr = 0;
    RGB_S tmp;
    serialLog("get Wifi Credentials from EEPROM", LOG);
    EEPROM.begin(512);
    EEPROM.get(addr,tmp);
    serialLog("Load RGB:" + String(tmp.r) + "," + String(tmp.g) + "," + String(tmp.b) + "\n", LOG);
    return rgbToByte(tmp);
}