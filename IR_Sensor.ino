int irsensor = A0;
void setup() {
  // put your setup code here, to run once:
pinMode(irsensor, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
int data = digitalRead(irsensor);
Serial.println(data);
delay(1000);
}
