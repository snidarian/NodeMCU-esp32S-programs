/*
 * Draw shot device
 * 
*/

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


#define LED_BUILTIN 2
#define BEEPER 32
// Rotary encoder inputs
#define CLK 25
#define DT 26
#define SW 13

int freq = 100;
volatile int counter = 0;
volatile int direction = 0;


void initialize() {
  for (int i = 0; i<7; i++) {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(BEEPER, HIGH);
  delay(80);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(BEEPER, LOW);
  delay(80);
  // setup oled display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  }
}


void regular_or_false_start() {
  // Regular start
  if (random(100) > 50){
    display.clearDisplay();
    display.display();
    display.setTextSize(2);
    display.setCursor(40, 25);
    display.print("FIRE");
    display.display();
    digitalWrite(LED_BUILTIN, HIGH); 
    digitalWrite(BEEPER, HIGH);
    delay(80); 
    digitalWrite(LED_BUILTIN, LOW);   
    digitalWrite(BEEPER, LOW);
    delay(50);
    digitalWrite(LED_BUILTIN, HIGH); 
    digitalWrite(BEEPER, HIGH);
    delay(80); 
    digitalWrite(LED_BUILTIN, LOW);   
    digitalWrite(BEEPER, LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH); 
    digitalWrite(BEEPER, HIGH);
    delay(500); 
    digitalWrite(BEEPER, LOW);  
    
  }
  // False start
  else if (random(100) < 50) {
    display.clearDisplay();
    display.display();
    display.setTextSize(2);
    display.setCursor(10, 25);
    display.print("DONT FIRE");
    display.display();
    digitalWrite(BEEPER, HIGH);
    digitalWrite(LED_BUILTIN, HIGH); 
    delay(1000);
    digitalWrite(BEEPER, LOW);
    digitalWrite(LED_BUILTIN, LOW); 
  }
}

void IRAM_ATTR ISR_encoder() {
  if (digitalRead(DT) == HIGH) {
    freq++;
    Serial.println("Mark 1");
  } else {
    freq--;
    Serial.println("Mark 2");
  }
}

//######### SETUPSETUPSETUPSETUPSETUPSETUPSETUPSETUPSETUPSETUPSETUP
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  pinMode(BEEPER, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(CLK), ISR_encoder, CHANGE);

  Serial.begin(9600);
  Serial.println("Powering up.");
  
  initialize();
}

// the loop function runs over and over again forever
void loop() {

  if (random(5) == 1) {
  display.clearDisplay();
  display.display();
  }
  display.setTextSize(1);
  display.setCursor((random(128)), (random(64)));
  display.print("x");
  display.display();
  //delay(freq);
  // trigger either a regular start or a random start

  if (random(1000) > 980) {
        regular_or_false_start();
  }
  if (digitalRead(SW) == LOW) {
    Serial.println("Switch pressed");
  }
  Serial.print("freq: ");
  Serial.println(freq);
  
  digitalWrite(LED_BUILTIN, HIGH);  
  delay(30);
  digitalWrite(LED_BUILTIN, LOW);
  delay(30);

}
