// NodeMCU with Adafruit BME280 breakout and SSD1306 Oled
// program is simple; Samples temp, hum and pres from sensor, and then displays to oled

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// OLED display is at 0x3C
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// BME280 definitions
#define SEALEVELPRESSURE_HPA (1013.25)

// BME280 using I2C data bus
Adafruit_BME280 bme;

unsigned long delay_time;

float temp;
float hum;
float pres;
float alt;

const int led_builtin = 2;


void setup() {
  Serial.begin(115200);
  // setup onboard status indicator LED
  pinMode(2, OUTPUT);

  // setup oled display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.setTextSize(1);
  display.setTextColor(WHITE);

  //Setup BME280
  while(!Serial); //get serial running
  Serial.println(F("BME280 test"));

  unsigned status;
  status = bme.begin();

  if (!status) {
    Serial.println("Could not find valid BME280 sensor, check wiring address, sensor ID");
    Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
  }
  Serial.println("Normal functioning");
  delay_time = 1000;
  Serial.println();
}

void sample_sensor_and_serial_print() {
  // Temp
  temp = bme.readTemperature();
  Serial.print("Temp: ");
  Serial.println(temp);
  

  // humidity
  hum = bme.readHumidity();
  Serial.print("Hum: ");
  Serial.println(hum);

  // pressure
  pres = bme.readPressure();
  Serial.print("Pressure: ");
  Serial.println(pres);

  // altitude
  alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
  Serial.print("Approx Altitude: ");
  Serial.println(alt);

  // Send over serial com as JSON object
  // $$$$$$$ to do list item
  }


void oled_display_sensor_values() {
  display.clearDisplay();
  display.display();
  delay(50);

  display.setCursor(0,0);
  display.print("Temp: ");
  display.print(temp);
  display.print("C");
  display.display();

  display.setCursor(0,17);
  display.print("Hum: ");
  display.print(hum);
  display.println("%");
  display.display();

  display.setCursor(0,35);
  display.print("pres: ");
  display.println(pres);
  display.display();
  delay(2000);
  
}





void loop() {

  digitalWrite(led_builtin, HIGH);
  sample_sensor_and_serial_print();
  digitalWrite(led_builtin, LOW);
  oled_display_sensor_values();
  

}
