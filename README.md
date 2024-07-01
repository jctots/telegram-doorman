# 🚪telegram-doorman
Interact with your [TCS](https://www.tcsag.de/) or [Koch](https://www.kochag.ch/de/) intercom with a Telegram Bot and ESP8266

![example workflow](https://github.com/jctots/telegram-doorman/actions/workflows/main.yml/badge.svg?branch=main)

<table border="0">
<tr><td>Open Door Demo<br><img src="/docs/demo.gif" alt="demo.gif" width="320"/></td>
    <td>Telegram Actions<br><img src="/docs/menu.png" alt="menu.png" width="320"/>
    <br><br>Example Notifications<br><img src="/docs/notifications.png" alt="notifications.png" width="320"/>
    </td>
</tr>
</table>

Currently supported TCS bus events:
- send open door command (up to two doors)
- receive notifications for open door
- receive notifications for intercom call
- receive notifications for apartment doorbell
- receive notifications during lift handset

# 📱Features
- Multi-user: easily register/remove additional 2 telegram chatIDs (group chatID or user chatID) as bot user
- Silent mode (per user): disable notification of TCS bus events for specific user
- Party mode (all users): automatic door unlock every intercom call
- Sniff mode (main user): forward all TCS bus events (for collecting protocol codes for initial configuration) 
- Dev mode (main user): silent mode for other users and enable sniff mode (useful for debugging and testing)
- Easy SW development via PlatformIO environment, Arduino OTA, and GitHub Actions

Other hardware-specific features:
- possible isolation of TCS BUS and ESP circuit via optocoupler when ESP is powered by separate supply 
- possible de-isolation of TCS BUS and ESP circuit via jumper when ESP is powered by TCS bus P-Line
- separate pin for resetting config to defaults (short-to-ground)

# 🌐System Overview
<img src="/docs/context.drawio.png" alt="context.drawio.png" width="640"/>

[Link to Interface Circuit Schematic Diagram](/docs/schematic.jpg)

# 🏃Getting Started
(Detailed user manual coming soon in /docs)
- Setup PlatformIO dev environment and build (or download latest .bin from release)
- Make interface circuit
- Setup Telegram Bot
- Flash, integrate and test
- Enable "Dev Mode" to capture own TCS:Bus protocol codes
- Change default config with captured codes
- Enjoy!

# 🔧Reference Circuit and Integration
(Detailed integration manual coming soon in /docs)
<table border="0">
  <tr>
    <td><img src="/docs/protoboard_with_d1mini.jpg" alt="protoboard_with_d1mini.jpg" width="200"/></td>
    <td><img src="/docs/intercom_open.jpg" alt="intercom_open.jpg" width="200"/></td>
    <td><img src="/docs/intercom_connected.jpg" alt="intercom_connected.jpg" width="200"/></td>
  </tr>
</table>

# 🚅To-do 
- [ ] find a way to power ESP from TCS bus P-Line without cracking sound
- [ ] proper cabling and housing
- [ ] support for [telegram-keyman](https://github.com/jctots/telegram-keyman) for complete keyless apartment entry!
- [ ] create user manual, getting started (sphinx-docs), integration manual (with oscilloscope photos)
- [ ] add section for known issues and limitations
- [ ] low power mode/wake-up/proper scheduling for optional battery support

# 🚂Roadmap 
- [ ] PCB layout
- [ ] logs (reset counter, etc.) and error handling
- [ ] simple HTML web server to update config
- [ ] simple web server for wireless serial debugging
- [ ] add sample unit tests for TDD demo
- [ ] create GitHub actions for automated test and docs generation
- [ ] add sample domain and application models for DDD demo

# 🗻Challenges
- from the [TCS Hacking YouTube video](https://youtu.be/xFLoauqj9yA?si=zF_Vwu4iNptckhIR), the TCS Bus A-Line is between 20V-24V, our system is between 7V-24V (verified using oscilloscope)
- in our apartment, we have 2 doors: main door (facing street) and garage door (inside tiefgarage)
- our system has 6-line TCS bus with video intercom (instead of the usual 2/3 line)
    - getting 5V from the 24V TCS bus P-line using DC-DC buck converter to power the ESP produces noise (cracking sound) in the video intercom (EMI/grounding issues?)
    - decided to use an optocoupler to separate the ESP ground from the TCS bus B-line
 
<img src="/docs/osc.jpg" alt="osc.jpg" width="640"/>
<img src="/docs/breadboard.jpg" alt="breadboard.jpg" width="640"/> 

# ✏️TCS System References 
- [TCS_Systemhandbuch](https://www.tcsag.de/fileadmin/user_upload/TCS_DE/Metanavigation/Downloads/Publikationen/TCS_Systemhandbuch.pdf)
- [PI_IVW511x-0145](https://www.tcsag.de/fileadmin/user_upload/PI_IVW511x-0145.pdf)

# 📜Credits
- [atc1441's TCSintercomArduino](https://github.com/atc1441/TCSintercomArduino), for the YouTube video and TCS bus decoding
- [peteh's doorman](https://github.com/peteh/doorman), for the TCS bus library
- [AzonInc's Doorman](https://github.com/AzonInc/Doorman), for the insights on ESPhome and home automation
- [witnessmenow's Universal-Arduino-Telegram-Bot](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot), for the Telegram library
- [Strooom's demoCloudBuilds](https://github.com/Strooom/demoCloudBuilds), for the PlatformIO-baesd CI/CD workflows
- [MrDIYca's wireless-serial-gateway](https://gitlab.com/MrDIYca/wireless-serial-gateway), for insights on serial wifi bridge


