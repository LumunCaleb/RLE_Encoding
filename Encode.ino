#include<Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 7          // Pin where the DHT11 is connected
#define DHTTYPE DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE); // Create a DHT object

  const int numReadings = 10; // Number of readings to take
  float humidityReadings[numReadings];
  float temperatureReadings[numReadings];
  float humidity, temperature;
void setup() 
{
  Serial.begin(9600); // Initialize serial communication
  Serial.println("CLEARDATA");
Serial.print("LABEL,TIME,TIMER,Humidity,,,Temperature,\n"); // Print column headers  //Serial.println("RESETTIMER");
// LEave 2 extra colmns for ther 
  dht.begin(); // Initialize the DHT sensor
}


void loop() {
  Serial.print("CLEARDATA\n"); // Clear any previous data in PLX-DAQ
//  Serial.print("LABEL,TIME,TIMER,Hum1,Hum2,Hum3,,,,Temp.1,Temp.2,Temp3,Temp4\n"); // Print column headers
  
  const int numReadings = 3; // Number of readings to take
  float humidityReadings[numReadings];
  float temperatureReadings[numReadings];
  
  // Loop to take multiple readings
  for (int i = 0; i < numReadings; ++i) {
    delay(500);  // Wait for 2 seconds between readings
    
    float humidity = dht.readHumidity(); // Read humidity from the DHT sensor
    float temperature = dht.readTemperature(); // Read temperature from the DHT sensor
    
    // Print raw sensor readings for debugging
    Serial.print("Raw Humidity Reading: ");
    Serial.println(humidity);
    Serial.print("Raw Temperature Reading: ");
    Serial.println(temperature);
    
    // Check if any reads failed
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
      continue; // Skip this iteration and continue with the next one
    }
    
    // Store readings in arrays
    humidityReadings[i] = humidity;
    temperatureReadings[i] = temperature;
  }
  
  // Print encoded sensor readings in tabular format
  Serial.print("DATA,TIME,TIMER,");
  encodeAndPrint(humidityReadings, numReadings);
  Serial.print(",");       //increase the number of "," to shift the columns to the right
  Serial.print(","); 
  Serial.print(",");

  encodeAndPrint(temperatureReadings, numReadings);
  Serial.println(); // Move to the next line after printing the data
  
//  delay(10000);  // Wait for 10 seconds before printing the next set of readings
}

// Function to encode and print sensor data
void encodeAndPrint(float data[], int length) {
  // Loop through the data array
  for (int i = 0; i < length; ++i) {
    int count = 1;
    // Check if consecutive readings are the same
    while (i < length - 1 && data[i] == data[i + 1]) {
      ++count; // Increment count
      ++i; // Move to the next element
    }
    
    // Print the encoded data
    Serial.print(data[i]);
    Serial.print("(");
    Serial.print(count);
    Serial.print(")");
    if (i < length - 1) {
      Serial.print(","); // Add comma if it's not the last element
    }
  }
}
