constexpr int SW_PIN = 2;
constexpr int RED_PIN = 42;
constexpr int YELLOW_PIN = 41;
constexpr int GREEN_PIN = 40;
int count;


void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(SW_PIN, INPUT_PULLUP);
  digitalWrite(RED_PIN,1);
  count = 0;
}


void loop() {
  while (digitalRead(SW_PIN) == 1)
    ;  // รอจนกว่าสวิตช์จะถูกกด
  delay(10);
  count++;
  printf("%d\n", count);
  while (digitalRead(SW_PIN) == 0)
    ;  // รอจนกว่าสวิตช์จะถูกปล่อย
  delay(10);
  switch (count) {
  case 1:
    digitalWrite(GREEN_PIN,0);
    digitalWrite(RED_PIN,1);
    break;
  case 2:
    digitalWrite(RED_PIN,0);
    digitalWrite(YELLOW_PIN,1);
    break;
  case 3:
    digitalWrite(YELLOW_PIN,0);
    digitalWrite(GREEN_PIN,1);
    break;
  default:
    //
    break;
}
  if (count == 3) {
    count = 0;
    printf("%d\n", count);
  }
}
