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
    doc["wifiOtaPass"] = config.wifi.otaPass;
    doc["telegramChatId0"] = config.telegram.chatId0;
    doc["telegramChatId1"] = config.telegram.chatId1;
    doc["telegramChatId2"] = config.telegram.chatId2;
    doc["telegramPassword"] = config.telegram.password;
    doc["tcsApartmentBell"] = config.tcs.apartmentBell;
    doc["tcsStreetCall"] = config.tcs.streetCall;
    doc["tcsGarageCall"] = config.tcs.garageCall; 
    doc["tcsStreetView"] = config.tcs.streetView; 
    doc["tcsGarageView"] = config.tcs.garageView; 
    doc["tcsStreetOpen"] = config.tcs.streetOpen; 
    doc["tcsGarageOpen"] = config.tcs.garageOpen; 
    doc["partyMode"] = config.partyMode;
    doc["silentMode0"] = config.silentMode0; 
    doc["silentMode1"] = config.silentMode1; 
    doc["silentMode2"] = config.silentMode2; 
    doc["sniffMode"] = config.sniffMode; 
    doc["devMode"] = config.devMode; 
    doc["silentMode0Prev"] = config.silentMode0Prev; 
    doc["silentMode1Prev"] = config.silentMode1Prev; 
    doc["silentMode2Prev"] = config.silentMode2Prev; 
    doc["sniffModePrev"] = config.sniffModePrev; 

 
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
        if (doc.containsKey("wifiOtaPass")) strncpy(config.wifi.otaPass, doc["wifiOtaPass"].as<const char *>(), sizeof(config.wifi.otaPass));
        // load telegram config
        if (doc.containsKey("telegramChatId0")) strncpy(config.telegram.chatId0, doc["telegramChatId0"].as<const char *>(), sizeof(config.telegram.chatId0));
        if (doc.containsKey("telegramChatId1")) strncpy(config.telegram.chatId1, doc["telegramChatId1"].as<const char *>(), sizeof(config.telegram.chatId1));
        if (doc.containsKey("telegramChatId2")) strncpy(config.telegram.chatId2, doc["telegramChatId2"].as<const char *>(), sizeof(config.telegram.chatId2));
        if (doc.containsKey("telegramPassword")) strncpy(config.telegram.password, doc["telegramPassword"].as<const char *>(), sizeof(config.telegram.password));
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
        if (doc.containsKey("silentMode0")) config.silentMode0 = doc["silentMode0"];
        if (doc.containsKey("silentMode1")) config.silentMode1 = doc["silentMode1"];
        if (doc.containsKey("silentMode2")) config.silentMode2 = doc["silentMode2"];
        if (doc.containsKey("sniffMode")) config.sniffMode = doc["sniffMode"];
        if (doc.containsKey("devMode")) config.devMode = doc["devMode"];
        if (doc.containsKey("silentMode0Prev")) config.silentMode0Prev = doc["silentMode0Prev"];
        if (doc.containsKey("silentMode1Prev")) config.silentMode1Prev = doc["silentMode1Prev"];
        if (doc.containsKey("silentMode2Prev")) config.silentMode2Prev = doc["silentMode2Prev"];
        if (doc.containsKey("sniffModePrev")) config.sniffModePrev = doc["sniffModePrev"];
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
        LittleFS.end();
        LittleFS.format();
        LittleFS.begin();
        saveConfig(config);  // save defaults if cannot load
        Serial.println("Default config saved");
    }

    //for debugging
    Serial.print("w1): ");   Serial.println(config.wifi.ssid);
    Serial.print("w2): ");   Serial.println(config.wifi.password);
    Serial.print("w3): ");   Serial.println(config.wifi.otaPass);
    Serial.print("t1): ");   Serial.println(config.telegram.chatId0);
    Serial.print("t2): ");   Serial.println(config.telegram.chatId1);
    Serial.print("t3): ");   Serial.println(config.telegram.chatId2);
    Serial.print("t4): ");   Serial.println(config.telegram.password);
    Serial.print("b1): ");   Serial.println(config.tcs.apartmentBell, HEX);
    Serial.print("b2): ");   Serial.println(config.tcs.streetCall, HEX);
    Serial.print("b3): ");   Serial.println(config.tcs.garageCall, HEX);
    Serial.print("b4): ");   Serial.println(config.tcs.streetView, HEX);
    Serial.print("b5): ");   Serial.println(config.tcs.garageView, HEX);
    Serial.print("b6): ");   Serial.println(config.tcs.streetOpen, HEX);
    Serial.print("b7): ");   Serial.println(config.tcs.garageOpen, HEX);
    Serial.print("m1): ");   Serial.println(config.partyMode);
    Serial.print("m2): ");   Serial.println(config.silentMode0);
    Serial.print("m3): ");   Serial.println(config.silentMode1);
    Serial.print("m4): ");   Serial.println(config.silentMode2);
    Serial.print("m5): ");   Serial.println(config.sniffMode);
    Serial.print("m6): ");   Serial.println(config.devMode);
    Serial.print("m7): ");   Serial.println(config.silentMode0Prev);
    Serial.print("m8): ");   Serial.println(config.silentMode1Prev);
    Serial.print("m9): ");   Serial.println(config.silentMode2Prev);
    Serial.print("mA): ");   Serial.println(config.sniffModePrev);

}