#include <Servo.h> 
Servo myservoTurn;

#define numOfValsRec 2
#define digitsPerValRec 4

int valsRec[numOfValsRec];
int stringLength = numOfValsRec*digitsPerValRec + 1;
int counter = 0;
bool counterStart = false;
String receivedString;

int myPins[3] = {2,4,5}; // Pin4 direction; Pin5 PWM
//const int Forward = 1;
//const int Backward = 0;

void moveRobot(int mySpeed, int myTurn, int maxSpeed = 255, int maxTurn=90)
{
  /*
  *mySpeed and myTurn range from -100 to 100
  *+ val of mySpeed is forward and - is backward
  *+val of myTurn is right turn and - is left turn
  */
  mySpeed = constrain(mySpeed, -100, 100);
  myTurn = constrain(myTurn, -100, 100);
  mySpeed = map(mySpeed, -100,100,-maxSpeed, maxSpeed);
  myTurn = map(myTurn, -100,100,45-maxTurn, 45+maxTurn);
  //mySpeed = constrain(mySpeed, -45, 45);
  //myTurn = constrain(myTurn, 0, 90);
  
  if (mySpeed >= 0){
    digitalWrite(myPins[0],LOW);
    digitalWrite(myPins[1],HIGH);
   }
  else{
    digitalWrite(myPins[0],HIGH);
    digitalWrite(myPins[1],LOW);
   }
  analogWrite(myPins[2],abs(mySpeed));
  myservoTurn.write(myTurn); 
}

void receiveData()
{
  while(Serial.available()) {
    char c = Serial.read();
    if (c=='$'){
      counterStart = true;
    }
    if(counterStart){
      if(counter<stringLength){
        receivedString = String(receivedString+c);
        counter++;
      }
      if(counter>=stringLength){
        for(int i=0; i<numOfValsRec; i++)
        {
          int num = (i*digitsPerValRec) +1;
          valsRec[i]=receivedString.substring(num,num+digitsPerValRec).toInt(); 
        }
        receivedString = "";
        counter = 0;
        counterStart = false;
      }
    }
  }
}

void setup(){
  for (int i=0; i<3; i++) {
    pinMode(myPins[i], OUTPUT);
  }
  pinMode(13, OUTPUT);
  Serial.begin(115200) ;
  myservoTurn.attach(9);
  moveRobot(0,0);
  
}

void loop() {
  receiveData();
  moveRobot(valsRec[0],valsRec[1]);
  delay(40);
  moveRobot(0,0);
  
}
