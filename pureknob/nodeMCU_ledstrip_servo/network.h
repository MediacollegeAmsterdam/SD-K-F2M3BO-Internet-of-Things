/*
 * Wifi Network
 * insert here your own network Wifi Credentials 
*/

const char* ssid     = "Medialab"; // wifi lan
const char* password = "Mediacollege"; // wifi lan

String server  = "http://schw.hosts1.ma-cloud.nl"; // deployment server
String path          = "/ma3d3/control.json"; // path to file with control data 
String path2         = "/ma3d3/jsonWrite.php?src=esp&data="; // path to file
String httpResponse;// response from webserver


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
