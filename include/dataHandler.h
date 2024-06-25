#pragma once

#include <Arduino.h>

#define CONFIG_FILENAME "/config.txt"


struct WifiConfig
{
    char ssid[32];
    char password[32];
};

struct TelegramConfig
{
    char chatIdMain[16];
    char chatIdSub1[16];
    char chatIdSub2[16];
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
    bool silentModeMain;
    bool silentModeSub1;
    bool silentModeSub2;
    bool sniffMode;
    bool devMode;
    bool silentModeMainPrev;
    bool silentModeSub1Prev;
    bool silentModeSub2Prev;
    bool sniffModePrev;
};


extern void setupConfig(void);
extern void saveConfig(Config &config);
extern bool loadConfig(Config &config);
