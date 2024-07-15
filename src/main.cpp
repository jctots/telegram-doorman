#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <ArduinoOTA.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>

#include <TCSBus.h>
#include <SimpleTimer.h>

#include "platform.h"
#include "secrets.h"
#include "dataHandler.h"
#include "eventHandler.h"
#include "serverhandler.h"
#include "html.h"

const unsigned long BOT_MTBS = 1000; // mean time between scan messages
unsigned long bot_lasttime;          // last time messages' scan has been done



X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

TCSBusReader tcsReader(PIN_BUS_READ);
TCSBusWriter tcsWriter(PIN_BUS_WRITE);

SimpleTimer timerLedPeriod(3000, &LedPeriodTimeout); // every 3 seconds
SimpleTimer timerLedPulse(10, &LedPulseTimeout);    // turn ON LED for 10ms

String chat_id = "0";
String text = "0";
String from_name = "0";

String chat_id_new_user = "0";

AsyncWebServer server(80);

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

  //alive
  sendTelegramBroadcast("[Bot Event] Startup");
  chat_id = config.telegram.chatId0;
  handleStatusRequest();

  //initialize TCS 
  tcsReader.begin();
  tcsWriter.begin();

  //initialize heartbeat timers
  timerLedPeriod.stop();
  timerLedPulse.stop();

  //initialize OTA
  // Port defaults to 8266
  ArduinoOTA.setPort(8266);
  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("telegram-doorman");
  // No authentication by default
  ArduinoOTA.setPassword(DEFAULT_OTA_PASSWORD);
  
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();

  //init webserver
  server.on("/", HTTP_GET, handleIndexPage);
  server.on("/get", HTTP_GET, handleGet);
  server.onNotFound(handle404);
  server.begin();

  Serial.println("-- setup end --\n\n\n");

  //start heartbeat
  timerLedPeriod.start();

  //turn off LED busy status
  digitalWrite(LED_BUILTIN, HIGH); 
}










void loop() {
  //part 1: handle OTA
  ArduinoOTA.handle();

  //part 2: handle TCS bus event
  if (tcsReader.hasCommand()) {
    uint32_t cmd = tcsReader.read();
    
    // loop event check
    for (uint8_t n=0; n<N_TCS_EVENTS; n++) {
      if (cmd == TcsEvents[n].cmd) {
        TcsEvents[n].event();
        break;
      }
    }
    
    if (config.sniffMode == true) handleSniffMode(cmd);
    else;
  }

  //part 3: handle telegram updates
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
        if ((strcmp(config.telegram.chatId0, chat_id.c_str()) == 0) ||
            (strcmp(config.telegram.chatId1, chat_id.c_str()) == 0) ||
            (strcmp(config.telegram.chatId2, chat_id.c_str()) == 0)) 
        {
          // loop event check
          for (uint8_t n=0; n<N_BOT_EVENTS; n++) {
            if (text == BotEvents[n].text) {
              BotEvents[n].event();
              break;
            }
            // break did not hit
            else if (n == N_BOT_EVENTS-1) handleUnknownRequest();
          }
        }
        // if unknown user, but want to register
        else if (text == "/start") handleStartRequest();
        // unknown user enters password
        else if (chat_id_new_user == chat_id) handlePasswordCheck();
        // if unknown user, and unknown command
        else; // no response
      }
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    bot_lasttime = millis();
  }

  //part 4: handle heartbeat
  timerLedPeriod.check();
  timerLedPulse.check();

}



