#include <SPI.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFiNINA.h>

#define sensorPin A0
#define temt6000Pin D13
#define dhtType DHT11

DHT_Unified dht(sensorPin, dhtType);

char ssid[] = "ADB-93A7E1";
char pass[] = "cm3mk5r6re6k";

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println();

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  server.begin();

  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {

  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {

          if (currentLine.length() == 0) {

            sensors_event_t event;
            dht.temperature().getEvent(&event);
            if (isnan(event.temperature))
            {
              Serial.println("Error Teplota sa nedá prečítať!");
            }
            else
            {
              Serial.print("Teplota: ");
              Serial.print(event.temperature);
              Serial.print(" *C    ");
            }

            // Get humidity event and print its value.
            dht.humidity().getEvent(&event);
            if (isnan(event.relative_humidity))
            {
              Serial.println("Error Vlhkosť sa nedá prečítať!");
            }
            else
            {
              Serial.print("Vlhkosť: ");
              Serial.print(event.relative_humidity);
              Serial.println("%");
            }

            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }



  char status;
  double T, P, p0; //Creating variables for temp, pressure and relative pressure

  Serial.print("You provided altitude: ");
  Serial.println(" meters");

  status = pressure.startTemperature();
  if (status != 0) {
    delay(status);

    status = pressure.getTemperature(T);
    if (status != 0) {
      Serial.print("Temp: ");
      Serial.print(T, 1);
      Serial.println(" deg C");

      status = pressure.startPressure(3);

      if (status != 0) {
        delay(status);

        status = pressure.getPressure(P, T);
        if (status != 0) {
          Serial.print("Pressure measurement: ");
          Serial.print(P);
          Serial.println(" hPa (Pressure measured using temperature)");

          p0 = pressure.sealevel(P, ALTITUDE);
          Serial.print("Relative (sea-level) pressure: ");
          Serial.print(p0);
          Serial.println("hPa");
        }
      }
    }
  }

  int value = digitalRead(temt6000Pin);
  Serial.println(value); 
  delay(200);

    client.stop();
    Serial.println("client disonnected");
  }
}
