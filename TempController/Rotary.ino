

int rotPinA = 10;  // Connected to CLK on KY-040
int rotPinB = 3;  // 11;  // Connected to DT on KY-040
int rotButtonPin = 12; // Should be12
int rotPinALast;  
int rotAVal;
int rotBVal;

int rotationValue = 0;

const int EDIT_START = 0;
const int EDIT_NONE = 1;
const int EDIT_ACTIVE = 2;
int editMode = EDIT_START;

const int SETUP_ITEM_TEMP = 1;
const int SETUP_ITEM_UNIT = 2;
const int SETUP_ITEM_DEVIATION = 3;
const int SETUP_ITEM_DONE = 4;
int setupItem = SETUP_ITEM_TEMP;

void rotarySetup(){
  pinMode (rotPinA,INPUT);
  pinMode (rotPinB,INPUT);
  pinMode (rotButtonPin, INPUT);
   /* Read Pin A
   Whatever state it's in will reflect the last position   
   */
  attachInterrupt(digitalPinToInterrupt(rotPinB), rotaryIsr, CHANGE);   // interrupt 0 is always connected to pin 2 on Arduino UNO
 
  rotPinALast = digitalRead(rotPinA);   
}

void rotary() { 

  //
  //  Handle Button Press
  //
  if( !(digitalRead(rotButtonPin)) ){
    delay( 100 );
    while( !(digitalRead(rotButtonPin)) );
    Serial.println( "Button Pressed" );

    if( screenMode == SCREENMODE_SCREEN_SAVER ){
      screenMode = SCREENMODE_DISPLAYTEMP;
      lcd.setBacklight(HIGH); // Turn on backlight, LOW for off
      screenTime = millis();
    }
    else if( screenMode != SCREENMODE_SETUP ){
      screenMode = SCREENMODE_SETUP;
      setupItem = SETUP_ITEM_TEMP;
      editMode = EDIT_START;
    }
    if(screenMode ==  SCREENMODE_SETUP && setupItem == SETUP_ITEM_TEMP && editMode == EDIT_START )
    {
      lcd.clear();
      lcd.setCursor ( 0, 0 );            
      lcd.print("Target Temp"); 
      lcd.setCursor ( 0, 1 );
      lcd.print( getDisplayTemperature( targetTemp ) );     
    }
    if(screenMode == SCREENMODE_SETUP ){
      if (editMode == EDIT_START){
        editMode = EDIT_NONE;
      }
      else if (editMode == EDIT_NONE){
         editMode = EDIT_ACTIVE; 
      }
      else if (editMode == EDIT_ACTIVE){
         editMode = EDIT_NONE; 
      }
      if( setupItem == SETUP_ITEM_DONE ){
        screenMode = SCREENMODE_DISPLAYTEMP; 
        setTempratureTemplate();  
        screenTime = millis();      
      }
    }
  }

  if(screenMode == SCREENMODE_SETUP && rotationValue != 0 )
  {
    setSettings( );
  }
  else if ( screenMode != SCREENMODE_SETUP && rotationValue != 0 ){
    changeDisplay();
  }
  if( rotationValue != 0 ){
    rotationValue = 0;
    screenTime = millis();      
  }
}


void rotaryIsr(){
  delay( 2 );
  rotAVal = digitalRead(rotPinA);
  if( rotAVal != rotPinALast ){ // Means the knob is rotating
    rotBVal = digitalRead(rotPinB);
    #ifdef DEBUG
      Serial.print ("Rotated: Last: "  );
      Serial.print( rotPinALast );
      Serial.print (" A Value: "  );
      Serial.print( rotAVal );
      Serial.print (" B Value: "  );
      Serial.print( rotBVal );
    #endif     
    // if the knob is rotating, we need to determine direction
    // We do that by reading pin B.
    if( rotPinALast != 0 ) {
      if( rotAVal == 0 && rotBVal == 0 ) {  // Means pin A Changed first - We're Rotating Clockwise
        rotationValue = 1;
        #ifdef DEBUG        
          Serial.println(" clockwise");        
        #endif
      } else {// Otherwise B changed first and we're moving CCW
        rotationValue = -1;
        #ifdef DEBUG        
          Serial.println(" counterclockwise");
        #endif
      }
    }
    if( rotAVal != rotPinALast ){ // Means the knob is rotating
      rotPinALast = rotAVal;
    }
  }
}


void setSettings( )
{
  if( editMode != EDIT_ACTIVE ){
    setupItem = setupItem + rotationValue;
  }
  if (setupItem > SETUP_ITEM_DONE)
  {
    setupItem = SETUP_ITEM_TEMP;
  }
  else if(setupItem < SETUP_ITEM_TEMP)
  {
    setupItem = SETUP_ITEM_DONE;
  }
  lcd.noCursor();
  Serial.println( " " );
  Serial.println( "ScreenMode: " + String( screenMode ) );
  Serial.println( "Edit Mode:" + String(editMode) + " Setup Item: " + String(setupItem) );
  switch(setupItem){
    case SETUP_ITEM_TEMP:
      if( editMode != EDIT_ACTIVE ){
        Serial.println( "Display temp" );
        lcd.clear();
        lcd.setCursor ( 0, 0 );            
        lcd.print("Target Temp"); 
      }
      else{
        Serial.println( "Update temp" );
        targetTemp = targetTemp + rotationValue;
      }
      Serial.println( "Traget Temp: " + String( targetTemp ) + " : " + getDisplayTemperature( targetTemp ) );
      lcd.setCursor ( 0, 1 );
      lcd.print( getDisplayTemperature( targetTemp ) );          
      break;
    case SETUP_ITEM_UNIT:
      //lcd.print("<"); // pad string with spaces for centering
      if( editMode != EDIT_ACTIVE ){
        lcd.clear();
        //lcd.begin(16,2);        // 20 columns by 4 rows on display
        // lcd.setBacklight(HIGH); // Turn on backlight, LOW for off
        lcd.setCursor ( 0, 0 );            
        lcd.print("Temperature Units"); 
      }
      else{
        if (unit == UNIT_FARENHEIGHT){
          unit = UNIT_CELSIUS;
        }
        else if(unit == UNIT_CELSIUS){
          unit = UNIT_FARENHEIGHT;
        }
      }
      lcd.setCursor ( 0, 1 );
      if(unit == UNIT_CELSIUS){
          lcd.print( "Celsius    " ); 
      }
      else{
          lcd.print("Farenheight");
      }
      break;
    case SETUP_ITEM_DEVIATION:
      if( editMode != EDIT_ACTIVE ){
        lcd.clear();
        //lcd.begin(16,2);        // 20 columns by 4 rows on display
        // lcd.setBacklight(HIGH); // Turn on backlight, LOW for off
        lcd.setCursor ( 0, 0 );            
        lcd.print("Deviation");            
      }
      else{
        if ( rotationValue == 1 ){
          deviation += 0.2;
        }
        else if( rotationValue == -1 ){
          deviation -= 0.2;
        }
     }
     lcd.setCursor ( 0, 1 );
     lcd.print( deviation );          
     break;
    case SETUP_ITEM_DONE:
      if( editMode != EDIT_ACTIVE ){
        lcd.clear();
        // lcd.begin(16,2);        // 20 columns by 4 rows on display
        // lcd.setBacklight(HIGH); // Turn on backlight, LOW for off
        lcd.setCursor ( 0, 0 );   
        lcd.print("Exit"); 
      }
      //lcd.print("<"); // pad string with spaces for centering
      break;
  }
  lcd.cursor();
}
