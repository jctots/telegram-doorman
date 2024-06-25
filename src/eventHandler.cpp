#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <TCSBus.h>

#include <SimpleTimer.h>

#include "platform.h"
#include "dataHandler.h"
#include "eventHandler.h"

extern WiFiClientSecure secured_client;
extern UniversalTelegramBot bot;

extern TCSBusReader tcsReader;
extern TCSBusWriter tcsWriter;
extern Config config;

extern SimpleTimer timerLedPeriod;
extern SimpleTimer timerLedPulse;

extern String chat_id;
extern String text;
extern String from_name;


void sendTelegramBroadcast(const String& text)
{
  digitalWrite(LED_BUILTIN, LOW); // turn on busy LED
   
  if (config.silentModeMain == false) bot.sendMessage(config.telegram.chatIdMain, text, "");
  if ((strcmp(config.telegram.chatIdSub1, "0") != 0)&&(config.silentModeSub1 == false)) bot.sendMessage(config.telegram.chatIdSub1, text, "");
  if ((strcmp(config.telegram.chatIdSub2, "0") != 0)&&(config.silentModeSub2 == false)) bot.sendMessage(config.telegram.chatIdSub2, text, "");

  digitalWrite(LED_BUILTIN, HIGH); // turn off busy LED
 
}

void sendTelegramUser(const String& chat_id, const String& text)
{
  digitalWrite(LED_BUILTIN, LOW); // turn on busy LED
   
  bot.sendMessage(chat_id, text, "");
 
  digitalWrite(LED_BUILTIN, HIGH); // turn off busy LED
}



void handleTcsApartmentBell(void)
{
  sendTelegramBroadcast("TCS Event: Door Bell Pressed!");
}

void handleTcsStreetCall(void)
{
  sendTelegramBroadcast("TCS Event: Call from Street!");
  if (config.partyMode == true) handleTcsStreetOpen();
}  

void handleTcsGarageCall(void)
{
  sendTelegramBroadcast("TCS Event: Call from Garage!");
  if (config.partyMode == true) handleTcsGarageOpen();
}  
    
void handleTcsStreetView(void)
{
  sendTelegramBroadcast("TCS Event: View Street Camera");
} 

void handleTcsGarageView(void)
{
  sendTelegramBroadcast("TCS Event: View Garage Camera");
} 


void handleSniffMode(uint32_t cmd)
{
  char outputString[8];
  utoa(cmd, outputString, 16);
  String msg = "TCS Event: " + String(outputString);
  sendTelegramUser(config.telegram.chatIdMain, msg);
 
}




void handleTcsStreetOpen(void)
{
  sendTelegramBroadcast("TCS Event: Open Street Door");
  tcsReader.disable();
  tcsWriter.write(config.tcs.streetOpen);
  tcsReader.enable();
  // dirty hack to also publish commands we have written
  tcsReader.inject(config.tcs.streetOpen);
}

void handleTcsGarageOpen(void)
{
  sendTelegramBroadcast("TCS Event: Open Garage Door");
  tcsReader.disable();
  tcsWriter.write(config.tcs.garageOpen);
  tcsReader.enable();
  // dirty hack to also publish commands we have written
  tcsReader.inject(config.tcs.garageOpen);
}


void handleSilentModeRequest(void)
{

  if (strcmp(config.telegram.chatIdMain, chat_id.c_str()) == 0)
  {

    if (config.silentModeMain == true)
    {
      config.silentModeMain = false;
      sendTelegramUser(config.telegram.chatIdMain, "Bot Event: Silent Mode Disabled");
    }
    else
    {
      config.silentModeMain = true;
      sendTelegramUser(config.telegram.chatIdMain, "Bot Event: Silent Mode Enabled");
    }
  }
  else if (strcmp(config.telegram.chatIdSub1, chat_id.c_str()) == 0)
  {
    if (config.silentModeSub1 == true)
    {
      config.silentModeSub1 = false;
      sendTelegramUser(config.telegram.chatIdSub1, "Bot Event: Silent Mode Disabled");
    }
    else
    {
      config.silentModeSub1 = true;
      sendTelegramUser(config.telegram.chatIdSub1, "Bot Event: Silent Mode Enabled");
    }
  }
  else // (strcmp(config.telegram.chatIdSub2, chat_id.c_str()) == 0)
  {
    if (config.silentModeSub2 == true)
    {
      config.silentModeSub2 = false;
      sendTelegramUser(config.telegram.chatIdSub2, "Bot Event: Silent Mode Disabled");
    }
    else
    {
      config.silentModeSub2 = true;
      sendTelegramUser(config.telegram.chatIdSub2, "Bot Event: Silent Mode Enabled");
    }
  }
  saveConfig(config); 
}


void handlePartyModeRequest(void)
{
  if (config.partyMode == false)
  {
    config.partyMode = true;
    sendTelegramBroadcast("Bot Event: Party Mode Enabled");
  }
  else
  {
    config.partyMode = false;
    sendTelegramBroadcast("Bot Event: Party Mode Disabled");
  }
  saveConfig(config); 
}


void handleHelpRequest(void)
{

}






