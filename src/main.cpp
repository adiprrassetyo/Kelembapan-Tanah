/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6CJx7m8sa"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "P2Lv5tBoOOW31AuHV5YtvMgEqqnAt-rN"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "ORACLE Academy - 2GHZ";
char pass[] = "babikelen";

// inisialisasi pin
int ledMerah = D4;
int ledKuning = D2;
int ledHijau = D3;
int Buzzer = D5;

// inisialisasi widget
WidgetLCD lcd(V5);
WidgetLCD LCD(V6);

void setup()
{
  // Debug console
  Serial.begin(9600);
  lcd.clear();
  LCD.clear();
  pinMode(ledMerah, OUTPUT);
  pinMode(ledKuning, OUTPUT);
  pinMode(ledHijau, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(ledMerah, LOW);
  digitalWrite(ledKuning, LOW);
  digitalWrite(ledHijau, LOW);
  digitalWrite(Buzzer, LOW);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{
  Blynk.run();
  int POT = analogRead(A0);
  lcd.print(0, 0, "KEADAAN");
  lcd.print(0, 1, "LED");
  LCD.print(0, 0, "COMMENT:");
  Blynk.virtualWrite(V4, POT);

  // jika nilai pot > 600 maka led merah menyala dan buzzer bunyi
  if (POT > 600)
  {
    lcd.print(8, 0, "KERING");
    lcd.print(4, 1, "MERAH ON");
    LCD.print(0, 1, "MEMBUTUHKAN AIR");
    digitalWrite(ledMerah, HIGH);
    digitalWrite(ledKuning, LOW);
    digitalWrite(ledHijau, LOW);
    digitalWrite(Buzzer, HIGH);
    delay(500);
    digitalWrite(Buzzer, LOW);
    delay(500);
  }

  // jika nilai pot > 500 && 600 < maka led hijau menyala
  if (POT > 500 && POT < 600)
  {
    lcd.print(8, 0, "NORMAL");
    lcd.print(4, 1, "HIJAU ON");
    LCD.print(0, 1, "AIR TERCUKUPI    ");
    digitalWrite(ledHijau, HIGH);
    digitalWrite(ledMerah, LOW);
    digitalWrite(ledKuning, LOW);
    digitalWrite(Buzzer, LOW);
  }

  // jika nilai pot < 500 maka led kuning menyala
  if (POT < 500)
  {
    lcd.print(8, 0, "BASAH");
    lcd.print(4, 1, "KUNING ON");
    LCD.print(0, 1, "KELEBIHAN AIR");
    digitalWrite(ledKuning, HIGH);
    digitalWrite(ledMerah, LOW);
    digitalWrite(ledHijau, LOW);
    digitalWrite(Buzzer, LOW);
  }
}
