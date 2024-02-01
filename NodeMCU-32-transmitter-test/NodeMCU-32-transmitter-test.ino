


const int led_builtin = 2; //builtin NodeMCU LED


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // builtin LED to signal that transmitter is transmitting
  pinMode(led_builtin, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.print('O');
  digitalWrite(led_builtin, LOW);
  delay(1000);
  Serial.print('K');
  digitalWrite(led_builtin, HIGH);
  delay(1000);
  

}
