


void bluetoothInterface()
{
  if(bluetooth.available())  // If the bluetooth sent any characters
  {
    static char buffer[80];


    if (readline(bluetooth.read(), buffer, 80) > 0) 
    {
      Serial.println( "BT Command: " );
      Serial.println( buffer );   
      Serial.println( "\r\n" );
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
