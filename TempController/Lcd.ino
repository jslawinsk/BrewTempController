

//LCD includes
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h> // include i/o class header

// LiquidCrystal Display
hd44780_I2Cexp lcd; // declare lcd object: auto locate & config display for hd44780 chip

#ifdef LCD_20X4
  int rows = 4;
  int columns = 20;
#else
  int rows = 2;
  int columns = 16;      
#endif

void lcdsetCursor( int col, int row ){
  lcd.setCursor ( col, row );
}

void lcdprint( String str ){
  lcd.print( str );
}

void lcdprint( float flt ){
  lcd.print( flt );
}

void lcdclear(){
  lcd.clear();
}

void lcdsetBacklightHIGH(){
  lcd.setBacklight( HIGH );  
}

void lcdsetBacklightLOW(){
  lcd.setBacklight( LOW );  
}

void lcdnoCursor(){
  lcd.noCursor();
}

void lcdcursor(){
  lcd.cursor();
}
void changeDisplay(){
  currentDisplayMode = currentDisplayMode + 1;
  if( currentDisplayMode > DISPLAYMODE_MINMAX ){
    currentDisplayMode = DISPLAYMODE_MAIN;
  }
  setTempratureTemplate();
}

void setTempratureTemplate(){
    //setup LCD
    lcd.begin(  columns, rows );        // 20 columns by 4 rows on display    
  
    lcd.setBacklight(HIGH); // Turn on backlight, LOW for off
    lcd.clear();
    
    lcd.setCursor ( 0, 0 );            // go to the top left corner
    #ifdef LCD_20X4
      lcd.print("Temperature ");   
    #else
      lcd.print("Temp ");
    #endif
    switch( currentDisplayMode ){
      case DISPLAYMODE_MAIN: 
        displayRelayTemplate( 1 );
        #ifdef LCD_20X4
          displayMinMax( 2 );
          displayTarget( 3 );
        #endif
        break;
      case DISPLAYMODE_TARGET:
        displayTarget( 1 );
        #ifdef LCD_20X4
          displayRelayTemplate( 2 );
          displayMinMax( 3 );
        #endif
        break;     
      case DISPLAYMODE_MINMAX:
        displayMinMax( 1 );
        #ifdef LCD_20X4
          displayTarget( 2 );
          displayRelayTemplate( 3 );
        #endif        
        break;
    }
}

void displayMinMax( int row )
{
  lcd.setCursor ( 0, row );            
  #ifdef LCD_20X4
    lcd.print("Min "); 
    lcd.setCursor ( 4, row );            
  #else
    lcd.print("Mn "); 
    lcd.setCursor ( 3, row );            
  #endif
  lcd.print( getDisplayTemperatureBasic( _minTemp ) );
  
  #ifdef LCD_20X4
    lcd.setCursor ( 9, row );            
    lcd.print("Max "); 
    lcd.setCursor ( 13, row );            
  #else
    lcd.setCursor ( 8, row );            
    lcd.print("Mx "); 
    lcd.setCursor ( 11, row );            
  #endif
  lcd.print( getDisplayTemperatureBasic( _maxTemp ) ); 
}

void displayTarget( int row )
{
  lcd.setCursor ( 5, row );            
  lcd.print("Target "); 
  #ifdef LCD_20X4
    lcd.setCursor ( 13, row );
  #else
    lcd.setCursor ( 12, row );
  #endif
  lcd.print( getDisplayTemperature( targetTemp ) );                      
}

void displayLogo(){
    lcd.begin( columns, rows );        // 20 columns by 4 rows on display    
  
    lcd.setBacklight(HIGH); // Turn on backlight, LOW for off
    lcd.clear();
    
    lcd.setCursor ( 0, 0 );            // go to the top left corner
    #ifdef LCD_20X4
      lcd.print("BrewTemp Controller");   
      lcd.setCursor ( 0, 2 );            // go to the 3rd row
    #else
      lcd.print("BrewTemp Control");   
      lcd.setCursor ( 0, 1 );            // go to the 2nd row
    #endif
    lcd.print("Version 1.0");   
}

void displayMessage( String message ){  
    lcd.setBacklight(HIGH); // Turn on backlight, LOW for off
    lcd.clear();
    #ifdef LCD_20X4
      lcd.setCursor ( 0, 3 );            // go to the bottom left corner
    #else
      lcd.setCursor ( 0, 0 );            // go to the top left corner
    #endif
    
    lcd.print( message );   
    delay( 4000 );
    setTempratureTemplate();
}
