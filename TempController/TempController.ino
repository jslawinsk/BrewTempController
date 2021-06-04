

#include <OneWire.h>
#include <DallasTemperature.h>

// #define DEBUG 1
//
// The following are defined for components which are installed
//
// #define BLUETOOTH_ENABLED 1
#define ROTARY_ENABLED 1
#define RELAY_ENABLED 1
// #define WIFI_ENABLED 1
//
// Define the following if using a Lolin D1 Mini, comment the following line for Arduino
//
// #define D1_MINI 1

float currentTemprature;  // Current Temprature store in farenheight

#ifdef D1_MINI
  int RelayHeat = D7;
  int RelayCool = D8;
#else
  int RelayHeat = 6;
  int RelayCool = 7;
#endif

const int RELAY_NONE = 0;
const int RELAY_HEAT = 1;
const int RELAY_COOL = 2;
const int RELAY_OVERRIDE_HEAT = 3;
const int RELAY_OVERRIDE_COOL = 4;
int RelayStatus = RELAY_NONE;

const int CONTROL_AUTO = 0;
const int CONTROL_HEAT = 1;
const int CONTROL_COOL = 2;
int controlMode = CONTROL_AUTO;

boolean controlCommandReceived = false;

const int SCREENMODE_DISPLAYTEMP = 1;
const int SCREENMODE_SETUP = 2;
const int SCREENMODE_SCREEN_SAVER = 3;
int screenMode = SCREENMODE_DISPLAYTEMP;

const int DISPLAYMODE_MAIN    = 0;
const int DISPLAYMODE_TARGET  = 1;
const int DISPLAYMODE_MINMAX  = 2;
int currentDisplayMode = DISPLAYMODE_MAIN;

unsigned long screenTime;

const int UNIT_FARENHEIGHT = 1;
const int UNIT_CELSIUS = 2;
int unit =  UNIT_FARENHEIGHT;

float deviation = 1.0;
int targetTemp = 70;
unsigned long tempratureTimer;

//temp sensor defines

// Data wire is plugged into port 2 on the Arduino

#ifdef D1_MINI
  #define ONE_WIRE_BUS D4
#else
  #define ONE_WIRE_BUS 9
#endif
#define TEMPERATURE_PRECISION 9 // Lower resolution
#define TEMPERATURE_MINIMUM 0
#define TEMPERATURE_MAXIMUM 275

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

int numberOfDevices; // Number of temperature devices found

float _minTemp = 1024.0;
float _maxTemp = -100.0;

DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address

void setup(void)
{
  displayLogo();
  
  //setup temp sensor
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Setup");

  // Start up the library
  sensors.begin();
  
  // Grab a count of devices on the wire
  numberOfDevices = sensors.getDeviceCount();
  
  // locate devices on the bus
  Serial.print("Locating devices...");
  
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  
  // Loop through each device, print out address
  for(int i=0;i<numberOfDevices; i++)
  {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i))
    {
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      printAddress(tempDeviceAddress);
      Serial.println();
    
      Serial.println("Setting resolution to ");
      Serial.println();
      Serial.println(TEMPERATURE_PRECISION, DEC);
      Serial.println();
      // set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
      sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
      delay(500);
      Serial.print("Resolution actually set to: ");
      Serial.print(sensors.getResolution(tempDeviceAddress), DEC); 
      delay(500);
      Serial.println();
      //break;
    }else{
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
    }    
  }

  Serial.begin(9600);
  #ifdef ROTARY_ENABLED
  Serial.println();
  Serial.print( "Setting up Rotary Encoder" );
  //rotary encoder setup

  rotarySetup();
  #endif
  Serial.begin (9600);

  //
  //  Releay Switch Setup
  //
  #ifdef RELAY_ENABLED
  pinMode(RelayHeat, OUTPUT); //Set Pin12 as output 
  pinMode(RelayCool, OUTPUT); //Set Pin12 as output 
  digitalWrite(RelayHeat, HIGH); //Turn off relay 
  digitalWrite(RelayCool, HIGH); //Turn off relay 
  #endif
  RelayStatus = RELAY_NONE;

  #ifdef BLUETOOTH_ENABLED  
  bluetoothSetup();
  #endif
  
  relaySetup();

  #ifdef WIFI_ENABLED
  wifiSetup();
  #endif
  
  screenTime = millis();
  tempratureTimer = millis();

  delay( 4000 );
  lcdclear();
  setTempratureTemplate();  
}


