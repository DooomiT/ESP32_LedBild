#pragma once
#include <Arduino.h>

enum State {
    STATE_UNDEFINED,
    STATE_SERIAL_INIT,
    STATE_CREDENTIAL_MANAGEMENT,
    STATE_WIFI_LISTENING,
    STATE_SYSTEM_ERROR,
};

enum LogState {
    LOGSTATE_LOG,
    LOGSTATE_DEBUG,
    LOGSTATE_ERROR
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

