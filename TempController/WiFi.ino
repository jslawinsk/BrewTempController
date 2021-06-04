#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//
//  TODO: Update The follwoing lines to specify your WiFi credentials
//
const char* ssid = "<Update SSID var here>";
const char* password = "<update NW Password here>";
//
//  TODO: Update the following to specify you desired IP
//
IPAddress ip( 10, 0, 0, 22 );
//
//  TODO: Update the following to specify you network gateway IP
//
IPAddress gateway( 10, 0, 0, 1 );

const char* www_username = "admin";
const char* www_password = "admin";

ESP8266WebServer server(80);

void wifiSetup() {
//  Serial.begin(115200);
  Serial.begin(9600);
  delay(10);
 
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  IPAddress subnet( 255, 255, 255, 0 );
  WiFi.config( ip, gateway, subnet );
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  server.on("/", [](){
    String page = "<h1>Temperature Sensor Data</h1><p>";
    page = page +"<a href=\"tempdata\"><button>Temperature Data</button></a>&nbsp;<a href=\"tempjson\"><button>Temperature JSON</button></a></p>";
    server.send(200, "text/html", page);
  });
  server.on("/tempdata", [](){

    if (!server.authenticate(www_username, www_password)) {
      return server.requestAuthentication();
    }
    String cool = "OFF";
    String heat = "OFF";
    String control = "Auto";
    if( RelayStatus == RELAY_HEAT ){
      heat = "ON";
    }
    else if( RelayStatus == RELAY_COOL ){
      cool = "ON";
    }
    if( controlMode == CONTROL_HEAT ){
      control = "Heat";
    }
    else if( controlMode == CONTROL_COOL ){
      control = "Cool";
    }
    String page = "<h1>Temperature Sensor Data</h1><p>";
    page = page + "Temperature: " + getDisplayTemperature( currentTemprature ) + "<br>";  
    page = page + "Target: " + getDisplayTemperature( targetTemp ) + "<br>";
    page = page + "Heat: " + heat + "<br>";
    page = page + "Cool: " + cool + "<br>";
    page = page + "Control: " + control + "<br>";
    page = page +"<a href=\"tempdata\"><button>Temperature Data</button></a>&nbsp;<a href=\"tempjson\"><button>Temperature JSON</button></a></p>";
    server.send(200, "text/html", page);
    delay(1000);
  });
  server.on("/tempjson", [](){
    if (!server.authenticate(www_username, www_password)) {
      return server.requestAuthentication();
    }
    String cool = "OFF";
    String heat = "OFF";
    String control = "Auto";
    if( RelayStatus == RELAY_HEAT ){
      heat = "ON";
    }
    else if( RelayStatus == RELAY_COOL ){
      cool = "ON";
    }
    if( controlMode == CONTROL_HEAT ){
      control = "Heat";
    }
    else if( controlMode == CONTROL_COOL ){
      control = "Cool";
    }    

    String page = "{\"temperature\":" + String( currentTemprature );
      page = page + ", \"target\":" + targetTemp;
      page = page + ", \"heat\":\"" + heat + "\"";
      page = page + ", \"cool\":\"" + cool + "\"";
      page = page + ", \"control\":\"" + control + "\"";
      page = page + "}";
    server.send(200, "application/json", page);
    delay(1000); 
  });
  server.begin();
  Serial.println("Web server started!"); 
}
 
void wifiLoop() {

  server.handleClient();

}
