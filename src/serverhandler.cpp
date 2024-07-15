#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>

#include "serverhandler.h"
#include "dataHandler.h"
#include "html.h"



void handle404(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void handleIndexPage(AsyncWebServerRequest *request)
{
    request->send_P(200, "text/html", index_html, loadConfigValues);
}

// Replaces placeholder with stored values
String loadConfigValues(const String& var){
  //Serial.println(var);
  if        (var == "wifiSsid")         return config.wifi.ssid;
  else if   (var == "wifiPassword")     return config.wifi.password;
  else if   (var == "wifiOtaPass")      return config.wifi.otaPass;
  else if   (var == "telegramChatId0")  return config.telegram.chatId0;
  else if   (var == "telegramChatId1")  return config.telegram.chatId1;
  else if   (var == "telegramChatId2")  return config.telegram.chatId2;
  else      return String();
}


// doc["telegramPassword"] = config.telegram.password;
// doc["tcsApartmentBell"] = config.tcs.apartmentBell;
// doc["tcsStreetCall"] = config.tcs.streetCall;
// doc["tcsGarageCall"] = config.tcs.garageCall; 
// doc["tcsStreetView"] = config.tcs.streetView; 
// doc["tcsGarageView"] = config.tcs.garageView; 
// doc["tcsStreetOpen"] = config.tcs.streetOpen; 
// doc["tcsGarageOpen"] = config.tcs.garageOpen; 
// doc["partyMode"] = config.partyMode;
// doc["silentMode0"] = config.silentMode0; 
// doc["silentMode1"] = config.silentMode1; 
// doc["silentMode2"] = config.silentMode2; 
// doc["sniffMode"] = config.sniffMode; 
// doc["devMode"] = config.devMode; 
// doc["silentMode0Prev"] = config.silentMode0Prev; 
// doc["silentMode1Prev"] = config.silentMode1Prev; 
// doc["silentMode2Prev"] = config.silentMode2Prev; 
// doc["sniffModePrev"] = config.sniffModePrev; 


void handleGet (AsyncWebServerRequest *request) {
  String message;

    if (request->hasParam("wifiSsid"))      strncpy(config.wifi.ssid      , request->getParam("wifiSsid")->value().c_str(), sizeof(config.wifi.ssid));
    if (request->hasParam("wifiPassword"))  strncpy(config.wifi.password  , request->getParam("wifiPassword")->value().c_str(), sizeof(config.wifi.password));
    

    saveConfig(config); 
    // Serial.println(inputMessage);
    request->send(200, "text/html");
  }