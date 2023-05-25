// #include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#define DHTTYPE DHT11   // DHT 11

// #define WIFI_SSID "HHA2803"
// #define WIFI_PASSWORD "24082001"

#define WIFI_SSID "IoT LAB"
#define WIFI_PASSWORD "kvt1ptit"

#define mqtt_server "192.168.3.102"
#define MQTT_username "HHA2803"
#define MQTT_password "Hoangha2001"

#define DHTPin 2
#define lcdColumns 20
#define lcdRows 4
#define lamp 14
#define fan 12


DHT dht(DHTPin, DHTTYPE);
LiquidCrystal_I2C lcd(0x3F, lcdColumns, lcdRows); 
float h,t,f;

WiFiClient espClient;
PubSubClient client(espClient);

long now = millis();
long lastMeasure = 0;

void setup_wifi();
void callback(String topic, byte* message, unsigned int length);
void reconnect();

void setup() {
  pinMode(lamp, OUTPUT);
  pinMode(fan, OUTPUT);
  
  dht.begin();
  lcd.init();
  Serial.begin(115200);
  setup_wifi();
  lcd.backlight();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP8266Client", MQTT_username, MQTT_password);

  now = millis();
  // Publishes new temperature and humidity every 30 seconds
  if (now - lastMeasure > 30000) {
    lastMeasure = now;
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float humidity = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float temperatureC = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float temperatureF = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temperatureC) || isnan(temperatureF)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Publishes Temperature and Humidity values
    client.publish("SC/Temperature", String(temperatureC).c_str());
    client.publish("SC/Humidity", String(humidity).c_str());
    //Uncomment to publish temperature in F degrees
    //client.publish("room/temperature", String(temperatureF).c_str());
    
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.println(" ºC");
    Serial.print(temperatureF);
    Serial.println(" ºF");

    lcd.setCursor(5, 0);
    lcd.print("Smart Class");

    lcd.setCursor(0, 1);
    lcd.print("Temperature: ");
    lcd.print(temperatureC);
    lcd.print("*C");
     
    // lcd.setCursor(0, 2);
    // lcd.print("Temperature:");
    // lcd.print(temperatureF);
    // lcd.print(" ºF");

    lcd.setCursor(0,3);
    lcd.print("Humidity: ");
    lcd.print(humidity);
    lcd.print("%");

    if(temperatureC > 20) {
      digitalWrite(fan, LOW);
      Serial.print("AutoFanOn");
    }
    else {
      digitalWrite(fan, HIGH);
      Serial.print("AutoFanOff");
    }
    
    
  }



} 