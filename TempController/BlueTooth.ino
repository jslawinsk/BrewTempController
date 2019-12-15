<<<<<<< HEAD

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
      memset(buffer, 0, sizeof(buffer));
    }

    // Send any characters the bluetooth prints to the serial monitor
    // Serial.print( (char)bluetooth.read() );  
  }
  unsigned long currentTime = millis();
  if( bluetoothTimer + 3000 < currentTime ){
    // Send any characters the Serial monitor prints to the bluetooth
    bluetooth.print( getDisplayTemperature( tempC ) );
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
=======

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
      memset(buffer, 0, sizeof(buffer));
    }

    // Send any characters the bluetooth prints to the serial monitor
    // Serial.print( (char)bluetooth.read() );  
  }
  unsigned long currentTime = millis();
  if( bluetoothTimer + 3000 < currentTime ){
    // Send any characters the Serial monitor prints to the bluetooth
    bluetooth.print( getDisplayTemperature( tempC ) );
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
>>>>>>> branch 'master' of https://github.com/jslawinsk/BrewTempController.git
