

int Relay1 = 6;
int Relay2 = 7;

void setup() 
{ 
    pinMode(Relay1, OUTPUT); //Set Pin12 as output 
    pinMode(Relay2, OUTPUT); //Set Pin12 as output 
}

void loop() 
{ 
    digitalWrite(Relay1, HIGH); //Turn off relay 
    digitalWrite(Relay2, LOW); //Turn off relay 
    delay(2000); 
    digitalWrite(Relay1, LOW); //Turn on relay 
    digitalWrite(Relay2, HIGH); //Turn off relay 
    delay(2000); 
} 
