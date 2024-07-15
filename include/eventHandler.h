#pragma once
#include <Arduino.h>

#define N_TCS_EVENTS 7
struct TcsEvent
{
  uint32_t cmd;
  void (*event)(void);
};
extern const TcsEvent TcsEvents[N_TCS_EVENTS];

#define N_BOT_EVENTS 12
struct BotEvent
{
  String text;
  void (*event)(void);
};
extern const BotEvent BotEvents[N_BOT_EVENTS];




extern void handleTcsStreetOpen(void);
extern void handleTcsGarageOpen(void);
extern void handleTcsApartmentBell(void);
extern void handleTcsStreetCall(void);
extern void handleTcsGarageCall(void);
extern void handleTcsStreetView(void);
extern void handleTcsGarageView(void);
extern void handleSniffMode(uint32_t cmd);

extern void handleTcsStreetOpenCmd(void);
extern void handleTcsGarageOpenCmd(void);
extern void handlePartyModeRequest(void);
extern void handleSilentModeRequest(void);
extern void handleSniffModeRequest(void);
extern void handleDevModeRequest(void);
extern void handleStatusRequest(void);
extern void handleHelpRequest(void);
extern void handleInvalidRequest(void);
extern void handleStopRequest(void);
extern void handleUnknownRequest(void);
extern void handleStartRequest(void);
extern void handlePasswordCheck(void);
extern void handleFormatRequest(void);
extern void handleResetRequest(void);

extern void sendTelegramBroadcast(const String& text);
extern void LedPeriodTimeout(void);
extern void LedPulseTimeout(void);
