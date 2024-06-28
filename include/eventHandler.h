#pragma once
#include <Arduino.h>

extern void sendTelegramBroadcast(const String& text);
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
extern void handleConfigRequest(void);
extern void handleHelpRequest(void);
extern void handleInvalidRequest(void);
extern void handleStopRequest(void);
extern void handleUnknownRequest(void);
extern void handleStartRequest(void);

extern void LedPeriodTimeout(void);
extern void LedPulseTimeout(void);
