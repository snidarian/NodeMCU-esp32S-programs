
const int led_builtin = 2; // builtin NodeMCU LED
int incoming_byte;


void setup() {
  Serial.begin(9600);
  // builtin led to signal that reciever is recieving
  pinMode(led_builtin, OUTPUT);

}

void loop() {
  // reciever loop

  // See if there's incoming serial data
  if (Serial.available() > 0) {
    // read oldest byte in serial buffer
    incoming_byte = Serial.read();

    if (incoming_byte == 'O') {
      digitalWrite(led_builtin, HIGH);
    }
    if (incoming_byte == 'K') {
      digitalWrite(led_builtin, LOW);
    }
    
    }

}
