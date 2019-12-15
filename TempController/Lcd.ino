<<<<<<< HEAD

void changeDisplay(){
  currentDisplayMode = currentDisplayMode + 1;
  if( currentDisplayMode > DISPLAYMODE_MINMAX ){
    currentDisplayMode = DISPLAYMODE_MAIN;
  }
  setTempratureTemplate();
}

void setTempratureTemplate(){
    //setup LCD
    lcd.begin(16,2);        // 20 columns by 4 rows on display    
  
    lcd.setBacklight(HIGH); // Turn on backlight, LOW for off
    lcd.clear();
    
    lcd.setCursor ( 0, 0 );            // go to the top left corner
    lcd.print("Temp ");   

    switch( currentDisplayMode ){
      case DISPLAYMODE_MAIN:
        lcd.setCursor ( 0, 1 );            // go to the 2nd row
        lcd.print("HT: "); // pad string with spaces for centering
        lcd.setCursor ( 3, 1 );            // go to the 2nd row
        if( RelayStatus == RELAY_HEAT ){
          lcd.print( "On " ); 
        }
        else{
          lcd.print( "Off" ); 
        }      
        lcd.setCursor ( 10, 1 );            // go to the 3rd row
        lcd.print("CL:"); // pad string with spaces for centering
        lcd.setCursor ( 13, 1 );            // go to the 2nd row
        if( RelayStatus == RELAY_COOL ){
          lcd.print( "On " ); 
        }
        else{
          lcd.print( "Off" ); 
        }      
        break;
      case DISPLAYMODE_TARGET:
        lcd.setCursor ( 0, 1 );            // go to the 2nd row
        lcd.print("Target "); 
        lcd.setCursor ( 7, 1 );            // go to the 2nd row
        lcd.print( getDisplayTemperature( targetTemp ) ); // pad string with spaces for centering
        break;     
      case DISPLAYMODE_MINMAX:
        lcd.setCursor ( 0, 1 );            // go to the 2nd row
        lcd.print("Mn "); // pad string with spaces for centering
        lcd.setCursor ( 3, 1 );            // go to the 2nd row
        lcd.print( getDisplayTemperature( min ) ); // pad string with spaces for centering
      
        lcd.setCursor ( 8, 1 );            // go to the 3rd row
        lcd.print("Mx "); // pad string with spaces for centering
        lcd.setCursor ( 11, 1 );            // go to the 2nd row
        lcd.print( getDisplayTemperature( max ) ); // pad string with spaces for centering
        break;
    }
}

void displayLogo(){
    lcd.begin(16,2);        // 20 columns by 4 rows on display    
  
    lcd.setBacklight(HIGH); // Turn on backlight, LOW for off
    lcd.clear();
    
    lcd.setCursor ( 0, 0 );            // go to the top left corner
    lcd.print("BrewTemp Controll");   
    lcd.setCursor ( 0, 1 );            // go to the 2nd row
    lcd.print("Version Beata 1.0");   
}

void displayMessage( String message ){
  
    lcd.setBacklight(HIGH); // Turn on backlight, LOW for off
    lcd.clear();
    
    lcd.setCursor ( 0, 0 );            // go to the top left corner
    lcd.print( message );   
    delay( 4000 );
    setTempratureTemplate();
}
=======

void changeDisplay(){
  currentDisplayMode = currentDisplayMode + 1;
  if( currentDisplayMode > DISPLAYMODE_MINMAX ){
    currentDisplayMode = DISPLAYMODE_MAIN;
  }
  setTempratureTemplate();
}

void setTempratureTemplate(){
    //setup LCD
    lcd.begin(16,2);        // 20 columns by 4 rows on display    
  
    lcd.setBacklight(HIGH); // Turn on backlight, LOW for off
    lcd.clear();
    
    lcd.setCursor ( 0, 0 );            // go to the top left corner
    lcd.print("Temp ");   

    switch( currentDisplayMode ){
      case DISPLAYMODE_MAIN:
        lcd.setCursor ( 0, 1 );            // go to the 2nd row
        lcd.print("HT: "); // pad string with spaces for centering
        lcd.setCursor ( 3, 1 );            // go to the 2nd row
        if( RelayStatus == RELAY_HEAT ){
          lcd.print( "On " ); 
        }
        else{
          lcd.print( "Off" ); 
        }      
        lcd.setCursor ( 10, 1 );            // go to the 3rd row
        lcd.print("CL:"); // pad string with spaces for centering
        lcd.setCursor ( 13, 1 );            // go to the 2nd row
        if( RelayStatus == RELAY_COOL ){
          lcd.print( "On " ); 
        }
        else{
          lcd.print( "Off" ); 
        }      
        break;
      case DISPLAYMODE_TARGET:
        lcd.setCursor ( 0, 1 );            // go to the 2nd row
        lcd.print("Target "); 
        lcd.setCursor ( 7, 1 );            // go to the 2nd row
        lcd.print( getDisplayTemperature( targetTemp ) ); // pad string with spaces for centering
        break;     
      case DISPLAYMODE_MINMAX:
        lcd.setCursor ( 0, 1 );            // go to the 2nd row
        lcd.print("Mn "); // pad string with spaces for centering
        lcd.setCursor ( 3, 1 );            // go to the 2nd row
        lcd.print( getDisplayTemperature( min ) ); // pad string with spaces for centering
      
        lcd.setCursor ( 8, 1 );            // go to the 3rd row
        lcd.print("Mx "); // pad string with spaces for centering
        lcd.setCursor ( 11, 1 );            // go to the 2nd row
        lcd.print( getDisplayTemperature( max ) ); // pad string with spaces for centering
        break;
    }
}

void displayLogo(){
    lcd.begin(16,2);        // 20 columns by 4 rows on display    
  
    lcd.setBacklight(HIGH); // Turn on backlight, LOW for off
    lcd.clear();
    
    lcd.setCursor ( 0, 0 );            // go to the top left corner
    lcd.print("BrewTemp Controll");   
    lcd.setCursor ( 0, 1 );            // go to the 2nd row
    lcd.print("Version Beata 1.0");   
}

void displayMessage( String message ){
  
    lcd.setBacklight(HIGH); // Turn on backlight, LOW for off
    lcd.clear();
    
    lcd.setCursor ( 0, 0 );            // go to the top left corner
    lcd.print( message );   
    delay( 4000 );
    setTempratureTemplate();
}
>>>>>>> branch 'master' of https://github.com/jslawinsk/BrewTempController.git
