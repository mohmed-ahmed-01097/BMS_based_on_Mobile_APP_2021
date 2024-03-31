
#include "WiFi.h"                                                       // WiFi header file
#include "EEPROM.h"                                                     // eeprom library
#include <ESP8266HTTPClient.h>                                          // esp8266 HTTP
#include <ESP8266WebServer.h>                                           // esp8266 webserver 

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);          
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0); 

ESP8266WebServer server(80);                                            //Establishing Local server at port 80 whenever required

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
void WiFi_Init(void){

  WiFi.begin(WIFI_SSID, WIFI_PASS);                                     // Connect to the Default WiFi ~~ 78msec
  if(!WiFi_Test()){                                                     // Test the Connection         ~~ 400msec
    WiFi.disconnect();                                                  // disconnection to the Default WiFi
    
    Serial.println();
    Serial.println("Disconnecting current wifi connection");
    Serial.println();
    Serial.println("Startup");
    
    String E_SSID = "";
    String E_PASS = "";
    EEPROM_Read_WiFi(E_SSID,E_PASS);                                    // read the stor WiFi
    WiFi.begin(E_SSID.c_str(), E_PASS.c_str());                         // begin the connection 
    
    WiFi_Check();                                                       // Check the WiFi 
  }
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ~~msec
void WiFi_Check(void){

  while(WiFi.status() != WL_CONNECTED) {
      if(!WiFi_Test()){                                               // Test the WiFi
          WiFi_Setup();                                               // Change the Store WiFi 
      } 
  }  
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// >10010msec
// Connecting to MAAM ...
// **************************************************
// Connect timed out,,,
// ********************
// Connection established!
// Connected to MAAM
// IP address:  192.168.1.6
// Succesfully Connected!!!
bool WiFi_Test(void){
  
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID); Serial.println(" ...");
  for (int i = 0 ; i < 50 ; i++) {                                      // during 10sec 
    delay(200);                                                         // wait 500msec to reconnect
    Serial.print("*");
    if (WiFi.status() == WL_CONNECTED) {                                // well connected
      
      Serial.println("");
      Serial.println("Connection established!"); 
      Serial.print("Connected to ");
      Serial.println(WIFI_SSID); 
      Serial.print("IP address:\t");
      Serial.println(WiFi.localIP());  
      Serial.println("Succesfully Connected!!!");  
       
      return true;                                                      // true
    }
  }
  Serial.println("");
  Serial.println("Connect timed out,,,");
  
  return false;                                                          // false
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// > 180msec to handle Client
// Connection Failed! Rebooting...
// Turning the HotSpot On
// Initializing_softap_for_wifi credentials_modification
// over
// Local IP: (IP unset)
// SoftAP IP: 192.168.4.1
// HTTP Server Started
// Waiting.
// .....................................................................................
void WiFi_Setup(void){

  Serial.println("Connection Failed! Rebooting...");
  Serial.println("Turning the HotSpot On");
  
  WiFi_SetupMode();                                                     // Setup HotSpot
  WiFi_LaunchWeb();                                                     // lanch the web server

  Serial.println();
  Serial.println("Waiting.");
  while(WiFi.status() != WL_CONNECTED) {                                // well connected 
    server.handleClient();                                              // handel client
    delay(100);                                                         // wait 500msec to reconnect
    Serial.print(".");
  }
  WiFi_SetupMode();                                                     // Setup HotSpot 
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// >90msec
// Local IP: (IP unset)
// SoftAP IP: 192.168.4.1
// HTTP Server Started
void WiFi_LaunchWeb(void){
  
  Serial.println("");
  if (WiFi.status() == WL_CONNECTED) {                                  // well connected
    Serial.println("WiFi connected");
    return;
  }
  
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  
  WiFi_CreatWebServer();                                                // Creat the server
  server.begin();                                                       // Start the server
  Serial.println("HTTP Server Started");                                // Actually start the server
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// >68msec
// Initializing_softap_for_wifi credentials_modification
// over
void WiFi_SetupMode(void){
  
  if(WiFi.status() == WL_CONNECTED) {                                   // well connected
      WiFi.mode(WIFI_STA);                                              // make WiFi mode as STATION
      server.close();                                                   // close the server
      Serial.println("HTTP Server Closed");                             // Actually close the server 
      return;                                                           // exit
  }
  
  WiFi.mode(WIFI_AP_STA);                                               // make WiFi mode as AP and STATION
  WiFi.softAP("SMART_PLUG SP1", "12345678");                            // initilize the AP to start  
  //Serial.println("Initializing_softap_for_wifi credentials_modification");
  //Serial.println("over"); 
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// >2,474
String WiFi_ScanAP(void){

  String st = "";
  WiFi.disconnect();                                                    // disconnection to the WiFi
  delay(100);                                                           // scane  the near Network
  int n = WiFi.scanNetworks();                                          // Number of Network
  Serial.println("scan done");
  
  if (n == 0) {
    Serial.println("no networks found");
  }else {
    Serial.print(n);
    Serial.println("networks found");
    for (int i = 0; i < n; ++i) {                                     // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
  st = "<ol>";
  for (int i = 0; i < n; ++i) {                                       // upload SSID and RSSI for each network found
    st += "<li>";
    st += WiFi.SSID(i);
    st += " (";
    st += WiFi.RSSI(i);
    st += ")";
    st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    st += "</li>";
  }
  st += "</ol>";
  delay(100);
  WiFi.reconnect();                                                    // disconnection to the WiFi
  return st;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WiFi_CreatWebServer(void){
  
  static int statusCode = 200;
  static String content;
    server.on("/", []() {
      server.send(200, "text/html", HTTP_Content());
    });

    server.on("/scan", []() {  
      server.send(200, "text/html", HTTP_Content());  
    }); 

    server.on("/setting", []() {                                                         // when press on Setting
      
      String N_SSID = server.arg("ssid");
      String N_PASS = server.arg("pass");
      
      if (N_SSID.length() > 0 && N_PASS.length() > 0) {
        WiFi.disconnect(); 
        EEPROM_Write_WiFi(N_SSID,N_PASS);
        WiFi.begin(N_SSID.c_str(), N_PASS.c_str());                                       // begin the connection 
        
        if(WiFi.status() == WL_CONNECTED){
          content  = "<!DOCTYPE HTML><html>\r\n";
          content += "<h1> Connected successfully with " + server.arg("ssid") + "</h1>";
          content += "<h2>\"Success\":saved to eeprom... reset to boot into new wifi</h2>";
          content += "</html>\n";
          statusCode = 200;
        }else{
          content  = "<!DOCTYPE HTML><html>\r\n";
          content += "<h1> Connected Failed with " + server.arg("ssid") + "</h1>";
          content += "<h2>\"Error\":UserName or Password is uncorrect</h2>";
          content += "</html>\n";
          statusCode = 200;
        }
        
        //ESP.reset();
      } else {
        content = "{\"Error\":\"404 not found\"}";
        statusCode = 404;
        Serial.println("Sending 404");
      }
      
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(statusCode, "text/html", content);  
      //server.send(statusCode, "application/json", content);
      WiFi_SetupMode();
      
    });
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// https://www.tutorialspoint.com/online_html_editor.php
String HTTP_Content(void){

  String content;
  IPAddress ip = WiFi.softAPIP();
  String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
  content =  "<!DOCTYPE HTML><html>\r\n";
  content += "<head>";
  content += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\r\n";
  content += "<link rel=\"icon\" href=\"data:,\">\r\n";
  content += "<style>";
  content += "html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\r\n";
  content += ".button { background-color: #195B6A; border: none; color: white; padding: 8px 20px; text-decoration: none; font-size: 20px; margin: 2px; cursor: pointer;border-radius: 4px;}\r\n";
  content += ".button2 { background-color: #77878A; border: none; color: white; padding: 8px 20px;}\r\n";
  content += "<title>ESP8266 Global Server</title>\r\n";
  content += "</style>\r\n";
  content += "</head>\r\n";
      
  content += "<body>\r\n";
  content += "<div id=\"webpage\">\r\n";
  content += "<h1>Smart Grid Lab <h1>\r\n";
  content += "<h2>ESP8266 Web Server</h2>\r\n";
  content += "<h3>Welcome to Wifi Credentials Update page</h3>\r\n";
      
  content += "<form action='scan' method='POST'>";
  content += "<button class='button'>SCAN</button></form>\r\n";
  content += ipStr;
  content += "<p>" + WiFi_ScanAP() + "</p>\r\n";
      
  content += "<form action='setting' method='GET'>";
  content += "<h3><label>SSID: </label><input type='text' name='ssid' placeholder='Username' length=32></h3>\r\n";
  content += "<h3><label>PASS: </label><input type='password'name='pass' placeholder='Password' length=64></h3>\r\n";
  content += "<button class='button'>SUBMIT</button></form>\r\n";
  content += "<p>If you click the 'Submit' button, the form-data will be sent to the Device</p>\r\n";
      
  content += "</div>\n";
  content += "</body>\n";
  content += "</html>\n";
  
  return content;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


