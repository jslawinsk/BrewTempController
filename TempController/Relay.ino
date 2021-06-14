
unsigned long relayTimer;

void relaySetup()
{
  relayTimer = millis();
}

void relayControl()
{
  //
  //  Relay Logic
  //
  unsigned long currentTime = millis();
  //
  //  Engage relay change only at 1 minute intevals
  //  To stop trigger change when temp is on a margin 
  //
  if( relayTimer + 60000 < currentTime || controlCommandReceived ){  
    controlCommandReceived = false;
    float curRepTemp = getDisplayTemperatureNum( currentTemprature );
    float TargetRepTemp = getDisplayTemperatureNum( targetTemp );
    if ( curRepTemp + deviation < TargetRepTemp || controlMode == CONTROL_HEAT ) {
      //
      //  Too Cold
      //  Turn On Heat, Turn OffCool
      //
      #ifdef RELAY_ENABLED
      digitalWrite(RelayHeat, LOW); //Turn on relay
      digitalWrite(RelayCool, HIGH); //Turn off relay
      #endif
      if ( RelayStatus != RELAY_HEAT ) {
        #ifdef DEBUG
          Serial.println( "Heat On" );
        #endif
        RelayStatus = RELAY_HEAT;
        if( currentDisplayMode == DISPLAYMODE_MAIN ){
          #ifdef LCD_20X4
            lcdsetCursor ( 5, 1 );            // go to the 2nd row
            lcdprint( "On " ); 
            lcdsetCursor ( 17, 1 );            // go to the 2nd row
            lcdprint( "Off" ); 
          #else        
            lcdsetCursor ( 3, 1 );            // go to the 2nd row
            lcdprint( "On " ); 
            lcdsetCursor ( 13, 1 );            // go to the 2nd row
            lcdprint( "Off" ); 
          #endif
        }
      }
    }
    else if ( curRepTemp - deviation > TargetRepTemp || controlMode == CONTROL_COOL ) {
      #ifdef RELAY_ENABLED
      digitalWrite(RelayHeat, HIGH); //Turn off relay
      digitalWrite(RelayCool, LOW); //Turn on relay
      #endif
      if ( RelayStatus != RELAY_COOL ) {
        #ifdef DEBUG
          Serial.println( "Cooling On" );
        #endif
        RelayStatus = RELAY_COOL;
        if( currentDisplayMode == DISPLAYMODE_MAIN ){
            #ifdef LCD_20X4
              lcdsetCursor ( 5, 1 );            // go to the 2nd row
              lcdprint( "Off" ); 
              lcdsetCursor ( 17, 1 );            // go to the 2nd row
              lcdprint( "On " ); 
            #else
              lcdsetCursor ( 3, 1 );            // go to the 2nd row
              lcdprint( "Off" ); 
              lcdsetCursor ( 13, 1 );            // go to the 2nd row
              lcdprint( "On " ); 
            #endif
        }
      }
    }
    else {
      #ifdef RELAY_ENABLED
      digitalWrite(RelayHeat, HIGH); //Turn off relay
      digitalWrite(RelayCool, HIGH); //Turn off relay
      #endif
      if ( RelayStatus != RELAY_NONE ) {
        #ifdef DEBUG
          Serial.println( "Heat and cooling off" );
        #endif
        RelayStatus = RELAY_NONE;
        if( currentDisplayMode == DISPLAYMODE_MAIN ){
          #ifdef LCD_20X4
            lcdsetCursor ( 5, 1 );            // go to the 2nd row
            lcdprint( "Off" ); 
            lcdsetCursor ( 17, 1 );            // go to the 2nd row
            lcdprint( "Off" ); 
          #else
            lcdsetCursor ( 3, 1 );            // go to the 2nd row
            lcdprint( "Off" ); 
            lcdsetCursor ( 13, 1 );            // go to the 2nd row
            lcdprint( "Off" ); 
         #endif
        }
      }
    }
    relayTimer = millis();
  }
}

void displayRelayTemplate( int row )
{
  lcdsetCursor ( 0,row );
  #ifdef LCD_20X4
    lcdprint("Heat: ");
    lcdsetCursor ( 5, row );
  #else
    lcdprint("HT: ");
    lcdsetCursor ( 3, row );
  #endif
  if( RelayStatus == RELAY_HEAT ){
    lcdprint( "On " ); 
  }
  else{
    lcdprint( "Off" ); 
  }      
  #ifdef LCD_20X4
    lcdsetCursor ( 12, row );
    lcdprint("Cool:");
    lcdsetCursor ( 17, row );
  #else
    lcdsetCursor ( 10, row );
    lcdprint("CL:");
    lcdsetCursor ( 13, row );
  #endif
  if( RelayStatus == RELAY_COOL ){
    lcdprint( "On " ); 
  }
  else{
    lcdprint( "Off" ); 
  }  
}
