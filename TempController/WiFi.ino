#ifdef WIFI_ENABLED

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
// #include <WiFi.h>

//
//  TODO: Update The follwoing lines to specify your WiFi credentials
//
const char* ssid = "<Update SSID var here>";
const char* password = "<update NW Password here>";
//
//  TODO: Update the following to specify your desired IP
//
IPAddress ip( 10, 0, 0, 22 );
//
//  TODO: Update the following to specify your network gateway IP
//
IPAddress gateway( 10, 0, 0, 1 );
//
//  TODO: Web login user names change accordingly
//
const char* www_username = "admin";
const char* www_password = "admin";

ESP8266WebServer server(80);

const int RESPONSE_HTML = 0;
const int RESPONSE_JSON = 1;

unsigned long previous_time = 0;
unsigned long resetDelay = 60000; // 60 second delay

void wifiSetup() {
  Serial.begin(9600);
  delay(10);

  wifiConnect();
 
  // Start the server
  server.begin();
  #ifdef DEBUG        
    Serial.println("Server started");
    // Print the IP address
    Serial.print("Use this URL : ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
  #endif
  
  server.on("/", [](){
    sendResponsePage( "", RESPONSE_HTML );
  });

  
  server.on("/tempdata", HTTP_OPTIONS, []() {
    #ifdef DEBUG
      Serial.println( "OPTIONS Request: " );
    #endif
    
    server.sendHeader("Access-Control-Allow-Credentials", "true");
    server.sendHeader("Access-Control-Allow-Headers", "Access-Control-Allow-Origin, Authorization, Access-Control-Allow-Credentials, Content-Type, Depth, User-Agent, X-File-Size, X-Requested-With, If-Modified-Since, X-File-Name, Cache-Control");
    server.sendHeader("access-control-allow-methods", "GET, OPTIONS, POST");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(204);
  });

  server.on("/tempdata", HTTP_GET, [](){
    #ifdef DEBUG
      Serial.println( "HTTP Request: " );
    #endif

    if (!server.authenticate(www_username, www_password)) {
      return server.requestAuthentication();
    }

    String querycommand = getQueryParam( "command", "none" );
    if( querycommand == "contolcool" ){
        controlMode = CONTROL_COOL;
        controlCommandReceived = true;
    }
    else if( querycommand == "contolheat" ){
        controlMode = CONTROL_HEAT;
        controlCommandReceived = true;
    }
    else if( querycommand == "contolauto" ){
        controlMode = CONTROL_AUTO;
        controlCommandReceived = true;
    }
    else if( querycommand == "settarget" ){
        String strTarget = getQueryParam( "target", "" );
        #ifdef DEBUG
          Serial.println( "Target: " + strTarget );
        #endif
        if( strTarget != "" ){
          targetTemp = strTarget.toFloat();
          controlCommandReceived = true;
          setTempratureTemplate();
        }   
    }
    else if( querycommand == "setdeviation" ){
        String strDeviation = getQueryParam( "deviation", "" );
        #ifdef DEBUG
          Serial.println( "Deviation: " + strDeviation );
        #endif
        if( strDeviation != "" ){
          deviation = strDeviation.toFloat();
          controlCommandReceived = true;
          setTempratureTemplate();
        }   
    }
    else if( querycommand == "setcalibration" ){
        String strCalibration = getQueryParam( "calibration", "" );
        #ifdef DEBUG
          Serial.println( "Calibration: " + strCalibration );
        #endif
        if( strCalibration != "" ){
          calibration = strCalibration.toFloat();
          controlCommandReceived = true;
          setTempratureTemplate();
        }   
    }
    else if( querycommand == "setunits" ){
        String strType = getQueryParam( "type", "" );
        #ifdef DEBUG
          Serial.println( "Units: " + strType );
        #endif
        if( strType == "C" ){
          unit =  UNIT_CELSIUS;
          setTempratureTemplate();
        }   
        else if( strType == "F" ){
          unit =  UNIT_FARENHEIGHT;
          setTempratureTemplate();
        }   
    }
    int responseType = RESPONSE_HTML;
    String respType = getQueryParam( "responseFormat", "HTML" );
    if( respType == "JSON" ){
      responseType = RESPONSE_JSON;
    }

    sendResponsePage( querycommand, responseType );
    delay(1000);
  });
  server.begin();
  #ifdef DEBUG        
    Serial.println("Web server started!"); 
  #endif
}
 
void wifiLoop() {
  unsigned long current_time = millis();
  if( WiFi.status() != WL_CONNECTED ){
    #ifdef DEBUG        
      Serial.println("WIFI disconnected reconnecting");
    #endif
    wifiConnect();
    previous_time = millis();
  }  
  else if( current_time - previous_time >= resetDelay * 30 ){
    #ifdef DEBUG        
      Serial.println("WIFI connection dropped");
    #endif
    WiFi.disconnect();
    previous_time = millis();
  }  
  server.handleClient();
}

void wifiConnect() {
  // Connect to WiFi network
  #ifdef DEBUG        
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
  #endif
  IPAddress subnet( 255, 255, 255, 0 );
  WiFi.config( ip, gateway, subnet );
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    #ifdef DEBUG        
      Serial.print(".");
    #endif
  }
  #ifdef DEBUG        
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  #endif  
}

String getQueryParam( String param, String defaultValue )  { 

  String value = defaultValue;

  if( server.arg( param ) != ""){ 
    value = server.arg( param );     //Gets the value of the query parameter
  }
  return value;
}

void sendResponsePage( String qCommand, int responseType ) {
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

    String unitType = "F";
    if( unit == UNIT_CELSIUS ){
      unitType = "C";
    }

    server.sendHeader("Access-Control-Allow-Origin", "*");

    if( responseType == RESPONSE_JSON ){
      String page = "{\"temperature\":" + String( currentTemprature );
      page = page + ", \"target\":" + targetTemp;
      page = page + ", \"heat\":\"" + heat + "\"";
      page = page + ", \"cool\":\"" + cool + "\"";
      page = page + ", \"control\":\"" + control + "\"";
      page = page + ", \"deviation\":" + deviation;
      page = page + ", \"calibration\":" + calibration;
      page = page + ", \"units\":\"" + unitType + "\"";
      page = page + "}";
      server.send(200, "application/json", page);
    }
    else{
      String page = "<h1>Temperature Sensor Data</h1><p>";
      page = page + "Temperature: " + getDisplayTemperatureBasic( currentTemprature ) + " " + unitType + "<br>";  
      page = page + "Target: " + getDisplayTemperatureBasic( targetTemp ) + " " + unitType + "<br>";
      page = page + "Heat: " + heat + "<br>";
      page = page + "Cool: " + cool + "<br>";
      page = page + "Control: " + control + "<br>";
      page = page + "Deviation: " + deviation + "<br>";
      page = page + "Calibration: " + calibration + "<br>";
      page = page + "Command: " + qCommand + "<br>";
      page = page +"<a href=\"tempdata\"><button>Temperature Data</button></a>&nbsp;<a href=\"tempdata?responseFormat=JSON\"><button>Temperature JSON</button></a></p>";
      server.send(200, "text/html", page);
    }
}
#endif
