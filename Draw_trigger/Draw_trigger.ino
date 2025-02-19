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
#define CLK 34
#define DT 26
#define SW 35

int event_freq = 25;
int currentStateCLK;
int previousStateCLK;
String encdir ="";

int false_start_ratio = 1;


void initialize() {
  for (int i = 0; i<7; i++) {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(BEEPER, HIGH);
  delay(50);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(BEEPER, LOW);
  delay(50);
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
  int event_choice = random(100);
  if (event_choice > 50){
    display.clearDisplay();
    display.display();
    display.setTextSize(2);
    display.setCursor(40, 25);
    display.print("FIRE");
    display.invertDisplay(true);
    display.display();
    digitalWrite(LED_BUILTIN, HIGH); 
    digitalWrite(BEEPER, HIGH);
    delay(50); 
    digitalWrite(LED_BUILTIN, LOW);   
    digitalWrite(BEEPER, LOW);
    delay(50);
    digitalWrite(LED_BUILTIN, HIGH); 
    digitalWrite(BEEPER, HIGH);
    delay(50); 
    digitalWrite(LED_BUILTIN, LOW);   
    digitalWrite(BEEPER, LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH); 
    digitalWrite(BEEPER, HIGH);
    delay(500); 
    digitalWrite(BEEPER, LOW);  
    
  }
  // False start
  else if (event_choice < 50) {
    display.clearDisplay();
    display.display();
    display.setTextSize(2);
    display.setCursor(15, 20);
    display.print("FALSE");
    display.setCursor(15, 40);
    display.print("START");
    display.invertDisplay(false);
    display.display();
    digitalWrite(BEEPER, HIGH);
    digitalWrite(LED_BUILTIN, HIGH); 
    delay(1000);
    digitalWrite(BEEPER, LOW);
    digitalWrite(LED_BUILTIN, LOW); 
  }
}

/*
void IRAM_ATTR ISR_encoder() {
  if (digitalRead(DT) == HIGH) {
    event_freq++;
    Serial.println("Mark 1");
  } else {
    event_freq--;
    Serial.println("Mark 2");
  }
}
*/

//######### SETUPSETUPSETUPSETUPSETUPSETUPSETUPSETUPSETUPSETUPSETUP
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);
  pinMode(SW, INPUT_PULLUP);
  pinMode(BEEPER, OUTPUT);
  //attachInterrupt(digitalPinToInterrupt(CLK), ISR_encoder, CHANGE);

  Serial.begin(9600);
  Serial.println("Powering up.");
  
  initialize();
}

// the loop function runs over and over again forever
void loop() {

  currentStateCLK = digitalRead(CLK);
  if (currentStateCLK != previousStateCLK) {
    if (digitalRead(DT) != currentStateCLK) {
      event_freq --;
      encdir ="CCW";
      Serial.println("CCW");
    }
    else {
      //encoder is rotating clockwise
      event_freq ++;
      encdir ="CW";
      Serial.println("CW");
    }
    previousStateCLK = currentStateCLK;
  }
  
  if (random(9) == 1) {
  display.invertDisplay(false);
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setCursor(2,2);
  display.print("\tEF: ");
  display.print(event_freq);
  display.print("/10k \tFSR: ");
  display.print(false_start_ratio);
  display.println(":1");
  }
  display.setTextSize(1);
  display.setCursor((random(15,128)), (random(14,64)));
  display.print("x");
  display.display();
  
  // trigger either a regular start or a random start
  if (random(10000) < event_freq) {
        regular_or_false_start();
  }
  if (digitalRead(SW) == HIGH11) {
    Serial.println("================Switch pressed============");
  }
  Serial.print("Event Frequency: ");
  Serial.println(event_freq);
  
  digitalWrite(LED_BUILTIN, HIGH);  
  //delay(30);
  digitalWrite(LED_BUILTIN, LOW);
  //delay(30);

}
