



int rotPinA = 10;  // Connected to CLK on KY-040
int rotPinB = 11;  // Connected to DT on KY-040
int rotButtonPin = 12;
int rotPinALast;  
int rotAVal;
int rotBVal;
boolean rotBCW;

void setup(void)
{
  
  //setup temp sensor
  // start serial port
  Serial.begin(9600);
  Serial.println("Rotary Test Demo.");


  Serial.println();
  Serial.print( "Setting up Rotary Encoder" );
  //rotary encoder setup
  pinMode (rotPinA,INPUT);
   pinMode (rotPinB,INPUT);
   pinMode (rotButtonPin, INPUT);
   /* Read Pin A
   Whatever state it's in will reflect the last position   
   */
 //  attachInterrupt(digitalPinToInterrupt(rotPinA), isr, FALLING);   // interrupt 0 is always connected to pin 2 on Arduino UNO
 
   rotPinALast = digitalRead(rotPinA);   
   Serial.begin (9600);

}

void rotary() { 

  if( !(digitalRead(rotButtonPin)) ){
    delay(100);
    while( !(digitalRead(rotButtonPin)) );
      Serial.println();
      Serial.print ("Button Pressed: "  );
  }
    
     delay( 2 );
     rotAVal = digitalRead(rotPinA);
     if (rotAVal != rotPinALast ){ // Means the knob is rotating
        // while( !(digitalRead(rotPinA)) );
        rotBVal = digitalRead(rotPinB);
        // while( !(digitalRead(rotPinB)) );
        Serial.println();
        Serial.print ("Rotated: Last: "  );
        Serial.print( rotPinALast );
        Serial.print (" A Value: "  );
        Serial.print( rotAVal );
        Serial.print (" B Value: "  );
        Serial.print( rotBVal );

     // We do that by reading pin B.
       if( rotAVal == 0 && rotBVal == 1 ) {  // Means pin A Changed first - We're Rotating Clockwise
         rotBCW = true;
       } else {// Otherwise B changed first and we're moving CCW
         rotBCW = false;
       }

       if( rotPinALast != 0 ) {
         if (rotBCW){
           Serial.println(" clockwise");
         }else{
           Serial.println(" counterclockwise");
         }
       }
       rotPinALast = rotAVal;
     } 
     // Cleanup up excess
     // delay(10);
     // while( !(digitalRead(rotPinA)) );
     // while( !(digitalRead(rotPinB)) );
     
}


void loop(void)
{ 
  rotary();
}