void handleSniffModeRequest(void)
{
  // only for main user
  if (strcmp(config.telegram.chatIdMain, chat_id.c_str()) == 0)
  {

    if (config.sniffMode == false)
    {
      config.sniffMode= true;
      saveConfig(config); 
      sendTelegramUser(config.telegram.chatIdMain,"Bot Event: Sniff Mode Enabled");
    }
    else
    {
      config.sniffMode = false;
      saveConfig(config);
      sendTelegramUser(config.telegram.chatIdMain,"Bot Event: Sniff Mode Disabled");
    }
  }
  // other user, invalid
  else
  {
    sendTelegramUser(chat_id,"Bot Event: Invalid Request");
  }
}  

void handleDevModeRequest(void)
{
  // only for main user
  if (strcmp(config.telegram.chatIdMain, chat_id.c_str()) == 0)
  {

    if (config.devMode == false)
    {
      config.devMode= true;
      config.silentModeMainPrev = config.silentModeMain;
      config.silentModeSub1Prev = config.silentModeSub1;
      config.silentModeSub2Prev = config.silentModeSub2;
      config.sniffModePrev = config.sniffMode;
      config.silentModeMain = false;
      config.silentModeSub1 = true;
      config.silentModeSub2 = true;
      config.sniffMode = true;
      saveConfig(config); 
      sendTelegramUser(config.telegram.chatIdMain,"Bot Event: Dev Mode Enabled");

    }
    else
    {
      config.devMode = false;
      config.silentModeMain = config.silentModeMainPrev;
      config.silentModeSub1 = config.silentModeSub1Prev;
      config.silentModeSub2 = config.silentModeSub2Prev;
      config.sniffMode = config.sniffModePrev;
      saveConfig(config); 
      sendTelegramUser(config.telegram.chatIdMain,"Bot Event: Dev Mode Disabled");
    }
  }
  // other user, invalid
  else
  {
    sendTelegramUser(chat_id,"Bot Event: Invalid Request");
  }

}

void handleConfigRequest(void)
{
   // only for main user
  if (strcmp(config.telegram.chatIdMain, chat_id.c_str()) == 0)
  {
    String msg;
    msg += "Bot Event:\n";
    msg += "Local IP: " + WiFi.localIP().toString() + "\n";
    msg += "ChatIdMain: " + String(config.telegram.chatIdMain)  + "\n";
    msg += "ChatIdSub1: " + String(config.telegram.chatIdSub1)  + "\n";
    msg += "ChatIdSub2: " + String(config.telegram.chatIdSub2)  + "\n";
    msg += "PartyMode: " + String(config.partyMode) + "\n";
    msg += "SilentMain: " + String(config.silentModeMain) + "\n";
    msg += "SilentSub1: " + String(config.silentModeSub1) + "\n";
    msg += "SilentSub2: " + String(config.silentModeSub2) + "\n";
    msg += "SniffMode: " + String(config.sniffMode) + "\n";
    msg += "DevMode: " + String(config.devMode) + "\n";
   
  
    sendTelegramUser(chat_id, msg);
  }
  // other user, invalid
  else
  {
    sendTelegramUser(chat_id,"Bot Event: Invalid Request");
  }
}


void handleInvalidRequest(void)
{
  sendTelegramUser(chat_id,"Bot Event: Invalid Request");
}

void handleStopRequest(void)
{
  String msg;

  if (strcmp(config.telegram.chatIdSub1, chat_id.c_str()) == 0)
  {
    strncpy(config.telegram.chatIdSub1, "0", sizeof(config.telegram.chatIdSub1));
    msg = "Bot Event: Hello "+ from_name + ", you are now removed as bot user!";
  }
  else if (strcmp(config.telegram.chatIdSub2, chat_id.c_str()) == 0)
  {
    strncpy(config.telegram.chatIdSub2, "0", sizeof(config.telegram.chatIdSub2));
    msg = "Bot Event: Hello "+ from_name + ", you are now removed as bot user!";
  }

  sendTelegramUser(chat_id, msg);
  saveConfig(config); 
}

void handleUnknownRequest(void)
{
  sendTelegramUser(chat_id, "Bot Event: Unknown Request");
}

void handleStartRequest(void)
{
  if (strcmp(config.telegram.chatIdSub1, "0") == 0)
  {
    strncpy(config.telegram.chatIdSub1, chat_id.c_str(), sizeof(config.telegram.chatIdSub1));
    saveConfig(config);
    String msg = "Bot Event: Hello "+ from_name + ", you are now added as bot user!";
    sendTelegramUser(chat_id, msg);
  }
  else if (strcmp(config.telegram.chatIdSub1, "0") == 0)
  {
    strncpy(config.telegram.chatIdSub2, chat_id.c_str(), sizeof(config.telegram.chatIdSub2));
    saveConfig(config);
    String msg = "Bot Event: Hello "+ from_name + ", you are now added as bot user!";
    sendTelegramUser(chat_id, msg);
  }
  else
  {
    String msg = "Bot Event: Hello "+ from_name + ", no more space left for new user, remove one first!";
    sendTelegramUser(chat_id, msg);
  }
}

        
void LedPeriodTimeout()
{
  digitalWrite(LED_BUILTIN, LOW); // turn on LED
  timerLedPeriod.start();
  timerLedPulse.start();
}

void LedPulseTimeout()
{
  digitalWrite(LED_BUILTIN, HIGH); // turn off LED
}
