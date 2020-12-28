#pragma once
#include <Arduino.h>

enum State {
    UNDEFINED,
    SERIAL_INIT,
    CREDENTIAL_MANAGEMENT,
    WIFI_LISTENING,
    SYSTEM_ERROR,
};

enum LogState {
    LOG,
    DEBUG,
    ERROR
};

struct WifiCredentials {
	char* ssid;
    char* password;
} wifi_credentials;

struct RGB {
    byte r;
    byte g;
    byte b;
};

struct RGB_S {
    char* r;
    char* g;
    char* b;
};

