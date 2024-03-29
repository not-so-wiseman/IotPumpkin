#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
   
#define FIREBASE_HOST "iotpumpkin.firebaseio.com"                         //database api url
#define FIREBASE_AUTH "2f7uLyqTNKePr6oglrKCtM8A3hxJKpSrHOccEhjv"

// Global variables
const int trigPin = 16;
const int echoPin = 5;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Connecting to Wifi");
  WiFi.begin("Denise M", "1Johnston");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);             //connect to Database
  delay(1000);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop() {
  long duration;
  long cm;

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  cm = microsecondsToCentimeters(duration);
  Serial.println("Distance: ");
  Serial.println(cm);
  if(cm <= 20){
    Firebase.setString("status", "ALERT");
    if (Firebase.failed()) {
      Serial.println("Firebase set failed");
      Serial.println(Firebase.error());
      return;
    }
    else{
      Serial.println("ALERT!");
    }
  }
  else {
    Firebase.setString("status", "IDLE"); 
  }

  delay(500);
}


long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}
