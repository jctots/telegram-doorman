# 🚪telegram-doorman
Interact with your TCS intercom with a Telegram Bot and ESP8266

<table>
 <tr>
  <td rowspan="2"><img src="/docs/demo.gif" alt="demo.gif" width="300"/></td>
  <td><img src="/docs/menu.png" alt="menu.png" width="300"/></td>
 </tr>
 <tr>
  <td><img src="/docs/notifications.png" alt="notifications.png" width="300"/></td>
 </tr>

</table>



Currently supported TCS bus events 
- send open door command (up to two doors)
- receive notifications for intercom call
- receive notifications for apartment doorbell
- receive notifications during lift handset


# 📱Features
- Multi-user - easily register/remove additional 2 telegram chatIDs (group chatID or user chatID) as bot user
- Silent mode (per user) - disable notification of TCS bus events for specific user
- Party mode (all users) - door unlock every intercom call
- Sniff mode (main user) - forward all TCS bus events 
- Dev mode (main user) - silent mode for other users and enable sniff mode (useful for debugging and testing)

Other hardware-specific features:
- possible isolation of TCS BUS and ESP circuit via optocoupler when ESP is powered by separate supply 
- possible deisolation of TCS BUS and ESP circuit via jumper when ESP is powered by TCS bus P-Line
- separate pin for resetting config to defaults (short-to-gnd)

# ✅To-do 
- [ ] find a way to power ESP from TCS bus P-Line  without cracking sound
- [ ] proper cabling and housing
- [ ] support for [telegram-keyman](https://github.com/jctots/telegram-keyman) for complete keyless apartment entry!
- [ ] create user manual, getting started (sphinx-docs)
- [ ] add oscilloscope photos for documentation
- [ ] add section for known issues and limitations

# 🛠️Backlog 
- [ ] PCB layout
- [ ] OTA update
- [ ] logs (reset counter, etc.) and error handling
- [ ] simple HTML server to update config
- [ ] add tests for TDD demo
- [ ] add GitHub actions for automated test and docs generation
- [ ] add domain and application models for DDD demo

# 🗻Challenges
- from the [TCS Hacking YouTube video](https://youtu.be/xFLoauqj9yA?si=zF_Vwu4iNptckhIR), the TCS Bus A-Line is between 20V-24V, our system is between 7V-24V (verified using oscilloscope)
- in our apartment, we have 2 doors: main door (facing street) and garage door (inside tiefgarage)
- our system has 6-line TCS bus with video intercom (instead of the usual 2/3 line)
    - getting 5V from the 24V TCS bus P-line using DC-DC buck converter to power the ESP produces noise (cracking sound) in the video intercom (EMI/grounding issues?)
    - decided to use an optocoupler to separate the ESP ground from the TCS bus B-line

# ✏️TCS Bus References (auf Deutsch)
- [TCS_Systemhandbuch](https://www.tcsag.de/fileadmin/user_upload/TCS_DE/Metanavigation/Downloads/Publikationen/TCS_Systemhandbuch.pdf)
- [PI_IVW511x-0145](https://www.tcsag.de/fileadmin/user_upload/PI_IVW511x-0145.pdf)

# 📜Credits
- [atc1441's TCSintercomArduino](https://github.com/atc1441/TCSintercomArduino), for the YouTube video and TCS bus decoding
- [peteh's doorman](https://github.com/peteh/doorman), for the TCS bus library
- [AzonInc's Doorman](https://github.com/AzonInc/Doorman), for the insights on HW circuit
