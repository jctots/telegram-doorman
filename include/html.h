// need to generate this file from .html files and inclide in build

#include <Arduino.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>

<html>
    <head>
        <title>Telegram Doorman Configuration Page</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <script>
            function submitMessage() {
              alert("Configuration Saved!");
              setTimeout(function(){ document.location.reload(false); }, 500);
            }
        </script>
    </head>
    
    <body>
        <h1>Telegram Doorman Configuration Page</h1>

        <form action="/get" target="hidden-form">
            
            <table>
                <tr><th>Setting</th>            <th>Value</th></tr>
                <tr><td>Wifi Name</td>          <td><input type="text" name="wifiSsid"          value=%wifiSsid%        required></td></tr>
                <tr><td>Wifi Password</td>      <td><input type="text" name="wifiPassword"      value=%wifiPassword%    required></td></tr>
                <tr><td>Admin Password</td>     <td><input type="text" name="wifiOtaPass"       value=%wifiOtaPass%     required></td></tr>
                <tr><td>Telegram ChatID 0</td>  <td><input type="text" name="telegramChatId0"   value=%telegramChatId0% required></td></tr>
                <tr><td>Telegram ChatID 1</td>  <td><input type="text" name="telegramChatId1"   value=%telegramChatId1% required></td></tr>
                <tr><td>Telegram ChatID 2</td>  <td><input type="text" name="telegramChatId2"   value=%telegramChatId2% required></td></tr>
                
            </table>
            <br>
            <input type="submit" value="Save Configuration" onclick="submitMessage()">
          </form>
          <iframe style="display:none" name="hidden-form"></iframe> 
    </body>
</html>

)rawliteral";