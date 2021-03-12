
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h> // include i/o class header

// #define DEBUG 1

hd44780_I2Cexp lcd; // declare lcd object: auto locate & config display for hd44780 chip

void setup() {
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("hello, world!");
  lcd.setCursor(2,1);
  lcd.print("Hello Arduino!");
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis()/1000);

//  lcd.setBacklight(HIGH);
//  delay(500);
//  lcd.setBacklight(LOW);
//  delay(500);
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
}
