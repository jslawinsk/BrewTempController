<<<<<<< HEAD


void relayControl()
{
  //
  //  Relay Logic
  //
  float curRepTemp = getDisplayTemperatureNum( tempC );
  float TargetRepTemp = getDisplayTemperatureNum( targetTemp );
  if ( curRepTemp + deviation < TargetRepTemp ) {
    //
    //  Too Cold
    //  Turn On Heat, Turn OffCool
    //
    digitalWrite(RelayHeat, LOW); //Turn on relay
    digitalWrite(RelayCool, HIGH); //Turn off relay
    if ( RelayStatus != RELAY_HEAT ) {
      Serial.println( "Heat On" );
      RelayStatus = RELAY_HEAT;
      if( currentDisplayMode == DISPLAYMODE_MAIN ){
        lcd.setCursor ( 3, 1 );            // go to the 2nd row
        lcd.print( "On " ); 
      }
    }
  }
  else if ( curRepTemp - deviation > TargetRepTemp ) {
    digitalWrite(RelayHeat, HIGH); //Turn off relay
    digitalWrite(RelayCool, LOW); //Turn on relay
    if ( RelayStatus != RELAY_COOL ) {
      Serial.println( "Cooling On" );
      RelayStatus = RELAY_COOL;
      if( currentDisplayMode == DISPLAYMODE_MAIN ){
        lcd.setCursor ( 13, 1 );            // go to the 2nd row
        lcd.print( "On " ); 
      }
    }
  }
  else {
    digitalWrite(RelayHeat, HIGH); //Turn off relay
    digitalWrite(RelayCool, HIGH); //Turn off relay
    if ( RelayStatus != RELAY_NONE ) {
      Serial.println( "Heat and cooling off" );
      RelayStatus = RELAY_NONE;
      if( currentDisplayMode == DISPLAYMODE_MAIN ){
        lcd.setCursor ( 3, 1 );            // go to the 2nd row
        lcd.print( "Off" ); 
        lcd.setCursor ( 13, 1 );            // go to the 2nd row
        lcd.print( "Off" ); 
      }
    }
  }

}
=======


void relayControl()
{
  //
  //  Relay Logic
  //
  float curRepTemp = getDisplayTemperatureNum( tempC );
  float TargetRepTemp = getDisplayTemperatureNum( targetTemp );
  if ( curRepTemp + deviation < TargetRepTemp ) {
    //
    //  Too Cold
    //  Turn On Heat, Turn OffCool
    //
    digitalWrite(RelayHeat, LOW); //Turn on relay
    digitalWrite(RelayCool, HIGH); //Turn off relay
    if ( RelayStatus != RELAY_HEAT ) {
      Serial.println( "Heat On" );
      RelayStatus = RELAY_HEAT;
      if( currentDisplayMode == DISPLAYMODE_MAIN ){
        lcd.setCursor ( 3, 1 );            // go to the 2nd row
        lcd.print( "On " ); 
      }
    }
  }
  else if ( curRepTemp - deviation > TargetRepTemp ) {
    digitalWrite(RelayHeat, HIGH); //Turn off relay
    digitalWrite(RelayCool, LOW); //Turn on relay
    if ( RelayStatus != RELAY_COOL ) {
      Serial.println( "Cooling On" );
      RelayStatus = RELAY_COOL;
      if( currentDisplayMode == DISPLAYMODE_MAIN ){
        lcd.setCursor ( 13, 1 );            // go to the 2nd row
        lcd.print( "On " ); 
      }
    }
  }
  else {
    digitalWrite(RelayHeat, HIGH); //Turn off relay
    digitalWrite(RelayCool, HIGH); //Turn off relay
    if ( RelayStatus != RELAY_NONE ) {
      Serial.println( "Heat and cooling off" );
      RelayStatus = RELAY_NONE;
      if( currentDisplayMode == DISPLAYMODE_MAIN ){
        lcd.setCursor ( 3, 1 );            // go to the 2nd row
        lcd.print( "Off" ); 
        lcd.setCursor ( 13, 1 );            // go to the 2nd row
        lcd.print( "Off" ); 
      }
    }
  }

}
>>>>>>> branch 'master' of https://github.com/jslawinsk/BrewTempController.git
