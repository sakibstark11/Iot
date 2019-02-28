// SSID:	VM3602012_1
// Protocol:	802.11n
// Security type:	WPA2-Personal
// Network band:	2.4 GHz
// Network channel:	6
// Link-local IPv6 address:	fe80::7cf2:2a72:a77b:e2a4%17
// IPv4 address:	192.168.0.59
// IPv4 DNS servers:	194.168.4.100
// 194.168.8.100
// Manufacturer:	Intel Corporation
// Description:	Intel(R) Wireless-AC 9260
// Driver version:	20.50.0.5
// Physical address (MAC):	18-1D-EA-54-33-3C
// Sakib
#include <vector>
#include <cstring>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h
// Initialize the OLED display using Wire library
SSD1306 display(0x3c, 5, 4);
// WiFi ssid and password, change it with your ssid and password:
const char* ssid = "tonystark"; 
const char* password = "tonystark"; 
// MQTT IP address of the PC/raspberry where you installed MQTT Server:
const char* mqttServer = "mqtt.coventry.ac.uk";
const unsigned int mqttPort = 8883;
const char* clientId = "ClientIDlion";  // unique client ID for this device
const char* mqttUsername = "302CEM";
const char* mqttPassword = "n3fXXFZrjw";
const char* mqttTopicLight1 = "302CEM/lion/light1";
const char* mqttTopicLight2 = "302CEM/lion/light2";
const char* dataFromSensors = "on"; // not used
// TLS/SSL certificate for encrypted mqtt:
const char* ca_cert = \ 
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIFYDCCA0igAwIBAgIURFc0JFuBiZs18s64KztbpybwdSgwDQYJKoZIhvcNAQEL\n" \
    "BQAwSDELMAkGA1UEBhMCQk0xGTAXBgNVBAoTEFF1b1ZhZGlzIExpbWl0ZWQxHjAc\n" \
    "BgNVBAMTFVF1b1ZhZGlzIFJvb3QgQ0EgMiBHMzAeFw0xMjAxMTIxODU5MzJaFw00\n" \
    "MjAxMTIxODU5MzJaMEgxCzAJBgNVBAYTAkJNMRkwFwYDVQQKExBRdW9WYWRpcyBM\n" \
    "aW1pdGVkMR4wHAYDVQQDExVRdW9WYWRpcyBSb290IENBIDIgRzMwggIiMA0GCSqG\n" \
    "SIb3DQEBAQUAA4ICDwAwggIKAoICAQChriWyARjcV4g/Ruv5r+LrI3HimtFhZiFf\n" \
    "qq8nUeVuGxbULX1QsFN3vXg6YOJkApt8hpvWGo6t/x8Vf9WVHhLL5hSEBMHfNrMW\n" \
    "n4rjyduYNM7YMxcoRvynyfDStNVNCXJJ+fKH46nafaF9a7I6JaltUkSs+L5u+9ym\n" \
    "c5GQYaYDFCDy54ejiK2toIz/pgslUiXnFgHVy7g1gQyjO/Dh4fxaXc6AcW34Sas+\n" \
    "O7q414AB+6XrW7PFXmAqMaCvN+ggOp+oMiwMzAkd056OXbxMmO7FGmh77FOm6RQ1\n" \
    "o9/NgJ8MSPsc9PG/Srj61YxxSscfrf5BmrODXfKEVu+lV0POKa2Mq1W/xPtbAd0j\n" \
    "IaFYAI7D0GoT7RPjEiuA3GfmlbLNHiJuKvhB1PLKFAeNilUSxmn1uIZoL1NesNKq\n" \
    "IcGY5jDjZ1XHm26sGahVpkUG0CM62+tlXSoREfA7T8pt9DTEceT/AFr2XK4jYIVz\n" \
    "8eQQsSWu1ZK7E8EM4DnatDlXtas1qnIhO4M15zHfeiFuuDIIfR0ykRVKYnLP43eh\n" \
    "vNURG3YBZwjgQQvD6xVu+KQZ2aKrr+InUlYrAoosFCT5v0ICvybIxo/gbjh9Uy3l\n" \
    "7ZizlWNof/k19N+IxWA1ksB8aRxhlRbQ694Lrz4EEEVlWFA4r0jyWbYW8jwNkALG\n" \
    "cC4BrTwV1wIDAQABo0IwQDAPBgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIB\n" \
    "BjAdBgNVHQ4EFgQU7edvdlq/YOxJW8ald7tyFnGbxD0wDQYJKoZIhvcNAQELBQAD\n" \
    "ggIBAJHfgD9DCX5xwvfrs4iP4VGyvD11+ShdyLyZm3tdquXK4Qr36LLTn91nMX66\n" \
    "AarHakE7kNQIXLJgapDwyM4DYvmL7ftuKtwGTTwpD4kWilhMSA/ohGHqPHKmd+RC\n" \
    "roijQ1h5fq7KpVMNqT1wvSAZYaRsOPxDMuHBR//47PERIjKWnML2W2mWeyAMQ0Ga\n" \
    "W/ZZGYjeVYg3UQt4XAoeo0L9x52ID8DyeAIkVJOviYeIyUqAHerQbj5hLja7NQ4n\n" \
    "lv1mNDthcnPxFlxHBlRJAHpYErAK74X9sbgzdWqTHBLmYF5vHX/JHyPLhGGfHoJE\n" \
    "+V+tYlUkmlKY7VHnoX6XOuYvHxHaU4AshZ6rNRDbIl9qxV6XU/IyAgkwo1jwDQHV\n" \
    "csaxfGl7w/U2Rcxhbl5MlMVerugOXou/983g7aEOGzPuVBj+D77vfoRrQ+NwmNtd\n" \
    "dbINWQeFFSM51vHfqSYP1kjHs6Yi9TM3WpVHn3u6GBVv/9YUZINJ0gpnIdsPNWNg\n" \
    "KCLjsZWDzYWm3S8P52dSbrsvhXz1SnPnxT7AvSESBT/8twNJAlvIJebiVDj1eYeM\n" \
    "HVOyToV7BjjHLPj4sHKNJeV3UvQDHEimUF+IIDBu8oJDqz2XhOdT+yHBTw8imoa4\n" \
    "WSr2Rz0ZiC3oheGe7IUIarFsNMkd7EgrO3jtZsSOeWmD3n+M\n" \
    "-----END CERTIFICATE-----\n";


WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);

// controlling electronics here
void electronics(String deviceID , String message){

    char* inputChars = const_cast<char*>(message.c_str());
    byte wordCount=0;
    String *  results;
    char* item = strtok(NULL,"+");

  while(item != NULL) {
      results [wordCount]= item;
      item = strtok(NULL, "+");
      wordCount++;
  }
}

    if (deviceID == mqttTopicLight1){
        if(message == "1"){
            digitalWrite(25, HIGH);
        }
        else if (message == "0"){
            digitalWrite(25, LOW);
        }
        else {digitalWrite(25, LOW);}
    }
}
// The receivedCallback() function will be invoked when this client receives data about the subscribed topic
void receivedCallback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message received: ");
    String convertedTopic (topic);
    Serial.println(convertedTopic);
    Serial.println("payload: ");
    String store = "";
    char * testPayload;
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
        store += (char)payload[i];
        testPayload[i] = (char)payload[i];
    }
    ;
 // push here
   
    delay(500);
    electronics(convertedTopic, store);
    display.drawString(0,20, store);
    display.display();
    delay(1000);
}
// Will attempt to connect to MQTT and subscribe to a topic feed
void mqttConnect() {
    // Loop until reconnected
    while (!mqttClient.connected()) {
        display.clear();
        display.drawString(0, 10, "MQTT connecting...");
        display.display();
        if (mqttClient.connect(clientId, mqttUsername, mqttPassword)) {
            display.drawString(0, 20, "Connected");
            display.display();
            delay(1000);
            Serial.println("Mqtt On");
            mqttClient.subscribe(mqttTopicLight1);//
            mqttClient.subscribe(mqttTopicLight2); // topic name MUST be in the format: 302CEM/<your_username>/<topic_name>
        } 
        else {
            display.drawString(0, 20, "...connect failed, status code =");
            display.drawString(0, 30, String(mqttClient.state()));
            display.drawString(0, 40, "try again in 5 seconds");
            display.display();
            delay(5000);
        }
    }
}

void setup() {
    pinMode(25, OUTPUT);
    display.init();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.clear();
    display.drawString(0, 0, "LION");
    display.drawString(0, 10, "Attempting to connect to:");
    display.drawString(0, 20, String(ssid));
    display.display();
    delay(500);
    Serial.begin(115200);
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
    display.clear();
    display.drawString(0, 0, "WiFi connected");
    display.drawString(0, 10, "IP address: " + WiFi.localIP().toString());
    display.drawString(0, 20, "Setting up MQTT...");
    display.display();
    delay(3000);
    espClient.setCACert(ca_cert);
    mqttClient.setServer(mqttServer, mqttPort); // Port 8883 for MQTT over SSL.
// The receivedCallback() function will be invoked when this client receives the subscribed topic:
    mqttClient.setCallback(receivedCallback);
}
void loop() {
    display.clear();
    display.drawString(0, 0, "TestMode");
    display.display();
    delay(10);
    // if client was disconnected then try to reconnect again
    if (!mqttClient.connected()) {
        mqttConnect(); // displays on line 0, 1 and 2
    }
    // this function will listen for incoming subscribed topic-process-invoke receivedCallback()
    mqttClient.loop();
//  mqttClient.publish(mqttTopic, dataToSend.c_str());
}

