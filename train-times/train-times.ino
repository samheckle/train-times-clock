#include <WiFiNINA.h>       // use this for MKR1010 and Nano 33 IoT boards
#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>

// libraries to parse proto file
#include <snappy.h>
#include <SnappyProto.h>
#include <pb.h>

#include "arduino_secrets.h"

WiFiClient netSocket;               // network socket to server
WiFiSSLClient wifi;
const char server[] = "https://api-endpoint.mta.info";  // server name
String route = "/Dataservice/mtagtfsfeeds/nyct%2Fgtfs";              // API route
// request timestamp in ms:
long lastRequest = 0;
// interval between requests:
int interval = 5000;

uint8_t buffer[128];
size_t message_length;
bool status;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);               // initialize serial communication
  while (!Serial);        // wait for serial monitor to open

  // while you're not connected to a WiFi AP,
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(SECRET_SSID);           // print the network name (SSID)
    WiFi.begin(SECRET_SSID, SECRET_PASS);         // try to connect
    delay(2000);
  }

  // When you're connected, print out the device's network status:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - lastRequest > interval ) {

    Serial.println("making request");
    HttpClient http = HttpClient(wifi, server, 443);      // make an HTTP client
    http.beginRequest();
    http.get(route);  // make a GET request
    http.sendHeader("x-api-key", API_KEY);
    http.endRequest();

    // read the status code and body of the response
    int statusCode = http.responseStatusCode();
//    String response = http.responseBody();
    Serial.println(statusCode);
//    pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);
//    status = pb_decode(&stream, SimpleMessage_fields, &http.responseBody());
//    Serial.print("Status code: ");
//    Serial.println(statusCode);
//    Serial.print("Response: ");
//    Serial.println(response);
//
//    // parse the string into a JSONVar object:
//    JSONVar myObject = JSON.parse(response);
//
//    // if you know the object's keys, you can ask for their values:
////    Serial.print("temperature:\t");
////    Serial.println(myObject["temperature"]);
////    Serial.print("humidity:\t");
////    Serial.println(myObject["humidity"]);
//
//    // myObject.keys() can be used to get an array
//    // of all the keys in the object
//    JSONVar keys = myObject.keys();
//    // read over the object, one key at a time:
//    for (int i = 0; i < keys.length(); i++) {
//      JSONVar value = myObject[keys[i]];
//      // what's the key:
//      Serial.print(keys[i]);
//      Serial.print("\t");
//      // and what's the value of the key:
//      Serial.print(value);
//      // and what's the type of the value:
//      Serial.print("\ttype: ");
//      Serial.println(JSON.typeof(value));
//    }
    lastRequest = millis();
  }

}
