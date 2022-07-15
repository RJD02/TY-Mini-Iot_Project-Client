#include <Arduino_JSON.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

WiFiClient wifiClient;
Servo servo;

const char fingerPrint[] = "B5 86 32 41 D3 FB 3D 2B 56 26 0E A3 51 14 75 E8 DC FD 4A 9B";
String host = "pet-feeder-iot.herokuapp.com";
//String host = "localhost:7000";
const char *ssid = "Galaxy A50C43E";
const char *password = "########";
const String petID = "627fb231c8c138404ab0c088";

void setup()
{

  // Servo interfacing

  servo.attach(2); // D2 pin
  servo.write(0);

  delay(2000);

  connectToWifi();

  Serial.begin(9600);

  // Create new pet and store petID;
  //  petID = getNewPetID();
}

void loop()
{
  isToFeed();
  delay(2000);
  // openCloseGate();
}

void connectToWifi()
{
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Waiting for connection");
  }

  Serial.println("Connected...");
  delay(1000);
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Wi-Fi connected!");
  }
  delay(2000);
}

String getResponse(String path, String body, String methodData, String headersData)
{
  WiFiClientSecure httpsClient;
  httpsClient.setFingerprint(fingerPrint);
  httpsClient.setTimeout(10000);
  delay(1000);

  Serial.println("HTTPS Connecting");

  httpsClient.connect(host, 443);

  if (httpsClient.verify(fingerPrint, host.c_str()))
  {
    Serial.println("Verified " + path);
    if (body != "" and headersData != "") {
      String sending = methodData + " " + path + " " + String("HTTP/1.1\r\n") +
                        String("Host: ") + host + String("\r\n") +
                        headersData + String("\r\n\r\n") +
                        body + String("\r\n") +
                        String("Connection: close\r\n\r\n");
      Serial.println(sending);
      httpsClient.print(sending);
    } else {
      String sending = methodData + " " + path + " " + String("HTTP/1.1\r\n") +
                        String("HOST: ") + host + String("\r\n") +
                        String("Connection: close\r\n\r\n");
      Serial.println(sending);
      httpsClient.print(sending);
    }
    String response = "";
    while (httpsClient.connected())
    {
      String line = httpsClient.readStringUntil('\n');
      if (line == "\r")
      {
        Serial.println("Headers received");
        break;
      }
    }
    while (httpsClient.available())
    {
      String line = httpsClient.readStringUntil('\n');
      response += line;
    }
    return response;
  }
  Serial.println("Certificates don't match");
  return "";
}

void getAllPets()
{
  String response = getResponse("/pet", "", "GET", "");
  JSONVar data = JSON.parse(response);
  Serial.println(data["pets"]);
  for (int i = 0; i < data["pets"].length(); i++) {
    auto pet = data["pets"][i];
    Serial.println(pet["_id"]);
  }
}

void isToFeed()
{
  String response = getResponse("/pet/toFeed/" + petID, "", "GET", "");
  JSONVar data = JSON.parse(response);
  if (data["message"] == String("Yes")) {
    openGate();
    closeGate();
    sendConfirmation();
    return ;
  }
  Serial.println("Not fucking rotating, because you've haven't fed your pet");
}

void openGate()
{
  // make servo motot spin 180 degrees
  Serial.println("Opening");
  // Open
  for(int i = 0; i < 91; i++) {
    servo.write(i);
    delay(50);
  }
  delay(2000);
}

void closeGate() {
  Serial.println("Closing");
  for(int i = 90; i > -1; i--) {
    servo.write(i);
    delay(50);
  }
}

void sendConfirmation()
{
  // Send confirmation that food was fed
  JSONVar body;
  body["petID"] = petID;
  body["message"] = "Success";
  String jsonBody = JSON.stringify(body);
//  String body = "message=Success&petID=" + petID;
  String headersData = "Content-Type: application/json\r\nContent-Length: " + String(jsonBody.length());
  String response = getResponse("/pet/fed/", jsonBody, "POST", headersData);
  Serial.println(response);
}
