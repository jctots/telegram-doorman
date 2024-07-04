#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <LittleFS.h>
#include <TCSBus.h>

#include <buildinfo.h>
#include <SimpleTimer.h>

#include "platform.h"
#include "dataHandler.h"
#include "eventHandler.h"

buildInfo BuildInfo;

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

extern String chat_id_new_user;

const TcsEvent TcsEvents[N_TCS_EVENTS] = {
  {config.tcs.apartmentBell , handleTcsApartmentBell  },
  {config.tcs.streetCall    , handleTcsStreetCall     },
  {config.tcs.garageCall    , handleTcsGarageCall     },
  {config.tcs.streetView    , handleTcsStreetView     },
  {config.tcs.garageView    , handleTcsGarageView     },
  {config.tcs.streetOpen    , handleTcsStreetOpen     },
  {config.tcs.garageOpen    , handleTcsGarageOpen     },
};
 
const BotEvent BotEvents[N_BOT_EVENTS] = {
  {"/sdoor"   , handleTcsStreetOpenCmd  },
  {"/gdoor"   , handleTcsGarageOpenCmd  },
  {"/silent"  , handleSilentModeRequest },
  {"/party"   , handlePartyModeRequest  },
  {"/help"    , handleHelpRequest       },
  {"/sniff"   , handleSniffModeRequest  },
  {"/dev"     , handleDevModeRequest    },
  {"/status"  , handleStatusRequest     },
  {"/start"   , handleInvalidRequest    },
  {"/stop"    , handleStopRequest       },
  {"/format"  , handleFormatRequest     },
};


void sendTelegramBroadcast(const String& text)
{
  digitalWrite(LED_BUILTIN, LOW); // turn on busy LED
   
  if (config.silentMode0 == false) bot.sendMessage(config.telegram.chatId0, text, "");
  if ((strcmp(config.telegram.chatId1, "0") != 0)&&(config.silentMode1 == false)) bot.sendMessage(config.telegram.chatId1, text, "");
  if ((strcmp(config.telegram.chatId2, "0") != 0)&&(config.silentMode2 == false)) bot.sendMessage(config.telegram.chatId2, text, "");

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
  sendTelegramBroadcast("[TCS Event] Door Bell Pressed!");
}

void handleTcsStreetCall(void)
{
  sendTelegramBroadcast("[TCS Event] Call from Street!");
  if (config.partyMode == true) handleTcsStreetOpen();
}  

void handleTcsGarageCall(void)
{
  sendTelegramBroadcast("[TCS Event] Call from Garage!");
  if (config.partyMode == true) handleTcsGarageOpen();
}  
    
void handleTcsStreetView(void)
{
  sendTelegramBroadcast("[TCS Event] View Street Camera");
} 

void handleTcsGarageView(void)
{
  sendTelegramBroadcast("[TCS Event] View Garage Camera");
} 

void handleTcsStreetOpen(void)
{
  sendTelegramBroadcast("[TCS Event] Open Street Door");
}

void handleTcsGarageOpen(void)
{
  sendTelegramBroadcast("[TCS Event] Open Garage Door");
}

void handleSniffMode(uint32_t cmd)
{
  char outputString[8];
  utoa(cmd, outputString, 16);
  String msg = "[TCS Event] TCS Code: " + String(outputString);
  sendTelegramUser(config.telegram.chatId0, msg);
 
}

void handleTcsStreetOpenCmd(void)
{
  
  tcsReader.disable();
  tcsWriter.write(config.tcs.streetOpen);
  tcsReader.enable();
  // dirty hack to also publish commands we have written
  tcsReader.inject(config.tcs.streetOpen);
}

void handleTcsGarageOpenCmd(void)
{
  tcsReader.disable();
  tcsWriter.write(config.tcs.garageOpen);
  tcsReader.enable();
  // dirty hack to also publish commands we have written
  tcsReader.inject(config.tcs.garageOpen);
}



