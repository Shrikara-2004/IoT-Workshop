int touch = 7;
int led = LED_BUILTIN;
void setup() {
pinMode(touch, INPUT);
pinMode(led, OUTPUT);
}

void loop() {
int data = digitalRead(touch);
Serial.println(data);
if(data == 1){
digitalWrite(led, HIGH);
delay(500);

}
else{
digitalWrite(led, LOW);
delay(500);

}
}
