#include <LittleFS.h>
#include <ArduinoJson.h>
#include "dataHandler.h"

extern Config config;

void saveConfig(Config &config)
{
    // Open file for writing
    File file = LittleFS.open(CONFIG_FILENAME, "w");
    if (!file)
    {
        Serial.println("Failed to create config file");
        return;
    }
    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/assistant to compute the capacity.
    StaticJsonDocument<1024> doc;

    // Set the values in the document
    doc["wifiSsid"] = config.wifi.ssid;
    doc["wifiPassword"] = config.wifi.password;
    doc["telegramChatIdMain"] = config.telegram.chatIdMain;
    doc["telegramChatIdSub1"] = config.telegram.chatIdSub1;
    doc["telegramChatIdSub2"] = config.telegram.chatIdSub2;
    doc["tcsApartmentBell"] = config.tcs.apartmentBell;
    doc["tcsStreetCall"] = config.tcs.streetCall;
    doc["tcsGarageCall"] = config.tcs.garageCall; 
    doc["tcsStreetView"] = config.tcs.streetView; 
    doc["tcsGarageView"] = config.tcs.garageView; 
    doc["tcsStreetOpen"] = config.tcs.streetOpen; 
    doc["tcsGarageOpen"] = config.tcs.garageOpen; 
    doc["partyMode"] = config.partyMode;
    doc["silentModeMain"] = config.silentModeMain; 
    doc["silentModeSub1"] = config.silentModeSub1; 
    doc["silentModeSub2"] = config.silentModeSub2; 
    doc["sniffMode"] = config.sniffMode; 
    doc["devMode"] = config.devMode; 

 
    // Serialize JSON to file
    if (serializeJson(doc, file) == 0)
    {
        Serial.println("Failed to write config to file");
    }

    // Close the file
    file.close();
}





bool loadConfig(Config &config)
{
    bool return_error;
    // Open file for reading
    // TODO: check if file exists
    File file = LittleFS.open(CONFIG_FILENAME, "r");

    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/v6/assistant to compute the capacity.
    StaticJsonDocument<1024> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, file);
    if (error) 
    {   Serial.println("Failed to read file");
        return_error = 1;
    }
    else
    {
        // load wifi config
        if (doc.containsKey("wifiSsid")) strncpy(config.wifi.ssid, doc["wifiSsid"].as<const char *>(), sizeof(config.wifi.ssid));
        if (doc.containsKey("wifiPassword")) strncpy(config.wifi.password, doc["wifiPassword"].as<const char *>(), sizeof(config.wifi.password));
        // load telegram config
        if (doc.containsKey("telegramChatIdMain")) strncpy(config.telegram.chatIdMain, doc["telegramChatIdMain"].as<const char *>(), sizeof(config.telegram.chatIdMain));
        if (doc.containsKey("telegramChatIdSub1")) strncpy(config.telegram.chatIdSub1, doc["telegramChatIdSub1"].as<const char *>(), sizeof(config.telegram.chatIdSub1));
        if (doc.containsKey("telegramChatIdSub2")) strncpy(config.telegram.chatIdSub2, doc["telegramChatIdSub2"].as<const char *>(), sizeof(config.telegram.chatIdSub2));
        // load tcs config
        if (doc.containsKey("tcsApartmentBell")) config.tcs.apartmentBell = doc["tcsApartmentBell"];
        if (doc.containsKey("tcsStreetCall")) config.tcs.streetCall = doc["tcsStreetCall"];
        if (doc.containsKey("tcsGarageCall")) config.tcs.garageCall = doc["tcsGarageCall"];
        if (doc.containsKey("tcsStreetView")) config.tcs.streetView = doc["tcsStreetView"];
        if (doc.containsKey("tcsGarageView")) config.tcs.garageView = doc["tcsGarageView"];
        if (doc.containsKey("tcsStreetOpen")) config.tcs.streetOpen = doc["tcsStreetOpen"];
        if (doc.containsKey("tcsGarageOpen")) config.tcs.garageOpen = doc["tcsGarageOpen"];
        // load bot config
        if (doc.containsKey("partyMode")) config.partyMode = doc["partyMode"];
        if (doc.containsKey("silentModeMain")) config.silentModeMain = doc["silentModeMain"];
        if (doc.containsKey("silentModeSub1")) config.silentModeSub1 = doc["silentModeSub1"];
        if (doc.containsKey("silentModeSub2")) config.silentModeSub2 = doc["silentModeSub2"];
        if (doc.containsKey("sniffMode")) config.sniffMode = doc["sniffMode"];
        if (doc.containsKey("devMode")) config.devMode = doc["devMode"];
        // load successful
        return_error = 0;
        Serial.println("Load successful");
    }


    // Close the file (Curiously, File's destructor doesn't close the file)
    file.close();

    return return_error;
}

void setupConfig(void)
{
    if (LittleFS.begin()) Serial.println("file system mounted");
    else Serial.println("Failed to mount file system");
    
    bool error = loadConfig(config);
    if (error){
        saveConfig(config);  // save defaults if cannot load
        Serial.println("Default config saved");
    }

    //for debugging
    Serial.print("w1): ");   Serial.println(config.wifi.ssid);
    Serial.print("w2): ");   Serial.println(config.wifi.password);
    Serial.print("t1): ");   Serial.println(config.telegram.chatIdMain);
    Serial.print("t2): ");   Serial.println(config.telegram.chatIdSub1);
    Serial.print("t3): ");   Serial.println(config.telegram.chatIdSub2);
    Serial.print("b1): ");   Serial.println(config.tcs.apartmentBell, HEX);
    Serial.print("b2): ");   Serial.println(config.tcs.streetCall, HEX);
    Serial.print("b3): ");   Serial.println(config.tcs.garageCall, HEX);
    Serial.print("b4): ");   Serial.println(config.tcs.streetView, HEX);
    Serial.print("b5): ");   Serial.println(config.tcs.garageView, HEX);
    Serial.print("b6): ");   Serial.println(config.tcs.streetOpen, HEX);
    Serial.print("b7): ");   Serial.println(config.tcs.garageOpen, HEX);
    Serial.print("m1): ");   Serial.println(config.partyMode);
    Serial.print("m2): ");   Serial.println(config.silentModeMain);
    Serial.print("m3): ");   Serial.println(config.silentModeSub1);
    Serial.print("m4): ");   Serial.println(config.silentModeSub2);
    Serial.print("m5): ");   Serial.println(config.sniffMode);
    Serial.print("m6): ");   Serial.println(config.devMode);

}