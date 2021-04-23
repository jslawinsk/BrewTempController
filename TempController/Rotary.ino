

int pinAstateCurrent = LOW;                // Current state of Pin A
int pinAStateLast = pinAstateCurrent;      // Last read value of Pin A

#ifdef D1_MINI
  int pinB = D0;        // Connected to DT on KY-040
  int pinA = D5;        // Connected to CLK on KY-040
  int switchPin = D3;   // Connected to SW on KY-040
#else
  int pinB = 3;        // Connected to DT on KY-040
  int pinA = 10;       // Connected to CLK on KY-040
  int switchPin = 12;  // Connected to SW on KY-040
#endif

int switchState = HIGH; // button value

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

  pinMode ( switchPin, INPUT_PULLUP );
  pinMode ( pinA, INPUT );
  pinMode ( pinB, INPUT );
   /* Read Pin A
   Whatever state it's in will reflect the last position   
   */
  attachInterrupt(digitalPinToInterrupt( pinA), rotaryIsr, CHANGE);   // interrupt 0 is always connected to pin 2 on Arduino UNO
  // pinAStateLast = digitalRead( pinA );   
}

void rotary(){
  //
  //  Handle Button Press
  //
  switchState = digitalRead( switchPin );    // Read the digital value of the switch (LOW/HIGH)
  // If the switch is pressed (LOW), button pressed
  if( switchState == LOW ) {
    delay( 100 );
    while( !(digitalRead( switchPin )) );
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


#ifdef D1_MINI
ICACHE_RAM_ATTR void rotaryIsr(){
#else
void rotaryIsr(){
#endif
  // delay( 2 );
  pinAstateCurrent = digitalRead( pinA );
  if ((pinAStateLast == LOW) && (pinAstateCurrent == HIGH)) {  

    if (digitalRead( pinB ) == HIGH) {      // If Pin B is HIGH
      rotationValue = -1;
      #ifdef DEBUG
      Serial.println("counterclockwise");             // Print on screen
      #endif
    } else {
      rotationValue = 1;
      #ifdef DEBUG
      Serial.println("clockwise");            // Print on screen
      #endif
    }
  }
  pinAStateLast = pinAstateCurrent; // Store the latest read value in the currect state variable
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
