int ldr = 4;
int red = 42;
int yellow = 41;
int green = 40;
int sw = 2;
int num;
//
void setup() {
  analogSetAttenuation(ADC_11db);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(2,INPUT_PULLUP);
  num = 1;
}
void loop() {
  int sw = digitalRead(2);
  int light = analogRead(ldr);
  //if(sw==0){
  //  delay(300);
  //  num++;
  //}
  // put your main code here, to run repeatedly:
  printf("%d\n",light);
  //printf("%d\n", sw);
  //printf("%d\n", num);
  delay(100);
  //if(num%2==0){
  //  digitalWrite(red,0);
  //  digitalWrite(yellow,0);
  //  digitalWrite(green,0);
  //}
  //else{
  if (light<=600){
    analogWrite(green, 255);
    analogWrite(red, 0);
    analogWrite(yellow, 0);
  }
  else if (light > 600 && light <= 1200){
    analogWrite(green, 127);
    analogWrite(red, 0);
    analogWrite(yellow, 0);
  }
  else if (light > 1200 && light <= 1800){
    analogWrite(yellow, 255);
    analogWrite(red, 0);
    analogWrite(green, 0);
  }
  else if (light > 1800 && light <= 2400){
    analogWrite(yellow, 127);
    analogWrite(red, 0);
    analogWrite(green, 0);
  }
  else if (light > 2400 && light <= 3000){
    analogWrite(red, 255);
    analogWrite(yellow, 0);
    analogWrite(green, 0);
  }
  else {
    analogWrite(red, 127);
    analogWrite(yellow, 0);
    analogWrite(green, 0);
  }
}
  //switch(light){
  //  case light>3000:
  //    analogWrite(green, 255);
  //  case 2400<light<3000:
  //    analogWrite(green, 127);
  //  case 1800<light<2400:
  //    analogWrite(yellow, 255);
  //  case 1200<light<1800:
  //    analogWrite(yellow, 127);
  //  case 600<light<1200:
  //    analogWrite(red, 255);
  //  case light<600:
  //   analogWrite(red, 127); 
  //  }
  //}

