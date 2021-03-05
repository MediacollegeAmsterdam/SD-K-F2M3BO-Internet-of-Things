/*
 * Duurzaam Huis Led Control
 * Temperature + Humidity Sensor DHT11
 * MediaCollege Amsterdam feb 2021
*/


#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

/* 
 *  temperature + humidity sensor type DHT11
 */
#include "DHT.h"
#define DHTTYPE DHT11   // DHT 11 sensor
uint8_t DHTPin = D7; // DHT Sensor data output
DHT dht(DHTPin, DHTTYPE);  // Initialize DHT sensor.    
float Temperature;
float Humidity;
float HeatIndex;


/*
 * Wifi Network
 * insert here your own network Wifi Credentials 
*/

const char* ssid     = "Medialab"; // wifi lan
const char* password = "Mediacollege"; // wifi lan

String server  = "http://schw.hosts1.ma-cloud.nl"; // deployment server
String path          = "/ma2d2/control.json"; // path to file with control data 
String path2         = "/ma2d2/jsonWrite.php?src=esp&data="; // path to file
String httpResponse;// response from webserver


//LED pins
uint8_t Led0 = LED_BUILTIN; //onboard Led
uint8_t Led1 = D3; //LED indicator 
uint8_t Led2 = D5; //LED indicator
uint8_t Led3 = D6; //LED indicator

// Slow it down
const int wait = 10000;  // msec between data requests


void setup () {
  Serial.begin(115200);
  dht.begin(); // start DHT11 sensor
}
 
void loop() {  
  if (WiFi.status() != WL_CONNECTED) wifiConnect();// reconnect Wifi if necessary 
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
     httpRequest(); // get data from server
     extractJson(); // execute data from server

     readDHT11();  // get sensor readings from DHT11
     httpRequest2(); // write data to server
  }
 
  //Serial.println(httpResponse);  //Print the response payload
  delay(wait);    //Send a request every XXX seconds
}

void wifiConnect(){
  WiFi.begin(ssid, password); // connect to wifi ssid with password 
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000); // wait 1 sec
      Serial.print("Connecting..");
      }
  //if connected
  Serial.print("\r\nConnected: IP address: "); 
  Serial.println(WiFi.localIP());
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
  if(dataObj[0] == "on"){
    Serial.print("device 1 is aan value: ");
    int range1 = dataObj[1];
    analogWrite(Led1, range1); // show brightness
    Serial.println(range1);
    }
  else{
    analogWrite(Led1,0); // turn it off
    }

  if(dataObj[2] == "on"){
    Serial.print("device 2 is ON value: ");
    int range2 = dataObj[3];
    analogWrite(Led2, range2);
    Serial.println(range2);
    }
   else{
    analogWrite(Led2,0); // turn it off
    }  
 }

 void readDHT11(){
  /* 
   *  read the sensor
   *  sampling rate DHT11 is max. 1Hz 
   *  the DHT11 is known for erroneous readings
   */
    float temperature =  round(dht.readTemperature()*10)/10; // Gets the values of the temperature
    float humidity = round(dht.readHumidity()*10)/10; // Gets the values of the humidity
    float heatindex = round(dht.computeHeatIndex( Temperature, Humidity, false)*10)/10; 
    // Compute heat index in Celsius (isFahrenheit = false)
    if(isnan(temperature) || isnan(humidity) || isnan(heatindex)){
        // sensor error
        Serial.println("DHT11 sensor error");
    }
    else{
        // the DHT11 readings look ok 
        Temperature = temperature;
        Humidity =  humidity ;
        HeatIndex = heatindex;
      }
 } 
