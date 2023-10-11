// aplikasi ini akan mengirimkan data ke server express.js melalui http request dan melakukan kontrol led dan buzzer

/* Comment this out to disable prints and save space */
#define BAUDRATE 9200

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <ArduinoJson.h>


// Your WiFi credentials.
// Set password to "" for open networks.
// const char* ssid = "ORACLE Academy - 2GHZ";
// const char* pass = "babikelen";
const char *ssid = "POCO M3";
const char *pass = "qwertyuiop1";

// inisialisasi server, untuk mengirim data ke server saya
const char *host = "192.168.8.1";
HTTPClient http;

// http://localhost/IoT-Project-Site/index.php/api/save_data/800/700/   ini adalah contoh url yang akan dikirimkan ke server 

// inisialisasi pin
int ledMerah = D4;
int ledKuning = D2;
int ledHijau = D3;
int Buzzer = D5;


// inisialisasi setup
void setup()
{
  Serial.println("Setup NodeMCU Baudrate . . .");
  Serial.begin(BAUDRATE);
  // Connect to WiFi network
  Serial.println("Setup koneksi. . .");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  // jika koneksi berhasil, maka akan muncul IP address di serial monitor
  Serial.println("");
  Serial.println("Connected to");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Debug console
  pinMode(ledMerah, OUTPUT);
  pinMode(ledKuning, OUTPUT);
  pinMode(ledHijau, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(ledMerah, LOW);
  digitalWrite(ledKuning, LOW);
  digitalWrite(ledHijau, LOW);
  digitalWrite(Buzzer, LOW);
}

void loop()
{ 
  // inisialisasi nilai kelembaban
  int POT = analogRead(A0);
  // inisialisasi nilai kelembaban String
  String nilai1 = String(POT);

  Serial.print("connecting to ");
  Serial.println(host);

  // Mengirimkan ke alamat host dengan port 80
  WiFiClient client;
  if (!client.connect(host, 80)) {
    Serial.println("connection failed");
    return;
  }

  // Isi konten yang akan dikirimkan adalah alamat ip si esp
  String url = "/IoT-Project-Site/index.php/api/save_data/" + nilai1 ;
  Serial.print("Requesting URL: ");
  Serial.println(url);

  // Mengirimkan request ke server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");


  // jika nilai pot > 600 maka led merah menyala dan buzzer bunyi
  if (POT > 600)
  {
    Serial.println("KERING");
    Serial.println("MERAH ON");
    Serial.println("MEMBUTUHKAN AIR");
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
    Serial.println("NORMAL");
    Serial.println("HIJAU ON");
    Serial.println("AIR TERCUKUPI    ");
    digitalWrite(ledHijau, HIGH);
    digitalWrite(ledMerah, LOW);
    digitalWrite(ledKuning, LOW);
    digitalWrite(Buzzer, LOW);
  }

  // jika nilai pot < 500 maka led kuning menyala
  if (POT < 500)
  {
    Serial.println( "BASAH");
    Serial.println("KUNING ON");
    Serial.println("KELEBIHAN AIR");
    digitalWrite(ledKuning, HIGH);
    digitalWrite(ledMerah, LOW);
    digitalWrite(ledHijau, LOW);
    digitalWrite(Buzzer, LOW);
  }
}
