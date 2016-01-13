//BridgeBlink.ino
#include <Bridge.h>

char D13char[1024];
int D13int = 0;

void setup() {
  // put your setup code here, to run once:
  Bridge.begin();
  
  Serial.begin(9600);
 
  //Console.println("You're connected to the Console!!!");
}

void loop() {
  // put your main code here, to run repeatedly:
  Bridge.get("to_arduino",D13char,1024);
  Serial.println("Test");
  //Console.println(D13char);
  Serial.println(D13char);
  delay(50);
}


