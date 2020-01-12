
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
      digitalWrite(RelayHeat, LOW); //Turn on relay
      digitalWrite(RelayCool, HIGH); //Turn off relay
      if ( RelayStatus != RELAY_HEAT ) {
        #ifdef DEBUG
          Serial.println( "Heat On" );
        #endif
        RelayStatus = RELAY_HEAT;
        if( currentDisplayMode == DISPLAYMODE_MAIN ){
          lcd.setCursor ( 3, 1 );            // go to the 2nd row
          lcd.print( "On " ); 
          lcd.setCursor ( 13, 1 );            // go to the 2nd row
          lcd.print( "Off" ); 
        }
      }
    }
    else if ( curRepTemp - deviation > TargetRepTemp || controlMode == CONTROL_COOL ) {
      digitalWrite(RelayHeat, HIGH); //Turn off relay
      digitalWrite(RelayCool, LOW); //Turn on relay
      if ( RelayStatus != RELAY_COOL ) {
        #ifdef DEBUG
          Serial.println( "Cooling On" );
        #endif
        RelayStatus = RELAY_COOL;
        if( currentDisplayMode == DISPLAYMODE_MAIN ){
          lcd.setCursor ( 3, 1 );            // go to the 2nd row
          lcd.print( "Off" ); 
          lcd.setCursor ( 13, 1 );            // go to the 2nd row
          lcd.print( "On " ); 
        }
      }
    }
    else {
      digitalWrite(RelayHeat, HIGH); //Turn off relay
      digitalWrite(RelayCool, HIGH); //Turn off relay
      if ( RelayStatus != RELAY_NONE ) {
        #ifdef DEBUG
          Serial.println( "Heat and cooling off" );
        #endif
        RelayStatus = RELAY_NONE;
        if( currentDisplayMode == DISPLAYMODE_MAIN ){
          lcd.setCursor ( 3, 1 );            // go to the 2nd row
          lcd.print( "Off" ); 
          lcd.setCursor ( 13, 1 );            // go to the 2nd row
          lcd.print( "Off" ); 
        }
      }
    }
    relayTimer = millis();
  }
}
