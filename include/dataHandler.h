#pragma once

#include <Arduino.h>

#define CONFIG_FILENAME "/config.txt"


struct WifiConfig
{
    char ssid[32];
    char password[32];
    char otaPass[16];
};

struct TelegramConfig
{
    char chatId0[16];
    char chatId1[16];
    char chatId2[16];
    char password[16];
};

struct TcsConfig
{
    uint32_t apartmentBell;
    uint32_t streetCall;
    uint32_t garageCall;
    uint32_t streetView;
    uint32_t garageView;
    uint32_t streetOpen;
    uint32_t garageOpen;
};


struct Config
{
    WifiConfig wifi;
    TelegramConfig telegram;
    TcsConfig tcs;
    
    bool partyMode;
    bool silentMode0;
    bool silentMode1;
    bool silentMode2;
    bool sniffMode;
    bool devMode;
    bool silentMode0Prev;
    bool silentMode1Prev;
    bool silentMode2Prev;
    bool sniffModePrev;
};


extern void setupConfig(void);
extern void saveConfig(Config &config);
extern bool loadConfig(Config &config);
