
int flamePin = 2;
int flame_detected=0;

void setup() {
  // put your setup code here, to run once:
  pinMode(flamePin,INPUT);
  Serial.begin(9600);

}

void loop() {
    Serial.println("test");
  delay(1000);

} 
