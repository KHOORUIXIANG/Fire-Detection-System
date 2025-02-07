#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin definitions
#define DHTPIN PB5 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11 sensor type
#define SMOKE_SENSOR_PIN PA0 // Analog pin for smoke sensor input
#define GREEN_SMOKE_LED PA6 // Green LED for smoke indication
#define RED_SMOKE_LED PB9   // Red LED for smoke alert
#define GREEN_TEMP_LED PA9  // Green LED for normal temperature
#define YELLOW_TEMP_LED PA8 // Yellow LED for moderate temperature
#define RED_TEMP_LED PA7    // Red LED for high temperature
#define BUZZER PB8          // Buzzer for smoke alarm

// Temperature thresholds
#define TEMP_LOW 32   // Low temperature threshold (Yellow LED on)
#define TEMP_HIGH 39  // High temperature threshold (Red LED on)

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Initialize LCD display with I2C address 0x27 and size 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Initialize LCD module
  lcd.begin();
  lcd.backlight(); // Turn on backlight
  
  // Initialize DHT sensor
  dht.begin();
  
  // Configure pins
  pinMode(SMOKE_SENSOR_PIN, INPUT);
  pinMode(GREEN_SMOKE_LED, OUTPUT);
  pinMode(RED_SMOKE_LED, OUTPUT);
  pinMode(GREEN_TEMP_LED, OUTPUT);
  pinMode(YELLOW_TEMP_LED, OUTPUT);
  pinMode(RED_TEMP_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  // Read temperature from DHT11 sensor
  int temperature = dht.readTemperature();
  
  // Read smoke level from MQ-2 sensor
  int smokeLevel = analogRead(SMOKE_SENSOR_PIN);
  
  // Display readings on LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Smoke:");
  lcd.print(smokeLevel);
  lcd.setCursor(0,1);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print((char)223); // Degree symbol
  lcd.print("C");
  
  // Smoke detection logic
  if (smokeLevel > 150) { // Adjust threshold based on calibration
    digitalWrite(BUZZER, HIGH);  // Activate buzzer (smoke detected)
    digitalWrite(RED_SMOKE_LED, HIGH);  // Turn on red LED (danger)
    digitalWrite(GREEN_SMOKE_LED, LOW); // Turn off green LED
  } else {
    digitalWrite(BUZZER, LOW);  // Deactivate buzzer
    digitalWrite(RED_SMOKE_LED, LOW);   // Turn off red LED
    digitalWrite(GREEN_SMOKE_LED, HIGH); // Turn on green LED (safe)
  }
  
  // Temperature monitoring logic
  if (temperature < TEMP_LOW) {
    digitalWrite(GREEN_TEMP_LED, HIGH);  // Normal temperature (green LED on)
    digitalWrite(YELLOW_TEMP_LED, LOW);
    digitalWrite(RED_TEMP_LED, LOW);
  } else if (temperature >= TEMP_LOW && temperature <= TEMP_HIGH) {
    digitalWrite(GREEN_TEMP_LED, LOW);
    digitalWrite(YELLOW_TEMP_LED, HIGH); // Moderate high temperature (yellow LED on)
    digitalWrite(RED_TEMP_LED, LOW);
  } else if (temperature > TEMP_HIGH) {
    digitalWrite(GREEN_TEMP_LED, LOW);
    digitalWrite(YELLOW_TEMP_LED, LOW);
    digitalWrite(RED_TEMP_LED, HIGH); // High temperature (red LED on)
  }
  
  delay(500); // Wait before next reading
}