void handleSilentModeRequest(void)
{

  if (strcmp(config.telegram.chatId0, chat_id.c_str()) == 0)
  {

    if (config.silentMode0 == true)
    {
      config.silentMode0 = false;
      sendTelegramUser(config.telegram.chatId0, "[Bot Event] Silent Mode Disabled");
    }
    else
    {
      config.silentMode0 = true;
      sendTelegramUser(config.telegram.chatId0, "[Bot Event] Silent Mode Enabled");
    }
  }
  else if (strcmp(config.telegram.chatId1, chat_id.c_str()) == 0)
  {
    if (config.silentMode1 == true)
    {
      config.silentMode1 = false;
      sendTelegramUser(config.telegram.chatId1, "[Bot Event] Silent Mode Disabled");
    }
    else
    {
      config.silentMode1 = true;
      sendTelegramUser(config.telegram.chatId1, "[Bot Event] Silent Mode Enabled");
    }
  }
  else // (strcmp(config.telegram.chatId2, chat_id.c_str()) == 0)
  {
    if (config.silentMode2 == true)
    {
      config.silentMode2 = false;
      sendTelegramUser(config.telegram.chatId2, "[Bot Event] Silent Mode Disabled");
    }
    else
    {
      config.silentMode2 = true;
      sendTelegramUser(config.telegram.chatId2, "[Bot Event] Silent Mode Enabled");
    }
  }
  saveConfig(config); 
}


void handlePartyModeRequest(void)
{
  if (config.partyMode == false)
  {
    config.partyMode = true;
    sendTelegramBroadcast("[Bot Event] Party Mode Enabled");
  }
  else
  {
    config.partyMode = false;
    sendTelegramBroadcast("[Bot Event] Party Mode Disabled");
  }
  saveConfig(config); 
}


void handleHelpRequest(void)
{

}






void handleSniffModeRequest(void)
{
  // only for main user
  if (strcmp(config.telegram.chatId0, chat_id.c_str()) == 0)
  {

    if (config.sniffMode == false)
    {
      config.sniffMode= true;
      saveConfig(config); 
      sendTelegramUser(config.telegram.chatId0,"[Bot Event] Sniff Mode Enabled");
    }
    else
    {
      config.sniffMode = false;
      saveConfig(config);
      sendTelegramUser(config.telegram.chatId0,"[Bot Event] Sniff Mode Disabled");
    }
  }
  // other user, invalid
  else
  {
    handleInvalidRequest();
  }
}  

void handleDevModeRequest(void)
{
  // only for main user
  if (strcmp(config.telegram.chatId0, chat_id.c_str()) == 0)
  {

    if (config.devMode == false)
    {
      config.devMode= true;
      config.silentMode0Prev = config.silentMode0;
      config.silentMode1Prev = config.silentMode1;
      config.silentMode2Prev = config.silentMode2;
      config.sniffModePrev = config.sniffMode;
      config.silentMode0 = false;
      config.silentMode1 = true;
      config.silentMode2 = true;
      config.sniffMode = true;
      saveConfig(config); 
      sendTelegramUser(config.telegram.chatId0,"[Bot Event] Dev Mode Enabled");

    }
    else
    {
      config.devMode = false;
      config.silentMode0 = config.silentMode0Prev;
      config.silentMode1 = config.silentMode1Prev;
      config.silentMode2 = config.silentMode2Prev;
      config.sniffMode = config.sniffModePrev;
      saveConfig(config); 
      sendTelegramUser(config.telegram.chatId0,"[Bot Event] Dev Mode Disabled");
    }
  }
  // other user, invalid
  else
  {
    handleInvalidRequest();
  }

}

