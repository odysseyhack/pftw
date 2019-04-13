#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// DHT
#define DHTPIN 16
#define DHTTYPE DHT11

const char* ssid = "Odyssey Hackaton 2019";
const char* password =  "";

const char* mqtt_server = "192.168.0.147";
const int mqtt_port = 8884;
const char* mqtt_user = "user1";
const char* mqtt_password = "pass1";
const char* will_topic = "status/1";
const int will_qos = 1;
const bool will_retain = false;
const char* will_message = "0";

const char* farm_id = "1";
const char* temp_stream_id = "1";
const char* humidity_stream_id = "2";


DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password, will_topic, will_qos, will_retain, will_message)) {
      Serial.println("connected");
      // // Once connected, publish an announcement...
      // client.publish("outTopic", "hello world");
      // // ... and resubscribe
      // client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void getAndSendTemperatureAndHumidityData() {
  Serial.println("Collecting temperature data.");

  // Reading temperature or humidity takes about 250 milliseconds!
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");

  String temperature = String(t);
  String humidity = String(h);

  // // Just debug messages
  // Serial.print( "Sending temperature and humidity : [" );
  // Serial.print( temperature ); Serial.print( "," );
  // Serial.print( humidity );
  // Serial.print( "]   -> " );
  //
  // // Prepare a JSON payload string
  // String payload = "{";
  // payload += "\"temperature\":"; payload += temperature; payload += ",";
  // payload += "\"humidity\":"; payload += humidity;
  // payload += "}";
  //
  // // Send payload
  // char attributes[100];
  // payload.toCharArray( attributes, 100 );

  char value[ 10];
  humidity.toCharArray( value, 10);
  client.publish( "v1/farm/1/stream/1", value );
  Serial.println( value );
  temperature.toCharArray( value, 10);
  client.publish( "v1/farm/1/stream/2", value );
  Serial.println( value );
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  dht.begin();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 10000) {
    getAndSendTemperatureAndHumidityData();
    lastMsg = now;

    // ++value;
    // snprintf (msg, 50, "hello world #%ld", value);
    // Serial.print("Publish message: ");
    // Serial.println(msg);
    // client.publish("v1/farm1/stream/1", msg);
  }
}
