#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

#include <TCSBus.h>
#include <SimpleTimer.h>

#include "platform.h"
#include "secrets.h"
#include "dataHandler.h"
#include "eventHandler.h"

const unsigned long BOT_MTBS = 1000; // mean time between scan messages
unsigned long bot_lasttime;          // last time messages' scan has been done


Config config = {
  {DEFAULT_WIFI_SSID, DEFAULT_WIFI_PASSWORD},
  {DEFAULT_TELEGRAM_CHAT_ID, "0", "0"},
  {DEFAULT_TCS_APT_BELL, DEFAULT_TCS_STREET_CALL, DEFAULT_TCS_GARAGE_CALL, DEFAULT_TCS_STREET_VIEW, DEFAULT_TCS_GARAGE_VIEW, DEFAULT_TCS_STREET_OPEN, DEFAULT_TCS_GARAGE_OPEN},
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};


X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

TCSBusReader tcsReader(PIN_BUS_READ);
TCSBusWriter tcsWriter(PIN_BUS_WRITE);

SimpleTimer timerLedPeriod(3000, &LedPeriodTimeout); // every 3 seconds
SimpleTimer timerLedPulse(10, &LedPulseTimeout);    // turn ON LED for 10ms

String chat_id;
String text;
String from_name;

void setup(void)
{

  //initialize LED for busy status
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // turn on LED busy status

  //initialize serial for debugging purposes
  Serial.begin(115200);
  Serial.println();
  Serial.println("\n\n\n-- setup start --");

  //setup and load config
  setupConfig();

  // attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(config.wifi.ssid);
  WiFi.begin(config.wifi.ssid, config.wifi.password);
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);

  sendTelegramBroadcast("Bot Event: Bot started up. Local IP Address: " + WiFi.localIP().toString());

  //initialize TCS 
  tcsReader.begin();
  tcsWriter.begin();

  //initialize heartbeat timers
  timerLedPeriod.stop();
  timerLedPulse.stop();


  Serial.println("-- setup end --\n\n\n");

  //start heartbeat
  timerLedPeriod.start();

  //turn off LED busy status
  digitalWrite(LED_BUILTIN, HIGH); 
}










void loop() {

  //handle TCS bus event
  if (tcsReader.hasCommand())
  {
    uint32_t cmd = tcsReader.read();
    
    if (cmd == config.tcs.apartmentBell) handleTcsApartmentBell();
    else if (cmd == config.tcs.streetCall) handleTcsStreetCall();
    else if (cmd == config.tcs.garageCall) handleTcsGarageCall();
    else if (cmd == config.tcs.streetView) handleTcsStreetView();
    else if (cmd == config.tcs.garageView) handleTcsGarageView();
    else;

    if (config.sniffMode == true) handleSniffMode(cmd);
    else;
  }

  //handle telegram updates
  if (millis() - bot_lasttime > BOT_MTBS)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      for (int i = 0; i < numNewMessages; i++)
      {
        chat_id = bot.messages[i].chat_id;
        text = bot.messages[i].text;
        from_name = bot.messages[i].from_name;
    
        // debug
        String debug_msg = "Chat id: " + chat_id + "\tFrom: " + from_name + "\tText: " + text;
        Serial.println(debug_msg);

        // respond only if registered user
        if ((strcmp(config.telegram.chatIdMain, chat_id.c_str()) == 0) ||
            (strcmp(config.telegram.chatIdSub1, chat_id.c_str()) == 0) ||
            (strcmp(config.telegram.chatIdSub1, chat_id.c_str()) == 0))
        {
          if (text == "/sdoor") handleTcsStreetOpen();
          else if (text == "/gdoor") handleTcsGarageOpen();
          else if (text == "/silent") handleSilentModeRequest();
          else if (text == "/party") handlePartyModeRequest();
          else if (text == "/help") handleHelpRequest();
          
          else if (text == "/sniff") handleSniffModeRequest();
          else if (text == "/dev") handleDevModeRequest();
          else if (text == "/config") handleConfigRequest();
          else if (text == "/start") handleInvalidRequest();
          else if (text == "/stop") handleStopRequest();
          else handleUnknownRequest();
        }
        // if unknown user, but want to register
        else if (text == "/start") handleStartRequest();
        // if unknown user, and unknown command
        else; // no response
                  
      }
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    bot_lasttime = millis();
  }

  //handle heartbeat
  timerLedPeriod.check();
  timerLedPulse.check();

}



