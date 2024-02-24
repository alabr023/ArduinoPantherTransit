#include <ESP8266Firebase.h>
#include <ESP8266WiFi.h>

#define _SSID "wifi"          // Your WiFi SSID
#define _PASSWORD "pw"      // Your WiFi Password
#define REFERENCE_URL "url"  // Your Firebase project reference url

Firebase firebase(REFERENCE_URL);

const int photoresistorPin = A0;
const int redLED = 12;
const int greenLED = 13;

bool emptySpot = false;
bool prevState = false;

void setup() {
  Serial.begin(9600);
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
  // Read analog value from the photoresistor
  int sensorValue = analogRead(photoresistorPin);
  
  // Convert analog value to voltage (assuming 3.3V reference voltage)
  float voltage = sensorValue * (3.3 / 1023.0);
  
  // Convert voltage to resistance using voltage divider formula
  // (you need to replace 10K with the actual resistance of the resistor in your voltage divider)
  float resistance = (1000.0 / (3.3 - voltage)) * voltage;
  
  // Print sensor value, voltage, and resistance to serial monitor
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println("V");
  Serial.print("Resistance: ");
  Serial.print(resistance);
  Serial.println(" ohms");

  emptySpot = voltage <= 3.3 / 2;

  digitalWrite(redLED, emptySpot);
  digitalWrite(greenLED, !emptySpot);

  if (emptySpot != prevState) {
    firebase.setInt("Parking_lot_1_test/parking_spot_1_test", emptySpot);
    prevState = emptySpot;
  }

  // Add delay if necessary
  delay(100);
}
