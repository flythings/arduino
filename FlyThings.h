//
// Created by Gonzalo Blazquez Gil on 8/6/16.
//

#ifndef FLYTHINGS_H
#define FLYTHINGS_H

#include "Arduino.h"

class FlyThings {

public:

    Flythings(const char* token, const char* workspace) {
        token_ = token;
        workspace_ = workspace;
    }

    ~Flythings(){

    }

protected:

    virtual bool network_connected(){
        return WiFi.status() == WL_CONNECTED && !(WiFi.localIP() == INADDR_NONE);
    }

    virtual bool connect_network(){
        long wifi_timeout = millis();

#ifdef _DEBUG_
        Serial.print(F("[NETWORK] Connecting to network "));
            Serial.println(wifi_ssid_);
#endif
        WiFi.begin(wifi_ssid_, wifi_password_);
        while( WiFi.status() != WL_CONNECTED) {
            if(millis() - wifi_timeout > 30000) return false;

#ifdef ESP8266
            yield();
#endif
        }


#ifdef _DEBUG_
        Serial.println(F("[NETWORK] Connected to WiFi!"));
#endif

        wifi_timeout = millis();

#ifdef _DEBUG_
        Serial.println(F("[NETWORK] Getting IP Address..."));
#endif

        while (WiFi.localIP() == INADDR_NONE) {
            if(millis() - wifi_timeout > 30000) return false;

#ifdef ESP8266
            yield();
#endif
        }
#ifdef _DEBUG_
        Serial.print(F("[NETWORK] Got IP Address: "));
            Serial.println(WiFi.localIP());
#endif
        return true;
    }

public:

    void add_wifi(const char* ssid, const char* password){
        wifi_ssid_ = ssid;
        wifi_password_ = password;
    }

    void sendData(float value, String foi, String procedure, String observableProperty, String uom) {
        const int httpPort = 80;
        if (client_.connect("flythings.itg.es", httpPort)) {

            String postData = "{\"foi\":\"";
            postData += foi;
            postData += "\",\"procedure\":\"";
            postData += procedure;
            postData += "\",\"observableProperty\":\"";
            postData += observableProperty;
            postData += "\",\"value\":";
            postData += value;
            postData += ",\"uom\":\"";
            postData += uom;
            postData += "\"}";

#ifdef _DEBUG_
            Serial.println(postData);
#endif
            client_.println("PUT /api/observation/single HTTP/1.1");
            client_.println("Host: flythings.itg.es");
            client_.println("Cache-Control: no-cache");
            client_.println("Content-Type: application/json");
            client_.print("x-auth-token: ");
            client_.println(token_);
            client_.print("workspace: ");
            client_.println(workspace_);
            client_.print("Content-Length: ");
            client_.println(postData.length());
            client_.println();
            client_.println(postData);
        }

    }


private:

    WiFiClient client_;
    const char* wifi_ssid_;
    const char* wifi_password_;
    const char* token_;
    const char* workspace_;
};

#endif



#endif //FLYTHINGS_H
