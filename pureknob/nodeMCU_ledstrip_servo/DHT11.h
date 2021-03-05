/* 
 *  temperature + humidity sensor type DHT11
 */
#include "DHT.h"
#define DHTTYPE DHT11   // DHT 11 sensor

DHT dht(DHTPin, DHTTYPE);  // Initialize DHT sensor.    
float Temperature;
float Humidity;
float HeatIndex;
int LightLevel;



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
