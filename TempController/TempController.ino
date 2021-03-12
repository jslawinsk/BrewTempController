

#include <OneWire.h>
#include <DallasTemperature.h>
//LCD includes
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h> // include i/o class header

// #define DEBUG 1

float currentTemprature;  // Current Temprature store in farenheight

int RelayHeat = 6;
int RelayCool = 7;

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

// LiquidCrystal Display
hd44780_I2Cexp lcd; // declare lcd object: auto locate & config display for hd44780 chip

//temp sensor defines

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 9
#define TEMPERATURE_PRECISION 9 // Lower resolution

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

int numberOfDevices; // Number of temperature devices found

float min = 1024;
float max = -100;

DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address

void setup(void)
{
  displayLogo();
  
  //setup temp sensor
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo.");

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
  Serial.println();
  Serial.print( "Setting up Rotary Encoder" );
  //rotary encoder setup

  rotarySetup();
  Serial.begin (9600);

  //
  //  Releay Switch Setup
  //
  pinMode(RelayHeat, OUTPUT); //Set Pin12 as output 
  pinMode(RelayCool, OUTPUT); //Set Pin12 as output 
  digitalWrite(RelayHeat, HIGH); //Turn off relay 
  digitalWrite(RelayCool, HIGH); //Turn off relay 
  RelayStatus = RELAY_NONE;

  bluetoothSetup();
  relaySetup();
  
  screenTime = millis();
  tempratureTimer = millis();

  delay( 4000 );
  lcd.clear();
  setTempratureTemplate();  
}


void loop(void)
{ 
  if(screenMode != SCREENMODE_SETUP ){
    temperatureInterface();
    relayControl();
    bluetoothInterface();
  }
  rotary();
}



// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
    if(screenMode == SCREENMODE_DISPLAYTEMP ){
      lcd.setCursor (5,0);         // go to col 16 of the last row
      lcd.print( getDisplayTemperature( currentTemprature ) );           // update the display with a new number
  
      //
      //  Screensaver code
      //
      unsigned long delayTime = 60000 * 15;
      unsigned long currentTime = millis();
      if( screenTime + delayTime < currentTime ){
        screenMode = SCREENMODE_SCREEN_SAVER;
        lcd.setBacklight(LOW);
      }
    }
}

String getDisplayTemperature( float tempF ){
    String strTemp = "";
    if(unit == UNIT_FARENHEIGHT)
    {
      strTemp = String(tempF);
    }
    else{
      float tempF = DallasTemperature::toCelsius( tempF );
      strTemp = String(tempF);
    }
    strTemp = strTemp.substring( 0, 4 );
    return strTemp;
}

float getDisplayTemperatureNum( float tempF ){
    float nTemp = tempF;
    if(unit == UNIT_CELSIUS)
    {
      nTemp = DallasTemperature::toCelsius(tempF);
      //nTemp = DallasTemperature::toFahrenheit(tempC);
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
        if(currentTemprature >= max)
        {
          max = currentTemprature;
          if( currentDisplayMode == DISPLAYMODE_MINMAX ){
            lcd.setCursor ( 11, 1 );            // go to the 2nd row
            lcd.print( getDisplayTemperature( max ) ); // pad string with spaces for centering
          }
        }
        if(currentTemprature <= min)
        {
          min = currentTemprature;
          if( currentDisplayMode == DISPLAYMODE_MINMAX ){
            lcd.setCursor ( 3, 1 );            // go to the 2nd row
            lcd.print( getDisplayTemperature( min ) ); // pad string with spaces for centering
          }
        }
    }
    // It responds almost immediately. Let's print out the data
    printTemperature(tempDeviceAddress); // Use a simple function to print out the data
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
