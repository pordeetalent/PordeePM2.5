/*
  Pordee PM2.5 Checker

  Project for Digital Economy Promotion Agency (depa) to get tempurature and PM2.5

  The circuit:
  * list the components attached to each input
  * list the components attached to each output

  Created 27 April 2021
  By Sirapol Nokyoongthong
  Modified 28 April 2021
  By Sira Nokyoongthong

  http://sirapol.nokyoongthong.com

*/

//start Config network and mqtt
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
const char* ssid = "SunNET";
const char* password = "maramara";

// Config MQTT Server
#define mqtt_server "202.139.197.40"
#define mqtt_port "1883"
#define mqtt_user "hapybot"
#define mqtt_password "Depa@1234"
#define mqtt_topic "temi4-temp"
#define mqtt_type "publish"
//end Config network and mqtt

#include <SoftwareSerial.h>

#include <Wire.h>
#include <Adafruit_MLX90614.h>
 
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

SoftwareSerial mySerial(D7,D8); // RX, TX
unsigned int pm1 = 0;
unsigned int pm2_5 = 0;
unsigned int pm10 = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) ;
  mySerial.begin(9600);
  
  mlx.begin();
}
 
void loop() {
  pm();
  bodytemp(); 
  delay(1000);
}
void pm() {
  int index = 0;
  char value;
  char previousValue;

  while (mySerial.available()) {
    value = mySerial.read();
    if ((index == 0 && value != 0x42) || (index == 1 && value != 0x4d)) {
      Serial.println("Cannot find the data header.");
      break;
    }

    if (index == 4 || index == 6 || index == 8 || index == 10 || index == 12 || index == 14) {
      previousValue = value;
    }
    else if (index == 5) {
      pm1 = 256 * previousValue + value;
      Serial.print("{ ");
      Serial.print("pm1: ");
      Serial.print(pm1);
      Serial.print(" ug/m3");
      Serial.print(", ");
    }
    else if (index == 7) {
      pm2_5 = 256 * previousValue + value;
      Serial.print("pm2.5: ");
      Serial.print(pm2_5);
      Serial.print(" ug/m3");
      Serial.print(", ");
    }
    else if (index == 9) {
      pm10 = 256 * previousValue + value;
      Serial.print("pm10: ");
      Serial.print(pm10);
      Serial.print(" ug/m3");
    }
    else if (index > 15) {
      break;
    }
      index++;
  }
  while (mySerial.available()) mySerial.read();
  Serial.println(" }");
}

void bodytemp() {
  Serial.print("Ambient = ");
  Serial.print(mlx.readAmbientTempC());
  Serial.print("*C\tObject = ");
  Serial.print(mlx.readObjectTempC()); Serial.println("*C");
//  Serial.print("Ambient = ");
//  Serial.print(mlx.readAmbientTempF());
//  Serial.print("*F\tObject = ");
//  Serial.print(mlx.readObjectTempF()); Serial.println("*F");

  Serial.println("");
}
