#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Logging.h>
#include <DataTypes.h>
#include <Helpers.h>

long timeout_time = 2000;

WiFiServer server(80); 

RGB rgb;

void sendHTML(WiFiClient *client){
    // Display the HTML web page
    client->println("<!DOCTYPE html><html>");
    client->println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client->println("<link rel=\"icon\" href=\"data:,\">");
    // Web Page Heading
    client->println("<body><h1>ESP8266 Web Server</h1>");
    // Display current state, and ON/OFF buttons for GPIO 5  
    client->println("<p>RGB - State R:" + String(rgb.r) + " G:" + String(rgb.g) + " B:" + String(rgb.b) + "</p>");
    // TODO implement switchable states
    client->println("</body></html>");
    // The HTTP response ends with another blank line
    client->println();
}

bool WiFiStateHandler(State *sys_state){
    rgb = getRGBMem();
    WiFiClient client = server.available();   
    if (client) {                             
        serialLog("New Client Connected\n", LOGSTATE_LOG);         
        String incomming_data = "";                
        long current_time = millis();
        long previous_time = current_time;
        String header;
        // loop while the client's connected
        while (client.connected() && current_time - previous_time <= timeout_time) {
            current_time = millis();         
            if (client.available()) {             
            char c = client.read();             
            serialLog("Received Byte: " + String(c), LOGSTATE_LOG);
            header += c;
            if (c == '\n') {     
                if (incomming_data.length() == 0) {             
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-type:text/html");
                    client.println("Connection: close");
                    client.println();    
                    if (header.indexOf("GET /RGB") >= 0) {
                        serialLog("GET RGB:" + String(rgb.r) + "," + String(rgb.g) + "," + String(rgb.b) + "\n", LOGSTATE_LOG);
                    } 
                    else if (header.indexOf("POST /RGB") >= 0) {
                        // use POST /RGB?255?255?255
                        char delimiter = '?';
                        RGB_S tmp;
                        int delimiter_index = header.indexOf(delimiter);
                        int second_delimiter_index = header.substring(delimiter_index + 1).indexOf(delimiter) + delimiter_index;
                        header.substring(0, delimiter_index).toCharArray(tmp.r, 3);
                        header.substring(delimiter_index + 1, second_delimiter_index).toCharArray(tmp.g, 3);
                        header.substring(second_delimiter_index + 1).toCharArray(tmp.b, 3);
                        rgb = rgbToByte(tmp);
                        setRGBMem(rgb);
                        serialLog("SET RGB:" + String(rgb.r) + "," + String(rgb.g) + "," + String(rgb.b) + "\n", LOGSTATE_LOG);
                    }
                    else if (header.indexOf("POST /CREDENTIALS" >= 0)) {
                        char delimiter = '?';
                        WifiCredentials tmp;
                        int delimiter_index = header.indexOf(delimiter);
                        int second_delimiter_index = header.substring(delimiter_index + 1).indexOf(delimiter) + delimiter_index;
                        header.substring(delimiter_index + 1, second_delimiter_index).toCharArray(tmp.ssid, 32);
                        header.substring(second_delimiter_index + 1).toCharArray(tmp.password, 32);
                        setCredentials(tmp);
                        serialLog("SET CREDENTIALS:" + String(tmp.ssid) + "," + String(tmp.password) + "\n", LOGSTATE_LOG);
                        *sys_state = STATE_CREDENTIAL_MANAGEMENT;
                    }
                    sendHTML(&client);
                    break;
                } 
                else { // if you got a newline, then clear incomming_data
                    incomming_data = "";
                }
                } 
                else if (c != '\r') {  // if you got anything else but a carriage return character,
                    incomming_data += c;      // add it to the end of the incomming_data
                }
            }
        }
        // Close the connection
        client.stop();
        serialLog("Client disconnected\n", LOGSTATE_LOG);
    }
    return true;
}



