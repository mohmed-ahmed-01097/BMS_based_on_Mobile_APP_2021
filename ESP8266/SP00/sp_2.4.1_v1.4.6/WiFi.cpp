
#include "WiFi.h"                                                       // WiFi header file
#include "Process.h"                                                    // Process library
#include "Firebase.h"                                                   // Firebase library
#include "EEPROM.h"                                                     // eeprom library
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>

/* Put IP Address details */
IPAddress local_IP(192,168,1,1);          
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0); 

AsyncWebServer server(80);                                            //Establishing Local server at port 80 whenever required

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

S_MyData *_DATA;
String _SSID      = "";
String _PASS      = "";  
String AP_SSID    = "";
String APs_Scan   = "";
String APs_Change = "";
bool APs_Scan_F   = false;
uint8_t Timer_N   = 0;
void Set_Data_Ptr( S_MyData *_data){
  _DATA = _data;
  server.end();
    // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
void WiFi_Init(void){
  
  EEPROM_Read_WiFi(_SSID,_PASS);                                      // read the stor WiFi
    
  if(_SSID == "⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮"){
    _SSID = String(WIFI_SSID);
    _PASS = String(WIFI_PASS);
  }
  
  WiFi.softAPConfig(local_IP, gateway, subnet);
  //WiFi.setAutoConnect  (true);                                        // Autoconnect to last known Wifi on startup
  WiFi.setAutoReconnect(true);
  WiFi.mode(WIFI_STA);                                                // make WiFi mode as STATION
  WiFi.begin(_SSID.c_str(), _PASS.c_str());                         // begin the connection 
  
  WiFi_Check();                                                         // Check the WiFi 
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ~~msec
void WiFi_Check(void){

  while(!WiFi_Test()){                                             // Test the WiFi
    WiFi_Setup();                                               // Change the Store WiFi 
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
  Serial.print(WiFi.SSID()); Serial.println(" ...");
  for (int i = 0 ; i < 50 ; i++) {                                      // during 10sec 
    delay(250);                                                         // wait 250msec to reconnect
    Serial.print("*");
    if (WiFi.status() == WL_CONNECTED) {                                // well connected
      Serial.println("");
      Serial.println("Connection established!"); 
      Serial.print("Connected to ");
      Serial.println(WiFi.SSID()); 
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
// >5010msec
// Connecting to MAAM ...
// **************************************************
// Connect timed out,,,
// ********************
// Connection established!
// Connected to MAAM
// IP address:  192.168.1.6
// Succesfully Connected!!!
void WiFi_ShortTest(void){
  static uint32_t last = millis();
  // if WiFi is down, try reconnecting
  if ((WiFi.status() != WL_CONNECTED) && ((millis() - last) >= 5000 || (last > millis() && (last - millis()) >= 5000))) {
    //Serial.print(millis());
    //Serial.println("Reconnecting to WiFi...");
    WiFi.reconnect();                                                      // wait 500msec to reconnect
    last = millis();
    Serial.print("Connecting to ");
    Serial.println(WiFi.SSID());
    if(WiFi.SSID()=="")
      WiFi.begin(_SSID.c_str(), _PASS.c_str());                         // begin the connection 
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// > 180msec to handle Client
// Connection Failed! Rebooting...
// Turning the HotSpot On
// Initializing_softap_for_wifi credentials_modification
// over
// Local IP: 192.168.1.100
// SoftAP IP: 192.168.1.1
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
  while(WiFi.status() != WL_CONNECTED) { 
    delay(0);                                                         // wait 100msec to reconnect
    WiFi_ScanAP();
    WiFi_ShortTest();
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
  /*
  if (WiFi.status() == WL_CONNECTED) {                                  // well connected
    Serial.println("WiFi connected");
    return;
  }
  */
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  
  WiFi_CreatWebServer();                                                // Creat the server
  server.begin();                                                       // Start the server
  Serial.println("HTTP Server Started");                                // Actually start the server
  Serial.println();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// >68msec
// Initializing_softap_for_wifi credentials_modification
// over
void WiFi_SetupMode(void){
  
  if(WiFi.status() == WL_CONNECTED) {                                   // well connected
    delay(5000);
    server.end();     // remove all rewrites, handlers and onNotFound/onFileUpload/onRequestBody callbacks
    Serial.println("HTTP Server Closed");                             // Actually close the server 
    WiFi.mode(WIFI_STA);                                                // make WiFi mode as STATION
    //ESP.restart();                                                    //restarts the CPU.
    return;
  }
  
  String E_SSID = "";
  String E_PASS = "";
  EEPROM_Read_AP(E_SSID,E_PASS);                                        // read the stor WiFi
    
  if(E_SSID == "⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮"){
    E_SSID  = "ESP_" + String(ESP.getChipId(), HEX);
    E_PASS  = "12345678";
    AP_SSID = "ESP_" + String(ESP.getChipId(), HEX);
  }else{
    AP_SSID = E_SSID.c_str();  
  }
  
  Serial.println(E_SSID);
  Serial.println(E_PASS);
  
  WiFi.mode(WIFI_AP_STA);                                                 // make WiFi mode as AP and STATION
  WiFi.softAP(E_SSID.c_str(), E_PASS.c_str(), CHANNEL, false, MAX_CONN);  // initilize the AP to start 
  //Serial.println("Initializing_softap_for_wifi credentials_modification");
  //Serial.println("over"); 
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int getRSSIasQuality(int RSSI){
  if (RSSI <= -100)
    return 0;
  else if (RSSI >= -50)
    return 100;
  else
    return 2 * (RSSI + 100);
}
// >2,474sec
void WiFi_ScanAP(void){
  if(APs_Scan_F){
    String st = "";
    int n = WiFi.scanNetworks();                                          // Number of Network
    
    if(n == WIFI_SCAN_FAILED)
      Serial.println("[ERROR] scan failed");
    else if(n == WIFI_SCAN_RUNNING){
      Serial.println("[ERROR] scan waiting");
      while(WiFi.scanComplete() == WIFI_SCAN_RUNNING)
        Serial.println(".");
      n = WiFi.scanComplete();
    }
    else if(n == 0 )
      Serial.println("No Access Point Found");
    else if( n > 0 ){
      int* indices = (int *)malloc(n * sizeof(int));
      //sort networks
      for (int i = 0; i < n; i++)
        indices[i] = i;
      // RSSI SORT
      for (int i = 0; i < n; i++){
        for (int j = i + 1; j < n; j++){
          if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i]))
            std::swap(indices[i], indices[j]);
        }
      }
      st = "<h4><ol>";
      for (int i = 0; i < n; ++i) {                                       // upload SSID and RSSI for each network found
        
        st += "<li>";
        int rssiperc = getRSSIasQuality(WiFi.RSSI(i));
        String typeopen = (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? "" : "l";
        String item = "<div><a href='#p' onclick='c(this)'>{v}</a><div role='img' aria-label='{r}%' title='{r}%' class='q q-{q} {i} '></div><div class='q '>{r}%</div></div><br/>";
        item.replace("{v}", WiFi.SSID(i));
        item.replace("{r}", (String)rssiperc);
        item.replace("{q}", (String)int(round(map(rssiperc,0,100,1,4))));
        item.replace("{i}", (String)typeopen);
        
        st += item;
        st += "</li>";
      }
      st += "</ol></h4>";
    }
    APs_Scan = st;
    APs_Scan_F = false;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WiFi_CreatWebServer(void){
  
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/" + String(menu[0]) + ".html", String(), false, processor);
  });
  
  // Route for root / web page
  server.on("/Home", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/" + String(menu[0]) + ".html", String(), false, processor);
  });
  // Route for root / web page
  server.on("/Timers", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/" + String(menu[2]) + ".html", String(), false, processor);
  });
  // Route for root / web page
  server.on("/Config", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/" + String(menu[3]) + ".html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  static int statusCode = 200;
  static String content;
    
/***********************************************************************************************************************************************/    

  // Get GPIO State
  server.on("/state", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(_DATA->Switch.D_State).c_str());
    //Serial.println("state");
  });
  // Set GPIO State
  server.on("/toggle", HTTP_GET, [](AsyncWebServerRequest *request){ 
    if(_DATA->Switch.D_State != bool(atoi (request->getParam("state")->value().c_str ()))){
      _DATA->Switch.Toggle_F = 1;
    }
    //Serial.println("toggle");
    request->send_P(200, "text/plain", "Success!");
  });
  
/***********************************************************************************************************************************************/    
  
  server.on("/Time", HTTP_GET, [](AsyncWebServerRequest *request){ 
    String _time = "";
    {
      _time = request->getParam("time")->value();
      Serial.println(_time);
      _DATA->Date.MM = _time.substring(  0,  2).toInt();
      Serial.println(_time.substring(  0,  2));
      _DATA->Date.DD = _time.substring(  3,  5).toInt();
      Serial.println(_time.substring(  3,  5));
      _DATA->Date.YY = _time.substring(  6,  8).toInt()+100;
      Serial.println(_time.substring(  6,  8));
      _DATA->Now.Hr  = _time.substring( 10, 12).toInt();
      Serial.println(_time.substring( 10, 12));
      _DATA->Now.Min = _time.substring( 13, 15).toInt();
      Serial.println(_time.substring(  13, 15));
      _DATA->Sec     = _time.substring( 16, 18).toInt(); 
      Serial.println(_time.substring( 16, 18));
    }
    Serial.println(_time);
    request->send(SPIFFS, "/" + String(menu[0]) + ".html", String(), false, processor);
  });
  server.on("/get_time", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", (String(_DATA->Now.Hr) + " : " + String(_DATA->Now.Min)).c_str());
  });
  // Route for root / web page
  server.on("/EditTime", HTTP_POST, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/EditTime.html", String(), false, processor);
  });
  server.on("/SaveTime", HTTP_POST, [](AsyncWebServerRequest *request){ 
    if (request->hasParam("t_h", true))
      if(atoi (request->getParam("t_h", true)->value().c_str ()))
        _DATA->Now.Hr  = atoi (request->getParam("t_h", true)->value().c_str ());
    if (request->hasParam("t_m", true))
      if(atoi (request->getParam("t_m", true)->value().c_str ()))
        _DATA->Now.Min = atoi (request->getParam("t_m", true)->value().c_str ());
    if (request->hasParam("d_d", true))
      if(atoi (request->getParam("d_d", true)->value().c_str ()))
        _DATA->Date.DD = atoi (request->getParam("d_d", true)->value().c_str ());
    if (request->hasParam("d_m", true))
      if(atoi (request->getParam("d_m", true)->value().c_str ()))
        _DATA->Date.MM = atoi (request->getParam("d_m", true)->value().c_str ());
    if (request->hasParam("d_y", true))
      if(atoi (request->getParam("d_y", true)->value().c_str ()))
        _DATA->Date.YY = atoi (request->getParam("d_y", true)->value().c_str ()) - START_DATE;
        
      if(_DATA->Now.Hr  >= MAX_HOURS  )   _DATA->Now.Hr  = 0;
      if(_DATA->Now.Min >= MAX_MINUTES)   _DATA->Now.Min = 0;
      if(_DATA->Date.DD  >= MAX_DAYS  )   _DATA->Date.DD = 1;
      if(_DATA->Date.MM  >= MAX_MONTHS)   _DATA->Date.MM = 1;
      if(_DATA->Date.YY  >= 200       )   _DATA->Date.YY = 121;
    request->send(SPIFFS, "/" + String(menu[0]) + ".html", String(), false, processor);
  });

/***********************************************************************************************************************************************/    
   
  server.on("/timer", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(_DATA->Timer[atoi (request->getParam("t")->value().c_str ())].State).c_str());
  });
  server.on("/timer_state", HTTP_GET, [](AsyncWebServerRequest *request){
    uint8_t i = atoi (request->getParam("t")->value().c_str ()); 
    _DATA->Timer[i].State = bool(atoi (request->getParam("state")->value().c_str ()));
    request->send_P(200, "text/plain", "Success!");
  });
  // Route for root / web page
  server.on("/EditTimer", HTTP_POST, [](AsyncWebServerRequest *request){
    Timer_N = atoi (request->getParam("t", true)->value().c_str ()); 
    request->send(SPIFFS, "/EditTimer.html", String(), false, processor);
  });
  server.on("/SetTimer", HTTP_POST, [](AsyncWebServerRequest *request){ 
    if (request->hasParam("t", true)){
      uint8_t i = atoi (request->getParam("t", true)->value().c_str ()); 
      if (request->hasParam("t_s_h", true))
        if(atoi (request->getParam("t_s_h", true)->value().c_str ()))
          _DATA->Timer[i].Start.Hr  = atoi (request->getParam("t_s_h", true)->value().c_str ());
      if (request->hasParam("t_s_m", true))
        if(atoi (request->getParam("t_s_m", true)->value().c_str ()))
          _DATA->Timer[i].Start.Min = atoi (request->getParam("t_s_m", true)->value().c_str ());
      if (request->hasParam("t_e_h", true))
        if(atoi (request->getParam("t_e_h", true)->value().c_str ()))
          _DATA->Timer[i].End.Hr    = atoi (request->getParam("t_e_h", true)->value().c_str ());
      if (request->hasParam("t_e_m", true))
        if(atoi (request->getParam("t_e_m", true)->value().c_str ()))
          _DATA->Timer[i].End.Min   = atoi (request->getParam("t_e_m", true)->value().c_str ());

      if(_DATA->Timer[i].Start.Hr  >= MAX_HOURS  )   _DATA->Timer[i].Start.Hr  = 0;
      if(_DATA->Timer[i].Start.Min >= MAX_MINUTES)   _DATA->Timer[i].Start.Min = 0;
      if(_DATA->Timer[i].End.Hr    >= MAX_HOURS  )   _DATA->Timer[i].End.Hr    = 0;
      if(_DATA->Timer[i].End.Min   >= MAX_MINUTES)   _DATA->Timer[i].End.Min   = 0;
    }
    request->send(SPIFFS, "/" + String(menu[2]) + ".html", String(), false, processor);
  });
/***********************************************************************************************************************************************/
 
  server.on("/EditAP", HTTP_POST, [](AsyncWebServerRequest *request){
    APs_Change = "";
    request->send(SPIFFS, "/EditAP.html", String(), false, processor);
  });
  server.on("/ChangeAP", HTTP_POST, [](AsyncWebServerRequest *request){
    String N_SSID , N_PASS , N_CONF ;
    if (request->hasParam("ssid", true))
      N_SSID = request->getParam("ssid", true)->value();
    Serial.println(N_SSID);
    if (request->hasParam("pass", true))
      N_PASS = request->getParam("pass", true)->value();
    Serial.println(N_PASS);
    if (request->hasParam("conf", true))
      N_CONF = request->getParam("conf", true)->value();
    Serial.println(N_CONF);
      
    if (N_PASS == N_CONF){
      APs_Change = "Passwords match<h2><h3>The Device Will Restart to save the Parameters ......";
      EEPROM_Write_AP(N_SSID,N_PASS);
    }else{
      APs_Change = "Passwords does not match!";
    } 
    //request->send_P(200, "text/plain", "Success!");  
    request->send(SPIFFS, "/CheckAP.html", String(), false, processor);
  });    
  server.on("/get_AP", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", APs_Change.c_str());
  });
  
/***********************************************************************************************************************************************/ 
  
  static uint8_t wifi_check = 0;
  server.on("/get_APs", HTTP_GET, [](AsyncWebServerRequest *request){
    APs_Scan_F = true;
    request->send_P(200, "text/plain", APs_Scan.c_str());
  });
  server.on("/get_WiFi", HTTP_GET, [](AsyncWebServerRequest *request){
    wifi_check++;
    if(wifi_check < 10 && WiFi.status() != WL_CONNECTED){ 
      request->send_P(200, "text/plain", ".........................Chicking.........................");
    }else{
      String state = "";
      switch (WiFi.status()){
        case WL_IDLE_STATUS:
          state = "WL_IDLE_STATUS";
        case WL_NO_SSID_AVAIL:
          state = "WL_NO_SSID_AVAIL";
        case WL_CONNECTED:
          state = "WL_CONNECTED";
        case WL_CONNECT_FAILED:
          state = "WL_CONNECT_FAILED";
        case WL_DISCONNECTED:
          state = "WL_DISCONNECTED";
        default:
          state = "UNKNOWN";
      }
      if(WiFi.status() == WL_CONNECTED){
        request->send_P(200, "text/plain", String("Connected successfully with " + String(WiFi.SSID()) + "Success:saved to eeprom... reset to boot into new wifi").c_str());
      }else{
        request->send_P(200, "text/plain", String("Connected Failed with " + String(WiFi.SSID()) + "Error: " + state).c_str());
      } 
    }
  });
  server.on("/setting", HTTP_POST, [](AsyncWebServerRequest *request){                     // when press on Setting
    String N_SSID , N_PASS;
    if (request->hasParam("ssid", true))
      N_SSID = request->getParam("ssid", true)->value();
    if (request->hasParam("pass", true))
      N_PASS = request->getParam("pass", true)->value();
    WiFi.disconnect(); 
    EEPROM_Write_WiFi(N_SSID,N_PASS);
    _SSID = N_SSID;    _PASS = N_PASS;
    WiFi.begin(_SSID.c_str(), _PASS.c_str());                                       // begin the connection 
    wifi_check = 0;
    //request->send_P(200, "text/plain", "Success!");    
    request->send(SPIFFS, "/CheckW.html", String(), false, processor);
  });
/***********************************************************************************************************************************************/
  
  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){  
    Serial.println();
    Serial.println("\t(.........................RESETING.........................)\t");
    Serial.println();
    ESP.reset();
  });   
  server.onNotFound([](AsyncWebServerRequest *request){
    request->send_P(404, "text/html", "<html><body><h1>\"Error 404\":\" not found\"</h1></body></html>");
  });  
