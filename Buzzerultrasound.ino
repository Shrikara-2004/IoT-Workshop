#define BUZZER_PIN 14  // GPIO14 for buzzer
#define TRIG_PIN 13    // GPIO13 for HC-SR04 Trigger
#define ECHO_PIN 12    // GPIO12 for HC-SR04 Echo

long duration;
int distance;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(115200);
}

void loop() {
  // Send a 10us pulse to trigger pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the echo duration
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate distance in cm
  distance = duration * 0.034 / 2;

  // Display the distance
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // If object is closer than 10 cm, beep
  if (distance > 0 && distance <= 10) {
    tone(BUZZER_PIN, 500); // 500 Hz sound
    delay(100);
    noTone(BUZZER_PIN);
    delay(100);
  } else {
    noTone(BUZZER_PIN); // Make sure buzzer is off
  }

  delay(200); // Small delay before next reading
}
