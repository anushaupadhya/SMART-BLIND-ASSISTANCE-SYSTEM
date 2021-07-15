// C++ code
const int trigPin = 9;
const int echoPin = 10;
const int buzzer = 11;
const int ledPin = 13;
int sensor = A1;              // the pin that the sensor is atteched to
int state = LOW;             // by default, no motion detected
int val = 0; 

long duration;
int distance;
int safetyDistance;



void setup() {
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
pinMode(buzzer, OUTPUT);
pinMode(ledPin, OUTPUT);
pinMode(sensor, INPUT);
Serial.begin(9600);
}


void loop() {
  val = digitalRead(sensor);   // read sensor value
  if (val == HIGH) {           // check if the sensor is HIGH
    digitalWrite(ledPin, HIGH); // turn LED ON
     digitalWrite(buzzer, HIGH);
    delay(500);                // delay 500 milliseconds 
    
    if (state == LOW) {
      Serial.println("Motion detected!"); 
      state = HIGH;       // update variable state to HIGH
    }
  } 
  else {
      digitalWrite(ledPin, LOW); // turn LED OFF
      delay(500);             // delay 200 milliseconds 
      
      if (state == HIGH){
        Serial.println("Motion stopped!");
        state = LOW;       // update variable state to LOW
    }
  }

digitalWrite(trigPin, LOW);
delayMicroseconds(2);
  
digitalWrite(trigPin, HIGH);
delayMicroseconds(10); 
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);

distance= duration*0.034/2;
 
safetyDistance = distance;
if(safetyDistance <25){
  digitalWrite(buzzer, HIGH);
  digitalWrite(ledPin, HIGH);
  delay(500);
}
else{
  digitalWrite(buzzer, LOW);
  digitalWrite(ledPin, LOW);
  Serial.print("out of range");
}
  
  
 Serial.print("Distance:");
 Serial.println(distance);
}

  

