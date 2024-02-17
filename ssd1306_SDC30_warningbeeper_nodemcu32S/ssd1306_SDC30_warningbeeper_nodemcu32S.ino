

// i2c control
#include <Wire.h>

// ssd1306 for oled display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_SCD30.h>

Adafruit_SCD30 scd30;


float co2;
float hum;
float temp;


// OLED display is at 0x3C
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// define beeper pin to indicate if CO2 levels are too high (GPIO 32)
#define beeper_pin 33


void setup() {

  Serial.begin(115200);

  // Initialize SCD30
  if (!scd30.begin()) {
    Serial.println("Failed to find SCD30 chip");
    while (1) { delay(10); }
  }
  Serial.println("SCD30 Found!");
  // if (!scd30.setMeasurementInterval(10)){
  //   Serial.println("Failed to set measurement interval");
  //   while(1){ delay(10);}
  // }
  Serial.print("Measurement Interval: "); 
  Serial.print(scd30.getMeasurementInterval()); 
  Serial.println(" seconds");

  
  // setup builtin LED
  pinMode(2, OUTPUT);
  // setup beeper CO2 warning pin
  pinMode(32, OUTPUT);

  // setup oled display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.setTextSize(1);
  display.setTextColor(WHITE);

}



void sample_CO2_sensor() {
  // samples C02 Sensor and stores values
  
  if (scd30.dataReady()){
    Serial.println("Data available!");

    if (!scd30.read()){ Serial.println("Error reading sensor data"); return; }

    Serial.print(scd30.temperature);
    temp = scd30.temperature;
    Serial.println("C");
    
    Serial.print(scd30.relative_humidity);
    hum = scd30.relative_humidity;
    Serial.println("%R/H");
    
    Serial.print("CO2: ");
    Serial.print(scd30.CO2, 3);
    Serial.println(" ppm");
    co2 = scd30.CO2;
  } else {
    Serial.println("No data");
  }
}

void assess_CO2_and_warn()
{
  if (scd30.CO2 > 5000)
  {
    Serial.println("DANGER: CO2 levels are at dangerous levels");
    display.clearDisplay();
    display.display();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("DANGER:");
    display.println("CO2 levels");
    display.print("above 5000ppm");
    display.display();
    
    analogWrite(beeper_pin, 255);
    delay(1000);
    analogWrite(beeper_pin, 70);
    delay(500);
    analogWrite(beeper_pin, 0);
  }
  else if (scd30.CO2 > 3000)
  {
    Serial.println("WARNING: CO2 levels are too high.");
    display.clearDisplay();
    display.display();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("WARNING:");
    display.println("ELAVATED");
    display.println("CO2 levels");
    display.display();
    
    analogWrite(beeper_pin, 127);
    delay(200);
    analogWrite(beeper_pin, 200);
    delay(170);
    analogWrite(beeper_pin, 0);
    delay(700);
  }
  else
  {
    return;
  }
}


void loop() {

  // builtin LED on Node MCU
  digitalWrite(2, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(100);
  digitalWrite(2, LOW); // turn the LED off by making the voltage LOW
  delay(100);

  sample_CO2_sensor();

  // Print text on the oled display
  display.clearDisplay();
  display.display();

///////////////////////////FIRST SCREEN///////////////////////////////
  
  display.setCursor(0, 0);
  // Temperature
  display.setTextSize(1);
  display.print("Temp: ");
  display.print(temp);
  display.println("C");

  // humidity
  display.setTextSize(2);
  display.setCursor(0, 25);
  display.println("R_H");  
  display.print(hum);
  display.print("%");

  display.display();

  
  delay(1400);
///////////////////////////////////SECOND SCREEN////////////////////////////////

  
  display.clearDisplay();
  display.display();

  display.setCursor(0, 0);
  // Temperature
  display.setTextSize(1);
  display.print("Temp: ");
  display.print(temp);
  display.println("C");
  display.display();
  
  // co2
  display.setCursor(0, 25);
  display.setTextSize(2);
  display.println("C02 PPM:");
  display.println(co2, 3);
  display.display(); 
  
  delay(1200);
  sample_CO2_sensor();

  ///////////////////////////////////THIRD SCREEN////////////////////////////////

  
  display.clearDisplay();
  display.display();

  display.setCursor(0, 0);
  // Temperature
  display.setTextSize(1);
  display.print("Temp: ");
  display.print(temp);
  display.println("C");
  display.display();
  
  // co2 (percentage in atmosphere)
  display.setCursor(0, 25);
  display.setTextSize(2);
  display.println("C02:");
  display.print((co2/10000), 3);
  display.println("%");
  display.display(); 
  
  delay(1200);

  assess_CO2_and_warn();
  
}
