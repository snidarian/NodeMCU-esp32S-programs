
const int led_builtin = 2; // builtin NodeMCU LED
const int tx_led = 7; // Tx led orange
const int rx_led = 8; // rx led green
int incoming_byte;


void setup() {
  Serial.begin(9600);
  // builtin led to signal that either a Tx or Rx data transaction is occuring  
  pinMode(led_builtin, OUTPUT);
  pinMode(tx_led, OUTPUT); // signals a Tx data transaction
  pinMode(rx_led, OUTPUT); // signals a Rx data transaction

}

void loop() {
  // reciever loop
  // See if there's incoming serial data
  if (Serial.available() > 0) {
    // read oldest byte in serial buffer
    incoming_byte = Serial.read();
    if (incoming_byte == 't') {
      digitalWrite(led_builtin, HIGH);
      delay(100);
      digitalWrite(rx_led, HIGH);
      digitalWrite(led_builtin, LOW);
      delay(100);
      digitalWrite(rx_led, LOW);
    }
    
    }
    else {
    // transmit loop
    Serial.print('t');
    digitalWrite(led_builtin, HIGH);
    digitalWrite(tx_led, HIGH);
    delay(100);
    digitalWrite(led_builtin, LOW);
    digitalWrite(tx_led, LOW);
    delay(100);
    }
}
