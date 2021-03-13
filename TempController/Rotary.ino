

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
const int SETUP_ITEM_CONTROL = 4;
const int SETUP_ITEM_DONE = 5;
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
    #ifdef DEBUG
      Serial.println( "Button Pressed" );
    #endif
    
    if( screenMode == SCREENMODE_SCREEN_SAVER ){
      screenMode = SCREENMODE_DISPLAYTEMP;
      lcdsetBacklightHIGH(); // Turn on backlight, LOW for off
      screenTime = millis();
    }
    else if( screenMode != SCREENMODE_SETUP ){
      screenMode = SCREENMODE_SETUP;
      setupItem = SETUP_ITEM_TEMP;
      editMode = EDIT_START;
    }
    if(screenMode ==  SCREENMODE_SETUP && setupItem == SETUP_ITEM_TEMP && editMode == EDIT_START )
    {
      lcdclear();
      lcdsetCursor ( 0, 0 );            
      lcdprint("Target Temp"); 
      lcdsetCursor ( 0, 1 );
      lcdprint( getDisplayTemperature( targetTemp ) );     
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
  else if ( screenMode != SCREENMODE_SETUP && screenMode != SCREENMODE_SCREEN_SAVER && rotationValue != 0 ){
    //
    //  Here we are checking screenMode != SCREENMODE_SETUP 
    //  because the ISR could be triggered between if and else
    //  this is to avoid gost display while in settings. If
    //  screenMode != SCREENMODE_SETUP is removed gost display appears while in settings (sometimes).
    //
    changeDisplay();
  }
  if( rotationValue != 0 ){
    rotationValue = 0;
    screenTime = millis(); 
    if( screenMode == SCREENMODE_SCREEN_SAVER ){
      screenMode = SCREENMODE_DISPLAYTEMP;
      lcdsetBacklightHIGH(); 
    }         
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
  lcdnoCursor();
  #ifdef DEBUG
    Serial.println( " " );
    Serial.println( "ScreenMode: " + String( screenMode ) );
    Serial.println( "Edit Mode:" + String(editMode) + " Setup Item: " + String(setupItem) );
  #endif
  switch(setupItem){
    case SETUP_ITEM_TEMP:
      if( editMode != EDIT_ACTIVE ){
        #ifdef DEBUG
          Serial.println( "Display temp" );
        #endif
        lcdclear();
        lcdsetCursor ( 0, 0 );            
        lcdprint("Target Temp"); 
      }
      else{
        #ifdef DEBUG
          Serial.println( "Update temp" );
        #endif
        targetTemp = targetTemp + rotationValue;
      }
      #ifdef DEBUG
        Serial.println( "Traget Temp: " + String( targetTemp ) + " : " + getDisplayTemperature( targetTemp ) );
      #endif
      lcdsetCursor ( 0, 1 );
      lcdprint( getDisplayTemperature( targetTemp ) );          
      break;
    case SETUP_ITEM_UNIT:
      //lcdprint("<"); // pad string with spaces for centering
      if( editMode != EDIT_ACTIVE ){
        lcdclear();
        lcdsetCursor ( 0, 0 );            
        lcdprint("Temperature Units"); 
      }
      else{
        if (unit == UNIT_FARENHEIGHT){
          unit = UNIT_CELSIUS;
        }
        else if(unit == UNIT_CELSIUS){
          unit = UNIT_FARENHEIGHT;
        }
      }
      lcdsetCursor ( 0, 1 );
      if(unit == UNIT_CELSIUS){
          lcdprint( "Celsius    " ); 
      }
      else{
          lcdprint("Farenheight");
      }
      break;
    case SETUP_ITEM_DEVIATION:
      if( editMode != EDIT_ACTIVE ){
        lcdclear();
        lcdsetCursor ( 0, 0 );            
        lcdprint("Deviation");            
      }
      else{
        if ( rotationValue == 1 ){
          deviation += 0.2;
        }
        else if( rotationValue == -1 ){
          deviation -= 0.2;
        }
     }
     lcdsetCursor ( 0, 1 );
     lcdprint( deviation );          
     break;
    case SETUP_ITEM_CONTROL:
      if( editMode != EDIT_ACTIVE ){
        lcdclear();
        lcdsetCursor ( 0, 0 );            
        lcdprint("Control"); 
      }
      else{
        if ( controlMode == CONTROL_AUTO ){
          controlMode = CONTROL_HEAT;
        }
        else if(controlMode == CONTROL_HEAT ){
          controlMode = CONTROL_COOL;
        }
        else if(controlMode == CONTROL_COOL ){
          controlMode = CONTROL_AUTO;
        }
      }
      lcdsetCursor ( 0, 1 );
      if( controlMode == CONTROL_AUTO ){
          lcdprint( "Auto" ); 
      }
      else if( controlMode == CONTROL_HEAT ){
          lcdprint( "Heat" ); 
      }
      else{
          lcdprint("Cool");
      }
      break;
    case SETUP_ITEM_DONE:
      if( editMode != EDIT_ACTIVE ){
        lcdclear();
        lcdsetCursor ( 0, 0 );   
        lcdprint("Exit"); 
      }
      //lcdprint("<"); // pad string with spaces for centering
      break;
  }
  lcdcursor();
}
