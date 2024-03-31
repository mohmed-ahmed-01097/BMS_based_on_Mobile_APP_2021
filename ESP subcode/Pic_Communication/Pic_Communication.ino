/*
 * There are three serial ports on the ESP known as U0UXD, U1UXD and U2UXD.
 * 
 * U0UXD is used to communicate with the ESP32 for programming and during reset/boot.
 * U1UXD is unused and can be used for your projects. Some boards use this port for SPI Flash access though
 * U2UXD is unused and can be used for your projects.
 * 
*/
 
 
//#define RXD1 3
//#define TXD1 1

#define RXD2 25
#define TXD2 27
 
void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(9600);
  //Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1);
  //Serial.println("Serial Txd is on pin: "+String(TXD1));
  //Serial.println("Serial Rxd is on pin: "+String(RXD1));
  Serial2.begin(9600);
  //Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial Txd is on pin: "+String(TXD2));
  Serial.println("Serial Rxd is on pin: "+String(RXD2));
}
 
void loop() { //Choose Serial1 or Serial2 as required
 /*
 if (Serial1.available()) {
    Serial.print(char(Serial1.read()));
  }
 if(Serial.available()) {
    Serial1.write(char(Serial.read()));
  }
  */
  
  if (Serial2.available()) {
    Serial.print(char(Serial2.read()));
  }
 if(Serial.available()) {
    Serial2.write(char(Serial.read()));
  }
  
}