/***********************************************************************************************************************************************/
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// https://www.tutorialspoint.com/online_html_editor.php
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Replaces placeholder with LED state value
String processor(const String& var){
  Serial.println(var); 
  APs_Scan_F = false;
  if(var.charAt(0) == 'H'){
    return head[var.charAt(1) - '1'];
  }
  else if(var.charAt(0) == 'C'){
    return comment[var.charAt(1) - '1'];
  }
  
  else if(var.charAt(0) == 'T' && var.charAt(1) == '_'){
    if(var.charAt(2) == 'N'){
      if(var.length()>3){
        if(var.charAt(4) == 'S'){
          if(var.charAt(6) == 'M'){
            return String(_DATA->Timer[Timer_N].Start.Min);
          }else if(var.charAt(6) == 'H'){
            return String(_DATA->Timer[Timer_N].Start.Hr );          
          }
        }else if(var.charAt(4) == 'E'){
          if(var.charAt(6) == 'M'){
            return String(_DATA->Timer[Timer_N].End.Min);
          }else if(var.charAt(6) == 'H'){
            return String(_DATA->Timer[Timer_N].End.Hr );          
          }
        }else if(var.charAt(4) == 'N'){
            return String(Timer_N); 
        }
      }else{
        if(_DATA->Timer[Timer_N].State)
          return "checked";
        return "";
      }
    }
    if(var.length()>3){
      if(var.charAt(2) == 'S'){
        if(var.charAt(4) == 'M'){
          return String(_DATA->Timer[var.charAt(6) - '0'].Start.Min);
        }else if(var.charAt(4) == 'H'){
          return String(_DATA->Timer[var.charAt(6) - '0'].Start.Hr );          
        }
      }else if(var.charAt(2) == 'E'){
        if(var.charAt(4) == 'M'){
          return String(_DATA->Timer[var.charAt(6) - '0'].End.Min);
        }else if(var.charAt(4) == 'H'){
          return String(_DATA->Timer[var.charAt(6) - '0'].End.Hr );          
        }
      }
    }else{
      return String(_DATA->Timer[var.charAt(2) - '0'].State);
    }
  }
  
  else if(var == "IP"){
    IPAddress ip = WiFi.softAPIP();
    return String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);  
  }
  else if(var == "AP"){
    return AP_SSID;  
  }
  else if(var == "STATE"){
    if(_DATA->Switch.D_State)   
      return "ON";
    return "OFF";  
  }
  else if(var.substring(0,4) == "Time"){
    if(var.charAt(5) == 'H')
      return String(_DATA->Now.Hr);
    if(var.charAt(5) == 'M')
      return String(_DATA->Now.Min);  
  }
  else if(var.substring(0,4) == "Date"){
    if(var.charAt(5) == 'D')
      return String(_DATA->Date.DD);
    if(var.charAt(5) == 'M')
      return String(_DATA->Date.MM);
    if(var.charAt(5) == 'Y')
      return String(_DATA->Date.YY + START_DATE);
  }
  return String();
}

