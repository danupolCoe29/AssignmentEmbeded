#include <ESP8266WiFi.h>
const char* ssid     = "guest";
const char* pass = "guest";

#include<DHT.h>
#define DHTPIN D7 
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE); 
#define analogPin A0 
//----------------------------------------//
// ThingSpeak Settings //
//----------------------------------------//
char thingSpeakAddress[] = "api.thingspeak.com";
String writeAPIKey = "BPHMQ1748BGG12NA"; 
WiFiClient client;

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, pass);
   
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("IP address.....");
  Serial.println(WiFi.localIP());
}

void loop () {
  int soil = map(analogRead(analogPin),0,1023,100,0); 
  float air_humidity = dht.readHumidity();
  float air_temperature = dht.readTemperature();
  String air_temp = (String) air_temperature; 
  String air_hum = (String) air_humidity; 
  String soil_hum=(String) soil;
  
  String data = "field1=" + air_temp + "&field2=" + air_hum + "&field3=" + soil_hum ; 
  String readdata ="air_temp=" + air_temp + "   air_hum=" + air_hum + "   soil_hum=" + soil_hum ;
  Serial.println(readdata);
  if (client.connect(thingSpeakAddress, 80)) {
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+writeAPIKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(data.length());
    client.print("\n\n");
    client.print(data); 
  }
  
  delay(6000);
}
