#ifdef BLUETOOTH_ENABLED  

#include <SoftwareSerial.h> 


int bluetoothTx = 4;  //2;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 5;  //3;  // RX-I pin of bluetooth mate, Arduino D3
unsigned long bluetoothTimer;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void bluetoothSetup()
{
  //
  // Bluetooth Setup
  //
  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);  // Start bluetooth serial at 9600
  bluetoothTimer = millis();  
}

void bluetoothInterface()
{
  if(bluetooth.available())  // If the bluetooth sent any characters
  {
    static char buffer[80];


    if (readline(bluetooth.read(), buffer, 80) > 0) 
    {
      #ifdef DEBUG        
        Serial.println( "BT Command: " );
        Serial.println( buffer );   
        Serial.println( "\r\n" );
      #endif
      displayMessage( buffer ); 
      String strBuffer = String( buffer );
      if( strBuffer.startsWith( "COMMAND:" ) ){
        String strCommand = strBuffer.substring( 8 );
        #ifdef DEBUG
          Serial.println( "Command: " + strCommand );
        #endif
        if( strCommand.startsWith( "TARGET:" ) ){
          String strTarget = strCommand.substring( 7 );
          #ifdef DEBUG
            Serial.println( "Target: " + strTarget );
          #endif
          targetTemp = strTarget.toFloat();
          controlCommandReceived = true;
          setTempratureTemplate();
        }
        else if( strCommand.startsWith( "DEVIATION:" ) ){
          String strDeviation = strCommand.substring( 10 );
          #ifdef DEBUG
            Serial.println( "Deviation: " + strDeviation );
          #endif
          deviation = strDeviation.toFloat();
          controlCommandReceived = true;
          setTempratureTemplate();
        }
        else if( strCommand.startsWith( "CONTROL:" ) ){
          String strControl = strCommand.substring( 8 );
          if( strControl == "AUTO" ){
            controlMode = CONTROL_AUTO;
            controlCommandReceived = true;
          }
          else if( strControl == "HEAT_ON" ){
            controlMode = CONTROL_HEAT;
            controlCommandReceived = true;
          }
          else if( strControl == "COOL_ON" ){
            controlMode = CONTROL_COOL;
            controlCommandReceived = true;
          }
          #ifdef DEBUG
            Serial.println( "Control: " + strControl );
          #endif
          /*
          if( currentDisplayMode == DISPLAYMODE_MAIN ){
            setTempratureTemplate();
          } 
          */         
        }
      }
      memset(buffer, 0, sizeof(buffer));
    }

    // Send any characters the bluetooth prints to the serial monitor
    // Serial.print( (char)bluetooth.read() );  
  }
  unsigned long currentTime = millis();
  if( bluetoothTimer + 3000 < currentTime ){
    // Send any characters the Serial monitor prints to the bluetooth
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
    String dataPacket = "{\"temperature\":" + String( currentTemprature );
    dataPacket = dataPacket + ", \"target\":" + targetTemp;
    dataPacket = dataPacket + ", \"heat\":\"" + heat + "\"";
    dataPacket = dataPacket + ", \"cool\":\"" + cool + "\"";
    dataPacket = dataPacket + ", \"control\":\"" + control + "\"";
    dataPacket = dataPacket + "}";
    bluetooth.print( dataPacket );
    bluetooth.print( "\n" );
    bluetoothTimer = millis();
  }    
}

int readline(int readch, char *buffer, int len)
{
  static int pos = 0;
  int rpos;

  if (readch > 0) {
    switch (readch) {
      case '\n': // Ignore new-lines
        break;
      case '\r': // Return on CR
        rpos = pos;
        pos = 0;  // Reset position index ready for next time
        return rpos;
      default:
        if (pos < len-1) {
          buffer[pos++] = readch;
          buffer[pos] = 0;
        }
    }
  }
  // No end of line has been found, so return -1.
  return -1;
}

#endif
