# 🚪telegram-doorman
Interact with your [TCS](https://www.tcsag.de/) or [Koch](https://www.kochag.ch/de/) intercom with a Telegram Bot and ESP8266

![example workflow](https://github.com/jctots/telegram-doorman/actions/workflows/main.yml/badge.svg)
[![Gitter chat](https://badges.gitter.im/gitterHQ/gitter.png)](https://app.gitter.im/#/room/#jctots-telegram-doorman:gitter.im)

<table border="0">
<tr><td>Open Door Demo<br><img src="/docs/demo.gif" alt="demo.gif" width="320"/></td>
    <td>Telegram Actions<br><img src="/docs/menu.png" alt="menu.png" width="320"/>
    <br><br>Example Notifications<br><img src="/docs/notifications.png" alt="notifications.png" width="320"/>
    </td>
</tr>
</table>

# 📱Features
- Multi-user: easily register/remove additional 2 telegram chatIDs (group chatID or user chatID) as bot user
- Silent mode (per user): disable notification of TCS bus events for specific user
- Party mode (all users): automatic door unlock every intercom call
- Sniff mode (admin user): forward all TCS bus events (for collecting protocol codes for initial configuration) 
- Dev mode (admin user): silent mode for other users and enable sniff mode (useful for debugging and testing)
- Simple DevOps environment via PlatformIO, Arduino OTA, and GitHub Actions

List of supported TCS:bus events:
- send open door command (up to two doors)
- receive notifications for intercom call
- receive notifications for apartment doorbell
- receive notifications during lift handset

List of implemented Telegram Bot commands:

| command       |description                          |user|
|---------------|-------------------------------------|-----|
| `/sdoor`      | Open Street door | all users |
| `/sdoooor`    | Keep Street door open longer (10 sec) | all users |
| `/gdoor`      | Open Garage door | all users|
| `/gdoooor`    | Keep Garage door open longer (10 sec) | all users|
| `/party`      | Party Mode (open door every intercom call) | all users |
| `/silent`     | Silent Mode (Disable all TCS:bus notifications) | all users |
| `/help`       | Show Help/other commands | all users |
| `/info`       | Show info and current settings | all users |
| `/sniff`      | Sniff mode (See all TCS:bus events) | admin user |
| `/dev`        | Dev mode (Silent others and enable sniff mode)  | admin user |
| `/start`      | Add current user as bot user (enter password) | unregistered user|
| `/stop`       | Remove current user from bot users  | registered user |
| `/reset`      | Reset configuration to defaults | admin user |
| `/restart`    | Restart doorman | admin user |


# ⚙️System Overview
<img src="/docs/context.drawio.png" alt="context.drawio.png" width="640"/>

# 🚀Getting Started
(Detailed user manual coming soon in /docs)
- Setup PlatformIO dev environment and perform build (or download latest .bin from release)
- Make interface circuit (see schematic)
- Setup Telegram Bot
- Flash, integrate and test
- Connect to temporary WiFi "doorman" and enter apartment's WiFI details
- Enable "Sniff Mode" to capture own TCS:Bus protocol codes
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

[Link to Interface Circuit Schematic Diagram](/docs/schematic.jpg)

# ✔️To-do 
- [ ] RESTful APIs/OpenAPI definitions and implementation to open door/subscribe to notifications
- [ ] Check if can include devcontainer setup
- [ ] find a way to power ESP from TCS bus P-Line without cracking sound (help!)
- [ ] proper cabling and housing
- [ ] support for [telegram-keyman](https://github.com/jctots/telegram-keyman) for complete keyless apartment entry!
- [ ] create user manual, getting started (sphinx-docs), integration manual
- [ ] low power mode/wake-up/proper scheduling for optional battery support
- [ ] PCB layout
- [ ] logs (reset counter, etc.) and error handling
- [ ] simple web server for wireless serial debugging
- [ ] add sample unit tests for TDD demo and Gherkin specs for BDD demo
- [ ] create GitHub actions for automated test and docs generation
- [ ] add sample domain and application models for DDD demo

# 🗻 Challenges
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


