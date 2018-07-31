#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo Door;

#define but1 A0
#define but2 A1
#define but3 A2
#define but4 A3
#define but5 A4
#define but6 A5
#define but7 A6
#define but8 A7
#define but9 2
#define but0 3
#define butOK 4
#define butNOK 5
#define ledA 8
#define ledF 7

const int passLen=6;
int Pass[passLen]={0,2,2,4,0,2},chgPass[4]={10,2,4,10};

int butCheck(){
  if(analogRead(but1)>50) return 1;
  else  if(analogRead(but2)>50) return 2;
  else  if(analogRead(but3)>50) return 3;
  else  if(analogRead(but4)>50) return 4;
  else  if(analogRead(but5)>50) return 5;
  else  if(analogRead(but6)>50) return 6;
  else  if(analogRead(but7)>50) return 7;
  else  if(analogRead(but8)>50) return 8;
  else  if(digitalRead(but9)==1) return 9;
  else  if(digitalRead(but0)==1) return 0;
  else  if(digitalRead(butOK)==1) return 10;
  else  if(digitalRead(butNOK)==1) return 11;
  else return -1;
}



void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 pinMode(but1,INPUT);
 pinMode(but2,INPUT);
 pinMode(but3,INPUT);
 pinMode(but4,INPUT);
 pinMode(but5,INPUT);
 pinMode(but6,INPUT);
 pinMode(but7,INPUT);
 pinMode(but8,INPUT);
 pinMode(but9,INPUT);
 pinMode(but0,INPUT);
 pinMode(butOK,INPUT);
 pinMode(butNOK,INPUT);
 pinMode(ledA,OUTPUT);
 pinMode(ledF, OUTPUT);
 Door.attach(6);
 Door.write(0);
 SPI.begin();  
  mfrc522.PCD_Init();
}

void loop() {
  
//------------------KeyCode Part----------------------------------------------------------------------------
    
    digitalWrite(ledA,LOW);
    digitalWrite(ledF,LOW);
    Door.write(0);
    int but=butCheck();
 if(but!= -1)Serial.println(but);
 if(but==Pass[0]){
   delay(500);
  for(int i=1;i<passLen;i++){
    while(butCheck()==-1);
    but=butCheck();Serial.println(but);
    if(but!=Pass[i]){Serial.println("Password Wrong!");digitalWrite(ledF,HIGH);delay(1000);return;}
    delay(185);
    }
  Serial.println("Password Correct!");
  digitalWrite(ledA, HIGH);
  Door.write(90);
  delay(1000);
  }


else if(but==chgPass[0]){
   delay(500);
  for(int i=1;i<4;i++){
    while(butCheck()==-1);
    but=butCheck();Serial.println(but);
    if(but!=chgPass[i]){Serial.println("Special Key gone wrong!");digitalWrite(ledF,HIGH);delay(1000);return;}
    delay(185);
    }
  Serial.println("Password Correct!");
  //blink mode
  digitalWrite(ledA, HIGH);
  digitalWrite(ledF, HIGH);
  delay(500);
  digitalWrite(ledA, LOW);
  digitalWrite(ledF, LOW);
  delay(500);
   digitalWrite(ledA, HIGH);
   delay(300);
    digitalWrite(ledA, LOW);
  delay(500);
  digitalWrite(ledF, HIGH);
  delay(300);
  digitalWrite(ledF, LOW);
  //Blink mode Done
//Ender Pass
     for(int i=0;i<passLen;i++){
      digitalWrite(ledA, LOW);
     while(butCheck()==-1);
    but=butCheck();Serial.println(but);digitalWrite(ledA,HIGH);
    Pass[i]=but;
    delay(185);
    }
//Enter Pass end
//end of Paas
    for(int i=0;i<5;i++){
      digitalWrite(ledA, HIGH);
      delay(300);
      digitalWrite(ledA,LOW);
      delay(400);
      digitalWrite(ledF, HIGH);
      delay(300);
      digitalWrite(ledF,LOW);
    }
//end of pass end
  }
  else if(but!=-1){digitalWrite(ledF,HIGH);delay(1000);}
  delay(175);

  
//---------------------RFID Part----------------------------------------------------------------------------


  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }

  
  
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  //making the string of the code got from the reader.
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  //making the vector of the cards.
  int numCards=2;
  String cards[numCards]={"92 FE FC 27","CB EB 20 D9"};
  
  int ok=-1;
  
  for(int i=0;i<numCards;i++)
  if (content.substring(1) == cards[i]) //change here the UID of the card/cards that you want to give access
           ok=i;
     Serial.println(ok);
if (ok!=-1){Serial.println("Authorized access");
    Serial.println();
    digitalWrite(ledA,HIGH);
    Door.write(90);
    delay(1000);
    }
    
    else{
        Serial.println(" Access denied");
        digitalWrite(ledF,HIGH);
       delay(1000);}
           }
