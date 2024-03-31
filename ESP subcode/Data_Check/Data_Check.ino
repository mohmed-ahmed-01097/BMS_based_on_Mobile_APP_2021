#include <ESP8266WiFi.h>                                                // esp8266 library
#include <FirebaseArduino.h>                                            // firebase library

#define FIREBASE_HOST   "bms-smart-grid-lab.firebaseio.com"             // the project name address from firebase id
#define FIREBASE_AUTH   "E8ZidnvxgsnDjZwOiRvYLNwMxakhr3hahCL6SHUr"      // the secret key generated from firebase
#define WIFI_SSID       "MAAM"                                          // input your home or public wifi name 
#define WIFI_PASSWORD   "01097502015Newmgh"                                      // password of wifi ssid

unsigned int i = 0; 
String ChipID = "MAAM_SP1_";
String Path;


void wifiCheck();
void wifiConnect();
void send_B(String path, String Sub_path, float value);
void send_F(String path, String Sub_path, float value);
void send_S(String path, String Sub_path, String value);
String int_to_str(unsigned int n);


void setup() {  
  Serial.begin(9600);             //Initialising if(DEBUG)Serial Monitor

  // Set the Configration of GPIO 2 
  pinMode(LED_BUILTIN, OUTPUT); 

  wifiConnect();              // connect to wifi
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                       // connect to firebase
}


void loop() {
  
  for(i=0;i<65536;i++){
    Path = "";
    Path = ChipID + int_to_str(i);

    send_B(Path , "/State" ,  true);
    send_B(Path , "/Triac" , false);

    send_F(Path , "/Volt"  , 220.00);
    send_F(Path , "/Amp"   ,  10.00);
    send_F(Path , "/Freq"  ,  50.00);
    send_F(Path , "/P_F"   ,   0.95);

    send_S(Path , "/Phone" ,"Ready");
    send_S(Path , "/ESP01" ,"Ready");
    
    for(int j = 0 ; j < 24 ; j++){ 
      String D_path = "/Day/" + int_to_str(j);

      send_F(Path + D_path , "Volt"  , 220.00);
      send_F(Path + D_path , "Amp"   ,  10.00);
      send_F(Path + D_path , "P_F"   ,   0.95);
    }

    for(int j = 0 ; j < 30 ; j++){ 
      String D_path = "/Month/" + int_to_str(j);

      send_F(Path + D_path , "S_VA"  , 2200.0);
      send_F(Path + D_path , "P_F"   ,   0.95);
    }

    for(int j = 0 ; j < 12 ; j++){ 
      String D_path = "/Year/" + int_to_str(j);

      send_F(Path + D_path , "S_VA"  , 2200.0);
      send_F(Path + D_path , "P_F"   ,   0.95);
    }
    
  }

  wifiCheck();
}


/////////////////////////////////////////////////////////////////////////////////////////

void wifiCheck(){
  if(WiFi.status() != WL_CONNECTED)
  {
    wifiConnect();
  }
  delay(10);
}

void wifiConnect(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                 // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID); Serial.println(" ...");

  int teller = 0;
  while (WiFi.status() != WL_CONNECTED)
  {                                             // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++teller); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());               // Send the IP address of the ESP8266 to the computer
}

void send_B(String path, String Sub_path, float value){
  Firebase.setBool(path + Sub_path , value );      //Set value 
  
  //Handle error 
  if (Firebase.failed()) { 
     Serial.print(path + Sub_path + "failed:"); 
     Serial.println(Firebase.error()); 
     delay(2500);  
     return; 
  } 
}

void send_F(String path, String Sub_path, float value){
  Firebase.setFloat(path + Sub_path , value );      //Set value 
  
  //Handle error 
  if (Firebase.failed()) { 
     Serial.print(path + Sub_path + "failed:"); 
     Serial.println(Firebase.error()); 
     delay(2500);  
     return; 
  } 
}

void send_S(String path, String Sub_path, String value){
  Firebase.setString(path + Sub_path , value );     //Set value 
  
  //Handle error 
  if (Firebase.failed()) { 
     Serial.print(path + Sub_path + "failed:"); 
     Serial.println(Firebase.error()); 
     delay(2500);  
     return; 
  } 
}

String int_to_str(unsigned int n){
  String ret = "";
  ret[0] = ( n        % 10)+ '0';
  ret[1] = ((n/10)    % 10)+ '0';
  ret[2] = ((n/100)   % 10)+ '0';
  ret[3] = ((n/1000)  % 10)+ '0';
  ret[4] = ((n/10000) % 10)+ '0';
  return ret;
}
///////////////////////////////////////////////////////////////////////////////////////
