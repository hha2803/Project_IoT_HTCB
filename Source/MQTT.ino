void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic room/lamp, you check if the message is either on or off. Turns the lamp GPIO according to the message
  if(topic=="SC/Lamp"){
      Serial.print("Changing Room lamp to ");
      if(messageTemp == "on"){
        digitalWrite(lamp, HIGH);
        Serial.print("LampOn");
      }
      else if(messageTemp == "off"){
        digitalWrite(lamp, LOW);
        Serial.print("LampOff");
      }
  }
  else if(topic=="SC/Fan"){
      Serial.print("Changing Room fan to ");
      if(messageTemp == "off"){
        digitalWrite(fan, HIGH);
        Serial.print("FanOn");
      }
      else if(messageTemp == "on"){
        digitalWrite(fan, LOW);
        Serial.print("FanOff");
      }
  }
  Serial.println();
  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client", MQTT_username, MQTT_password)) {
      Serial.println("connected");  
      client.subscribe("SC/Lamp");
      client.subscribe("SC/Fan");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}