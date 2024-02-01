

// i2c control
#include <Wire.h>

// ssd1306 for oled display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// OLED display is at 0x3C
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {

  Serial.begin(115200);
  
  // setup builting LED
  pinMode(2, OUTPUT);


  // setup oled display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.setTextSize(1);
  display.setTextColor(WHITE);


}

void loop() {
  // builtin LED on Node MCU
  digitalWrite(2, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(300);
  digitalWrite(2, LOW); // turn the LED off by making the voltage LOW
  delay(300);


  // Print text on the oled display
  display.clearDisplay();
  display.display();
  delay(500);
  
  display.setCursor(0, 0);
  // Display TEMP text
  display.print("TEMP -- ");
  display.display();
  delay(500);

  display.setCursor(50, 25);
  display.print("PRES");
  display.display();
  delay(500);

  display.setCursor(80, 50);
  display.print("HUM");
  display.display(); 
  delay(1000);

}