void loop(void)
{ 
  if(screenMode != SCREENMODE_SETUP ){
    temperatureInterface();
    relayControl();
    #ifdef BLUETOOTH_ENABLED
      bluetoothInterface();
    #endif
  }
  #ifdef ROTARY_ENABLED
  rotary();
  #endif

  #ifdef WIFI_ENABLED
  wifiLoop();
  #endif

}



// function to print the temperature for a device
void printTemperature( )
{
    if(screenMode == SCREENMODE_DISPLAYTEMP ){
      lcdsetCursor (5,0);         // go to col 16 of the last row
      lcdprint( getDisplayTemperature( currentTemprature ) );           // update the display with a new number
  
      //
      //  Screensaver code
      //
      unsigned long delayTime = 60000 * 15;
      unsigned long currentTime = millis();
      if( screenTime + delayTime < currentTime ){
        screenMode = SCREENMODE_SCREEN_SAVER;
        lcdsetBacklightLOW();
      }
    }
}

String getDisplayTemperature( float tempF ){
    String strTemp = "";
    if(unit == UNIT_FARENHEIGHT)
    {
      strTemp = String(tempF);
      strTemp = strTemp.substring( 0, 4 );
      strTemp = strTemp + " \xDF";
      strTemp = strTemp + "F";
    }
    else{
      // float tempF = DallasTemperature::toCelsius( tempF );
      float tempC = ( tempF - 32 ) * 0.5556;;
      strTemp = String( tempC );
      strTemp = strTemp.substring( 0, 4 );
      strTemp = strTemp + " \xDF";
      strTemp = strTemp + "C";
    }
    return strTemp;
}

String getDisplayTemperatureBasic( float tempF ){
    String strTemp = "";
    if(unit == UNIT_FARENHEIGHT)
    {
      strTemp = String(tempF);
    }
    else{
      // float tempF = DallasTemperature::toCelsius( tempF );
      float tempC = ( tempF - 32 ) * 0.5556;;
      strTemp = String( tempC );
    }
    strTemp = strTemp.substring( 0, 4 );
    return strTemp;
}


float getDisplayTemperatureNum( float tempF ){
    float nTemp = tempF;
    if(unit == UNIT_CELSIUS)
    {
      // nTemp = DallasTemperature::toCelsius(tempF);
      nTemp = (tempF-32) * 0.5556;
    }
    return nTemp;
}

//
//  Get Temprature Evry 1 Sec and Display
//
void temperatureInterface()
{
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus

  unsigned long currentTime = millis();
  if( tempratureTimer + 1000 < currentTime ){

    sensors.requestTemperatures(); // Send the command to get temperatures
    // Loop through each device, print out temperature data
    for(int i=0;i<numberOfDevices; i++)
    {
      // Search the wire for address
      if(sensors.getAddress(tempDeviceAddress, i))
      {
        // Output the device ID
        //    Serial.print("Temperature for device: ");
        //    Serial.println(i,DEC);
        //float tempC = sensors.getTempC(tempDeviceAddress);
        //currentTemprature = DallasTemperature::toFahrenheit(tempC);
        currentTemprature = sensors.getTempF(tempDeviceAddress);
        if( currentTemprature >= TEMPERATURE_MINIMUM && currentTemprature <= TEMPERATURE_MAXIMUM )
        {
          if(currentTemprature >= _maxTemp)
          {
            _maxTemp = currentTemprature;
            if( currentDisplayMode == DISPLAYMODE_MINMAX ){
              lcdsetCursor ( 11, 1 );            // go to the 2nd row
              lcdprint( getDisplayTemperatureBasic( _maxTemp ) ); // pad string with spaces for centering
            }
          }
          if(currentTemprature <= _minTemp)
          {
            _minTemp = currentTemprature;
            if( currentDisplayMode == DISPLAYMODE_MINMAX ){
              lcdsetCursor ( 3, 1 );            // go to the 2nd row
              lcdprint( getDisplayTemperatureBasic( _minTemp ) ); // pad string with spaces for centering
            }
          }
        }
    }
    // It responds almost immediately. Let's print out the data
    printTemperature( ); // Use a simple function to print out the data
   } 
    //else ghost device! Check your power requirements and cabling
  }
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