void handleStatusRequest(void)
{
  char current_version[32];
  snprintf(current_version, (uint8_t)32, "v%d.%d.%d", BuildInfo.mainVersionDigit, BuildInfo.minorVersionDigit, BuildInfo.patchVersionDigit);
   // only for main user
  if (strcmp(config.telegram.chatId0, chat_id.c_str()) == 0)
  {
    String msg;
    msg += "[Bot Event] Status:\n";
    msg += "Build: " + String(current_version) + "-"+ String(BuildInfo.lastCommitTag)+" ("+ String(BuildInfo.branch) +")\n";
    msg += "Local IP: " + WiFi.localIP().toString() + "\n";
    msg += "ChatId0: " + String(config.telegram.chatId0)  + "\n";
    msg += "ChatId1: " + ((strcmp(config.telegram.chatId1, "0") == 0) ? "None" : String(config.telegram.chatId1)) + "\n";
    msg += "ChatId2: " + ((strcmp(config.telegram.chatId2, "0") == 0) ? "None" : String(config.telegram.chatId2))  + "\n";
    msg += "PartyMode: ";
    msg += config.partyMode ? "On\n" : "Off\n";
    msg += "SilentMode0: ";
    msg += config.silentMode0 ? "On\n" : "Off\n";
    msg += "SilentMode1: ";
    msg += config.silentMode1 ? "On\n" : "Off\n";
    msg += "SilentMode2: ";
    msg += config.silentMode2 ? "On\n" : "Off\n";
    msg += "SniffMode: ";
    msg += config.sniffMode ? "On\n" : "Off\n";
    msg += "DevMode: ";
    msg += config.devMode ? "On\n" : "Off\n";
   
  
    sendTelegramUser(config.telegram.chatId0, msg);
  }
  // other user, invalid
  else
  {
    handleInvalidRequest();
  }
}


void handleInvalidRequest(void)
{
  sendTelegramUser(chat_id,"[Bot Event] Invalid Request");
}

void handleStopRequest(void)
{
  String msg;
 
  // telegram message is done first so broadcast to removed user still works
  msg = "[Bot Event] User "+ from_name + " removed as bot user!";
  sendTelegramBroadcast(msg);

  if (strcmp(config.telegram.chatId1, chat_id.c_str()) == 0) strncpy(config.telegram.chatId1, "0", sizeof(config.telegram.chatId1));
  else if (strcmp(config.telegram.chatId2, chat_id.c_str()) == 0) strncpy(config.telegram.chatId2, "0", sizeof(config.telegram.chatId2));
  else;
  
  saveConfig(config); 
}

void handleUnknownRequest(void)
{
  sendTelegramUser(chat_id, "[Bot Event] Unknown Request");
}

void handleStartRequest(void)
{
  chat_id_new_user = chat_id;
  String msg = "[Bot Event] Enter password:";
  sendTelegramUser(chat_id, msg);
}


void handlePasswordCheck(void)
{
  if (strcmp(config.telegram.password, text.c_str()) == 0)
  {
    if (strcmp(config.telegram.chatId1, "0") == 0)
    {
      strncpy(config.telegram.chatId1, chat_id.c_str(), sizeof(config.telegram.chatId1));
      saveConfig(config);
      String msg = "[Bot Event] User "+ from_name + " added as bot user!";
      sendTelegramBroadcast(msg);
    }
    else if (strcmp(config.telegram.chatId2, "0") == 0)
    {
      strncpy(config.telegram.chatId2, chat_id.c_str(), sizeof(config.telegram.chatId2));
      saveConfig(config);
      String msg = "[Bot Event] User "+ from_name + " added as bot user!";
      sendTelegramBroadcast(msg);
    }
    else
    {
      String msg = "[Bot Event] Hello "+ from_name + ", no more space left for new user, remove one first!";
      sendTelegramUser(chat_id, msg);
    }
  }
  else
  {
      String msg = "[Bot Event] Wrong Password!";
      sendTelegramUser(chat_id, msg);
  }
  chat_id_new_user = "0";
}

void handleFormatRequest(void){
  // only for main user
  if (strcmp(config.telegram.chatId0, chat_id.c_str()) == 0)
  {

    LittleFS.end();
    LittleFS.format();
  
    sendTelegramUser(config.telegram.chatId0, "[Bot Event] File System Formatted. Resetting...");
    (int)bot.getUpdates(bot.last_message_received + 1);//to flush reset command
    ESP.reset();
  }
  // other user, invalid
  else
  {
    handleInvalidRequest();
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
