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
const char *ssid = "ORACLE Academy - 2GHZ";
const char *pass = "babikelen";
// const char *ssid = "POCO M3";
// const char *pass = "12345678";

// inisialisasi server, untuk mengirim data ke server saya
const char *host = "http://kelembabantanahaz.komputasi.org";
// Fingerprint sertifikat SSL server (diperoleh dari sertifikat server yang valid)
// const char *fingerprint = "CF:2A:98:95:FF:D9:A3:B1:C0:F9:61:F9:BC:D1:AC:FE:49:F3:85:11";
HTTPClient http;

// Objek WiFiClientSecure untuk koneksi HTTPS
// BearSSL::WiFiClientSecure client;
WiFiClient client;

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
  delay(1000);
  // Connect to WiFi networkdelay(1000);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  Serial.println("Setup koneksi. . .");
  WiFi.mode(WIFI_STA); //This line hides the viewing of ESP as wifi hotspot
  WiFi.begin(ssid, pass);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
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

  // client.setFingerprint(fingerprint);
}

void loop()
{ 
  // inisialisasi nilai kelembaban
  int POT = analogRead(A0);
  // inisialisasi nilai kelembaban String
  String nilai1 = String(POT);
  String url = "/index.php/api/save_data/" + nilai1 ;
  String send = host + url;
  // http.begin(client, host);
  Serial.print("connecting to ");
  Serial.println(host);

  delay(1000);
  http.begin(client, send);
  Serial.print("Requesting URL: ");
  Serial.println(send);

  int httpCode = http.GET();
  // httpCode will be negative on error
  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      String payload = http.getString();
      Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

http.end(); //Free the resources
// jika koneksi ke server gagal maka akan muncul koneksi gagal di serial monitor
  // if (!client.connect(host, 443)) {
  //   Serial.println("connection failed");
  //   return;
  // }                              

  // Isi konten yang akan dikirimkan adalah alamat ip si esp
  // String url = "index.php/api/save_data/" + nilai1 ;
  // Serial.print("Requesting URL: ");
  // Serial.println(url);

  // // Mengirimkan request ke server
  // client.print(String("GET ") + url + " HTTP/1.1\r\n" +
  //              "Host: " + host + "\r\n" +
  //              "Connection: close\r\n\r\n");
  // unsigned long timeout = millis();
  // while (client.available() == 0) {
  //   yield();
  //   if (millis() - timeout > 5000) {
  //     Serial.println(">>> Client Timeout !");
  //     client.stop();
  //     return;
  //   }
  // }

  // read all the lines of the reply from server and print them to Serial
  // while (client.available()) {
  //   String line = client.readStringUntil('\r');
  //   Serial.print(line);
  // }

  // Serial.println();
  // Serial.println("closing connection");


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
