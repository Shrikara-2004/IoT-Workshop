int led = LED_BUILTIN;

void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT); //explaining it takes 2 parameters

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite (led, HIGH);
  delay(500);
  digitalWrite (led, LOW);
  delay(500);

}
