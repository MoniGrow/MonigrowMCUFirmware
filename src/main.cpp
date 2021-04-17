
/**
 * Created by K. Suwatchai (Mobizt)
 * 
 * Email: k_suwatchai@hotmail.com
 * 
 * Github: https://github.com/mobizt
 * 
 * Copyright (c) 2021 mobizt
 *
*/

//This example shows how to create a document in a document collection. This operation required Email/password, custom or OAUth2.0 authentication.

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "WIFISSID"
#define WIFI_PASSWORD "WIFIPASSWORD"

/* 2. Define the Firebase project host name and API Key */
#define FIREBASE_HOST "monigrow-a9692.firebaseio.com"
#define API_KEY "AIzaSyB72HwiUWgB8JEPs08CAi884iftTHt8Cno"

/* 3. Define the project ID */
#define FIREBASE_PROJECT_ID "monigrow-a9692"

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "jasonbm123@gmail.com"
#define USER_PASSWORD "ESP32DevicePassword"

#define SerialNumber 1234
//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long dataMillis = 0;
int count = 0;

void setup()
{

    Serial.begin(115200);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    /* Assign the project host and api key (required) */
    config.host = FIREBASE_HOST;
    config.api_key = API_KEY;

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

#if defined(ESP8266)
    //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
    fbdo.setBSSLBufferSize(1024, 1024);
#endif
}

void loop()
{

    if (millis() - dataMillis > 6000 || dataMillis == 0)
    {
        dataMillis = millis();

        String content;
        FirebaseJson js;
        
        //We will create the nested document in the parent path "a0/b0/c0
        //a0 is the collection id, b0 is the document id in collection a0 and c0 is the collection id in the document b0.
        //and d? is the document id in the document collection id c0 which we will create.
        String documentPath = "ESP32data/" + String(auth.token.uid.c_str()) + "/sensorData";

        js.set("fields/temperature/integerValue", String(count).c_str());
        js.set("fields/humidity/integerValue", String(rand()).c_str());
        js.set("fields/timeStamp/booleanValue", count % 2 == 0);
        js.set("fields/userid/stringValue", String(auth.token.uid.c_str()));
        js.toString(content);

        count++;
        //Serial.println());
       // Serial.println(auth.uid)
        Serial.println("------------------------------------");
        Serial.println("Create a document...");

        if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath.c_str(), content.c_str()))
        {
            Serial.println("PASSED");
            Serial.println("------------------------------------");
            Serial.println(fbdo.payload());
            Serial.println("------------------------------------");
            Serial.println();
        }
        else
        {
            Serial.println("FAILED");
            Serial.println("REASON: " + fbdo.errorReason());
            Serial.println("------------------------------------");
            Serial.println();
        }
    }
}