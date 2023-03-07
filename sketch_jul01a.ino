
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <Servo.h>

Servo myservo1; 
Servo myservo2;
Servo myservo3;
Servo myservo4;

const char* ssid = "Nags";
const char* password = "12345678";



//Server address
String serverName = "http://192.168.119.140:3000/api/position";


// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 100;

void setup() {
  Serial.begin(115200); 
  myservo1.attach(D5);  // attaches 
  myservo2.attach(D6);
  myservo3.attach(D7);
  myservo4.attach(D8);

//  int value1;
//  
//  int value2;
//  
//  int value3;
//  
//  int value4;
  

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

  }
  Serial.println("moving to logic");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
   int value1 = 90;
  
  int value2 = 0;
  
  int value3 = 90;
  
  int value4 = 180;
  // Send an HTTP POST request depending on timerDelay
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
//      digitalWrite(LED01, HIGH);
      WiFiClient client;
      HTTPClient http;

//      String serverPath = serverName;
      
      // Your Domain name with URL path or IP address with path
//      http.begin(client, serverPath.c_str());
      http.begin(client, serverName);
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload); 
        StaticJsonBuffer<200> jsonBuffer;

    JsonObject& root = jsonBuffer.parseObject(payload);


  value1 = root["base"];
  value2 = root["shoulder"];
  value3 = root["elbow"];
  value4 = root["grip"];
     
  int read1 = myservo1.read();
  int read2 = myservo2.read();
  int read3 = myservo3.read();  
  int read4 = myservo4.read();

  while(read1 != value1){
    read1++;
    myservo1.write(read1);
    delay(30);
  }
    while(read2 != value2){
    read2++;
    myservo2.write(read2);
    delay(30);
  }
     while(read3 != value3){
    read3++;
    myservo3.write(read3);
    delay(30);
  }
     while(read4 != value4){
    read4++;
    myservo4.write(read4);
    delay(30);
  }   }
      else {
        Serial.print("Error code1: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}


    
