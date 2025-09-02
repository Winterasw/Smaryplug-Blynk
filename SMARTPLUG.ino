#define BLYNK_TEMPLATE_ID "TMPL6YZk1Sysm"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "kbzTwnJ0XVOtXbhi6M7X1a1lb5B9nBEj"
#define BLYNK_PRINT Serial
#define LED D4
#define RELAY D0
#define ANALOG_INPUT_SENSOR A0
#define currCalibration 120

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"
DHT dht;
#include <EmonLib.h>
EnergyMonitor emon1;
int timeS = 3580;
int relay = 0;
float kwhALL = 0;
float Temps;
double kwh = 0;

// WiFi Connetion 2.5 G
char ssid[] = "Asaiwn_2.4G";
char pass[] = "0909287745";
BlynkTimer timer;

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED() {
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent() {
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}

void setup() {
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
  Temps = 0;
  emon1.current(ANALOG_INPUT_SENSOR, currCalibration);  // Current: input pin, calibration.
  Serial.println("--- Smart Plug ON ---");
}

BLYNK_WRITE(V4) {

  int Rv = param.asInt();
  if (Rv == 0) {
    digitalWrite(RELAY, HIGH);
    relay = 1;
  } else {
    digitalWrite(RELAY, LOW);
    relay = 0;
  }
}
BLYNK_WRITE(V5) {
  int state = param.asInt();  //looking for V0 value and store it to state variable
  if (state == 0) {           //when satet value is 1 the led is on, vice-versa
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
}
void loop() {
  Blynk.run();
  timer.run();

  Temps = dht.getTemperature();
  Serial.print("Temperature : ");
  Serial.print(Temps);
  Serial.println(" °C");
  delay(1000);
  Blynk.virtualWrite(V6, Temps);
  if (Temps > 50) {
    digitalWrite(LED, LOW);
    Blynk.virtualWrite(V1, 0);  // LED HIGH (on)
    Blynk.virtualWrite(V0, 0);  // RELAY HIGH (off)
  } else
    digitalWrite(LED, HIGH);

  // Current Meter
  double Irms = emon1.calcIrms(1480);  // Calculate Irms only
  Serial.print(Irms * 230.0);
  Serial.print(" Watt  ");                 // Apparent power
  Blynk.virtualWrite(V7, (Irms * 230.0));  // power
  Serial.print(" ");
  Serial.print(Irms);
  Serial.println(" Amp");
  Blynk.virtualWrite(V8, Irms);  // current

  if (relay == 1) {
    timeS = timeS + 1;
    Serial.println(" - - - PLUG ON - - - ");
    Serial.print(timeS);
    Serial.println(" sec");
    kwh = ((Irms * 230.0) * (timeS / 3600)) / 1000;  // calculation Unit = kwh * consume tiome
    kwhALL = kwhALL + kwh;
    Blynk.virtualWrite(V9, kwhALL);
    Serial.print(kwh);
    Serial.print(" , ");
    Serial.print(kwhALL);
    Serial.println(" KwhALL");
    if (timeS >= 3600) {
      timeS = 1;
    }
  }
}
