
int sensorPin = 36; //A0 FOR ARDUINO/ 36 FOR ESP

void setup()
{ 
  Serial.begin(9600);
}
void loop() {
  int sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  int turbidity = map(sensorValue, 0, 750, 100, 0);
  delay(100);
  Serial.println(turbidity);
  delay(1000);
  if (turbidity < 20) {
    Serial.println(" its CLEAR ");
  }
  if ((turbidity > 20) && (turbidity < 50)) {
    Serial.println(" its CLOUDY ");
  }
  if (turbidity > 50) {
    Serial.println(" its DIRTY ");
  }
}