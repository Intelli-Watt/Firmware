#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "your-ssid";
const char* password = "your-password";
const char* mqtt_server = "your-hivemq-cloud-url";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  // Send telemetry
  String payload = "{\"device_id\":\"dev-001\",\"ts\":\"2025-10-26T10:20:30Z\",\"voltage_V\":230.4,\"current_A\":1.52,\"energy_wh\":12345.6}";
  client.publish("device/dev-001/telemetry", payload.c_str());
  delay(5000);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
