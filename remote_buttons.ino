#include <IRremote.h>  //including infrared remote header file
int RECV_PIN = 12;     // the pin where you connect the output pin of IR sensor
IRrecv irrecv(RECV_PIN);
decode_results results;
void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(13, OUTPUT);
}
void loop() {

  digitalWrite(13, HIGH);
  if (irrecv.decode(&results))  // Returns 0 if no data ready, 1 if data ready.
  {
    int readResults = results.value;  // Results of decoding are stored in result.value
    Serial.println(" ");
    Serial.print("Code: ");
    Serial.println(results.value, HEX);  //prints the value a a button press
    Serial.println(" ");
    irrecv.resume();  // Restart the ISR state machine and Receive the next value
  }
}