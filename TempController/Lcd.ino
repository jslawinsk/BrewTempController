

//LCD includes
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h> // include i/o class header

// LiquidCrystal Display
hd44780_I2Cexp lcd; // declare lcd object: auto locate & config display for hd44780 chip

const int LCD_TYPE_16x2 = 0;
const int LCD_TYPE_20x4 = 1;

int lcdType = LCD_TYPE_16x2;
int rows;
int columns;

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
        lcd.print( getDisplayTemperature( _minTemp ) ); // pad string with spaces for centering
      
        lcd.setCursor ( 8, 1 );            // go to the 3rd row
        lcd.print("Mx "); // pad string with spaces for centering
        lcd.setCursor ( 11, 1 );            // go to the 2nd row
        lcd.print( getDisplayTemperature( _maxTemp ) ); // pad string with spaces for centering
        break;
    }
}

void displayLogo(){
    if( lcdType = LCD_TYPE_20x4 ){
      rows = 4;
      columns = 20;
    }
    else{
      rows = 2;
      columns = 16;      
    }
    lcd.begin( columns, rows );        // 20 columns by 4 rows on display    
  
    lcd.setBacklight(HIGH); // Turn on backlight, LOW for off
    lcd.clear();
    
    lcd.setCursor ( 0, 0 );            // go to the top left corner
    if( lcdType = LCD_TYPE_20x4 ){
      lcd.print("BrewTemp Controller");   
      lcd.setCursor ( 0, 2 );            // go to the 2nd row
    }
    else{
      lcd.print("BrewTemp Control");   
      lcd.setCursor ( 0, 1 );            // go to the 2nd row
    }
    lcd.print("Version 1.0");   
}

void displayMessage( String message ){
  
    lcd.setBacklight(HIGH); // Turn on backlight, LOW for off
    lcd.clear();
    
    if( lcdType = LCD_TYPE_20x4 ){
      lcd.setCursor ( 0, 3 );            // go to the bottom left corner
    }
    else {
      lcd.setCursor ( 0, 0 );            // go to the top left corner
    }  
    lcd.print( message );   
    delay( 4000 );
    setTempratureTemplate();
}
