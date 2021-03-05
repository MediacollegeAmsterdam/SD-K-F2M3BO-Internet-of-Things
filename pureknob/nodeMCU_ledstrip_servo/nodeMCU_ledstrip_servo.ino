/*
 * Duurzaam Huis Led Control
 * Temperature + Humidity Sensor DHT11
 * MediaCollege Amsterdam feb 2021
*/

// network
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

//IO pins
#include "pinIO.h"

// Neopixel
#include "neoPixel.h"

// Servo
#include "servo.h"

// temperature + humidity sensor type DHT11
#include "DHT11.h"


// LDR
#include "ldr.h"

/*
 * Wifi Network
 * insert here your own network Wifi Credentials 
*/
#include "network.h" 




// Slow it down
const int wait = 10000;  // msec between data requests


void setup () {
  Serial.begin(115200); // start Serial monitor
  setupServo(); // init servo
  startNeoPixel(); // NeoPixel start
  dht.begin(); // start DHT11 sensor
}
 
void loop() {  
  if (WiFi.status() != WL_CONNECTED) wifiConnect();// reconnect Wifi if necessary 
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
     httpRequest(); // get data from server
     extractJson(); // execute data from server
     LightLevel = readLDR(); // get light sensor
     readDHT11();  // get sensor readings from DHT11
     httpRequest2(); // write data to server
  }
 
  //Serial.println(httpResponse);  //Print the response payload
  delay(wait);    //Send a request every XXX seconds
}



void httpRequest(){
  HTTPClient http;               //Declare an instance of class HTTPClient
  
  http.begin(server + path);     //Specify request destination
  int httpCode = http.GET();     //Send the request
   
  if (httpCode > 0) {            //Check the returning code
      httpResponse = http.getString();  //Get the request response payload                                          
     }
   http.end();   //Close connection  
  }



void httpRequest2(){
  
  //https://arduinojson.org/v6/api/jsonarray/
  StaticJsonDocument<200> jsonDoc;
  // create an empty array, arrayLocalData
  JsonArray arrayLocalData = jsonDoc.to<JsonArray>();
  arrayLocalData.add("temperature"); 
  arrayLocalData.add(Temperature); // value from DHT11
  arrayLocalData.add("humidity");
  arrayLocalData.add(Humidity); // value from DHT11
  arrayLocalData.add("heatindex");
  arrayLocalData.add(HeatIndex); // value from DHT11
  arrayLocalData.add("light");
  arrayLocalData.add(LightLevel); // value from DHT11
  String serverString;
  serializeJson(jsonDoc, serverString);
  
  HTTPClient http;                                       //Instance of class HTTPClient
  String url = server + path2  + serverString;
  Serial.println(url);
  http.begin(url);                                      //Specify request destination
  int httpCode = http.GET();                            //Send the request  
  }

void extractJson() {                //extract JSON string from HTTP data
  //Serial.println(httpResponse);
  // JSONLib 6 https://arduinojson.org/v6/api/jsonarray/
  int size = httpResponse.length() + 1;
  char jsonString[size];
  httpResponse.toCharArray(jsonString, size);
  StaticJsonDocument<200> jsonDoc;
  deserializeJson(jsonDoc, jsonString);// Deserialize the JSON document
  JsonArray dataObj = jsonDoc.as<JsonArray>();
  parseCommands(dataObj);
 } 

void parseCommands(JsonArray dataObj){
  int red,green,blue = 0;
  int servoAngle;
  if(dataObj[0] == "on"){
    Serial.print("red value: ");
    red = dataObj[1];  // neopixel red  
    Serial.println(red);
    }
  else{
    red = 0 ; // turn it off
    }

  if(dataObj[2] == "on"){
    Serial.print("green value: ");
    green = dataObj[3];
    Serial.println(green);
    }
   else{
    green = 0; // turn it off
    }  


   if(dataObj[4] == "on"){
    Serial.print("blue value: ");
    blue = dataObj[5];
    Serial.println(blue);
    }
   else{
    blue = 0; // turn it off
    }   
    
   if(dataObj[6] == "on"){
    Serial.print("servo value: ");
    servoAngle = dataObj[7];
    Serial.println(servoAngle);
    }
   else{
    servoAngle = 0; // turn it off
    }
   myservo.write(servoAngle); // set servo    
   setNeoColor(red, green, blue); // set Led strip 
 }
