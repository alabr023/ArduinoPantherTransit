tgit#include <ESP8266Firebase.h>
#include <ESP8266WiFi.h>

#define _SSID ""          // Your WiFi SSID
#define _PASSWORD ""      // Your WiFi Password
#define REFERENCE_URL "https://test-5e724-default-rtdb.firebaseio.com/"  // Your Firebase project reference url

Firebase firebase(REFERENCE_URL);

const int trigPin = D2;  // Pin connected to HC-SR04 trigger
const int echoPin = D3;  // Pin connected to HC-SR04 echo
const int redLED = D6;   // Red LED pin
const int greenLED = D7; // Green LED pin

bool emptySpot = false;
bool prevState = false;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  // Connect to WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("-");
  }

  Serial.println("");
  Serial.println("WiFi Connected");

  // Print the IP address
  Serial.print("IP Address: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
  // Trigger ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Measure the duration of the echo pulse
  unsigned long duration = pulseIn(echoPin, HIGH);
  
  // Calculate distance in cm
  float distance = duration * 0.034 / 2;

  // Print distance to serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Check if distance is less than or equal to 10cm
  emptySpot = (distance <= 10);

  // Update LED status
  digitalWrite(redLED, emptySpot);
  digitalWrite(greenLED, !emptySpot);

  // Update Firebase if status changes
  if (emptySpot != prevState) {
    firebase.setInt("home/spot1", emptySpot);
    prevState = emptySpot;
  }

  // Add delay if necessary
  delay(100);
}
