#include <SoftwareSerial.h>
#include <TinyGPS.h>
SoftwareSerial SIM900(7, 8);
TinyGPS gps;  //Creates a new instance of the TinyGPS object

#define button1 3 //Button pin, on the other pin it's wired with GND

bool button_State; //Button state


const int trigPin = 9;
const int echoPin = 10;
const int buzzer = 11 ;
const int ledPin = 13;

int sensor = 2;              // the pin that the sensor is atteched to
int state = LOW;             // by default, no motion detected
int val = 0; 
int ldrPin=A0;//Set A0(Analog Input) for LDR.


// defines variables
long duration;
int distance;
int safetyDistance;


void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(buzzer, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(sensor, INPUT);
  pinMode(button1, INPUT_PULLUP); //The button is always on HIGH level, when pressed it goes LOW
  SIM900.begin(9600);
  Serial.begin(9600); // Starts the serial communication
  delay(1000);
}


void loop() {
  //pir.................................................
  
   val = digitalRead(sensor);   // read sensor value
  if (val == HIGH) {           // check if the sensor is HIGH
    digitalWrite(ledPin, HIGH);// turn LED ON  
     digitalWrite(buzzer, HIGH);
    delay(10);                // delay 500 milliseconds 
    
    if (state == LOW) {
      Serial.println("Motion detected!"); 
      state = HIGH;       // update variable state to HIGH
    }
  } 
  else {
      digitalWrite(ledPin, LOW); // turn LED OFF
      delay(10);             // delay 200 milliseconds 
      
      if (state == HIGH){
        Serial.println("Motion stopped!");
        state = LOW;       // update variable state to LOW
    }
  }

  
//LDR..........................................................
  int ldrStatus = analogRead(ldrPin);   //read the status of the LDR value

  //check if the LDR status is <= 300
  //if it is, the LED is HIGH

   if (ldrStatus <=300) {

    digitalWrite(ledPin, HIGH);               //turn LED on
    digitalWrite(buzzer, HIGH);  
    Serial.println("LDR is DARK, LED is ON");
    
   }
  else {

    digitalWrite(ledPin, LOW);          //turn LED off
    digitalWrite(buzzer, LOW);  
    Serial.println("---------------");
  }
  //distance...........................................
digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration/2) / 29.1;
    //when distance is greater than or equal to 200 OR less than or equal to 0,the buzzer and LED are off
  if (distance >= 50 || distance <= 0) 
        {
        Serial.println("no object detected");
        digitalWrite(buzzer,LOW);
        digitalWrite(ledPin,LOW);
        }
  else {
        Serial.println("object detected \n");
        Serial.print("distance= ");              
        Serial.print(distance);        //prints the distance if it is between the range 0 to 200
                    // play tone of 400Hz for 500 ms          
        digitalWrite(ledPin,HIGH);
        digitalWrite(buzzer,HIGH);
        delay(10);
  }

 //gsm---------------------------------------------------------
 button_State = digitalRead(button1);   //We are constantly reading the button State
 
  if (button_State == LOW) {            //And if it's pressed
    Serial.println("Button pressed");   //Shows this message on the serial monitor
    delay(100);                         //Small delay to avoid detecting the button press many times
    
 

    
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      //Serial.print(c);
      if (gps.encode(c)) 
        newData = true;  
    }
  }

  if (newData)      //If newData is true
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);   
    SIM900.print("AT+CMGF=1\r"); 
    delay(400);
    SIM900.println("AT + CMGS = \"+919999888877\"");// recipient's mobile number with country code
    delay(300);
    SIM900.print("Latitude = ");
    SIM900.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    SIM900.print(" Longitude = ");
    SIM900.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    delay(200);
    SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26
    delay(200);
    SIM900.println();
    

  }
 
  Serial.println(failed);
 // if (chars == 0)
   // Serial.println("* No characters received from GPS: check wiring *");
 
  }
}

  

