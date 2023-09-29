/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPLySLeRMX8"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "o6mkbEWI10u-nRKFpUNiFBHzqrbs_qYc"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>



// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Maxx";
char pass[] = "maxmatiw30";

#include "DHT.h"
DHT dht;

BlynkTimer timer;

#define LED D4
#define RELAY D0

BLYNK_WRITE(V0)
{
  
  int Rv = param.asInt();
  if (Rv == 0){ 
    digitalWrite(RELAY, HIGH);
  }
  else{
    digitalWrite(RELAY, LOW);
  }
}
BLYNK_WRITE(V1)
{
  int state = param.asInt(); //looking for V0 value and store it to state variable
  if (state == 0){ //when satet value is 1 the led is on, vice-versa
    digitalWrite(LED, HIGH);
  }
  else{
    digitalWrite(LED, LOW);
  }
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}


void setup()
{
  // Debug console
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
  pinMode(RELAY, OUTPUT);
  pinMode(LED, OUTPUT);
  
  dht.setup(D1);
  digitalWrite(RELAY, HIGH);
  digitalWrite(LED, HIGH);  
}

void loop()
{
  Blynk.run();
  timer.run();
  
  float Temps = dht.getTemperature();
  Serial.print("Temperature : ");
  Serial.print(Temps);
  Serial.println(" °C");
  delay(1000);
  Blynk.virtualWrite(V3, Temps);
  if (Temps > 50) {
    digitalWrite(LED, LOW);
    Blynk.virtualWrite(V1, 0); // LED HIGH (on)
    Blynk.virtualWrite(V0, 0); // RELAY HIGH (off)
  }
  else
    digitalWrite(LED, HIGH);

}


