#include <Servo.h>
#include "Wire.h"
#include "LiquidCrystal.h"
#define S1 A0
#define S2 A1
#define S3 A2
#define S4 A3
#define Servo1 3
#define Servo2 5
LiquidCrystal lcd(0);
/*  1  |  2
 *  ------- 
 *  4  |  3
 */
 
 Servo Baza;
 Servo Cap;

int Svalue[4]={0,0,0,0},medie=0;
int Cap_position=90,Baza_position=90;
int angleLimit1=45,angleLimit2=90;



void Sensor_Read(){
   Svalue[0]=analogRead(S1);
   Svalue[1]=analogRead(S2);
   Svalue[2]=analogRead(S3);
   Svalue[3]=analogRead(S4);
}
void Sensor_State(int &ma,int &a,int &b){
  
   ma=0;
   for(int i=0;i<3;i++)
     for(int j=i+1;j<4;j++){
       int med=(Svalue[i]+Svalue[j])/2;
       if(med>ma){ma=med;a=i;b=j;}
     }
}
void Servo_Write(){
  Sensor_Read();
  //state
for(int i=0;i<4;i++){
Serial.print(i);
Serial.print(". ");
Serial.print(Svalue[i]);
Serial.print(" ");
}
Serial.println();

  for(int i=0;i<4;i++)medie+=Svalue[i];
  medie/=4; 
  Serial.println(medie);
  
  int Sensor[2]={0,0},med=0;
  int TOLERANTA=0;
  Sensor_State(med,Sensor[0],Sensor[1]);
  Serial.print("Senzor 1: ");
  Serial.print(Sensor[0]);
  Serial.print(" Senzor 2: ");
  Serial.print(Sensor[1]);
  Serial.println();
  Serial.println(med);
   if(med!=0){
     //UP LIGHT
  if((Sensor[0]==0 && Sensor[1]==1) || (Sensor[0]==1 && Sensor[1]==0))
  while(abs(med-(Svalue[2]+Svalue[3])/2)>TOLERANTA && Cap_position<=angleLimit1+angleLimit2){
    
    Cap_position++;
    
    Cap.write(Cap_position);
    Sensor_Read();
    Sensor_State(med,Sensor[0],Sensor[1]);
     delay(50);
    }
  //DOWN LIGHT  
    if((Sensor[0]==3 && Sensor[1]==2) || (Sensor[0]==2 && Sensor[1]==3))
  while(abs(med-(Svalue[1]+Svalue[0])/2)>TOLERANTA && Cap_position>=angleLimit1){
    
    Cap_position--;
    Cap.write(Cap_position);
    
    Sensor_Read();
    Sensor_State(med,Sensor[0],Sensor[1]);
    delay(50);
    }
   //LEFT LIGHT
      if((Sensor[0]==0 && Sensor[1]==3) || (Sensor[0]==3 && Sensor[1]==0)){
  while(abs(med-(Svalue[1]+Svalue[2])/2)>TOLERANTA && Baza_position>=5 ){
    if(Cap_position<90)Baza_position++;
    else Baza_position--;
    Baza.write(Baza_position);
    Sensor_Read();
    Sensor_State(med,Sensor[0],Sensor[1]);
     delay(50);
    }
    if(Baza_position==5)Baza.write(90);
    
    }
    //RIGHT LIGHT
      if((Sensor[0]==1 && Sensor[1]==2) || (Sensor[0]==2 && Sensor[1]==1)){
  while(abs(med-(Svalue[0]+Svalue[3])/2)>TOLERANTA && Baza_position<=180){
    if(Cap_position<90)Baza_position--;
    Baza_position++;
    Baza.write(Baza_position);
    Sensor_Read();
    Sensor_State(med,Sensor[0],Sensor[1]);
     delay(50);
    }
    if(Baza_position==5)Baza.write(90);
    }
   }
/**/
//State





delay(500);
}

int SP_value(){
 return analogRead(A6);
}

void LCD_write(int SPanel_value){
     lcd.setCursor(0,1);
     lcd.print("Solar Panel Voltage:");
     lcd.setCursor(7,2);
     if(SPanel_value!=0 && SPanel_value<=535)
     lcd.print((float)SPanel_value/100);
     else lcd.print(4.87);
     lcd.print(" V");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
 pinMode(S1, INPUT);
 pinMode(S2, INPUT);
 pinMode(S3, INPUT);
 pinMode(S4, INPUT);
 pinMode(SP, INPUT);
Baza.attach(Servo1);
Cap.attach(Servo2);
lcd.begin(20,4);
lcd.print("Testing the LCD");
delay(500);
lcd.setCursor(8,1);
lcd.print(".");
delay(500);
lcd.print(".");
delay(500);
lcd.print(".");
delay(1000);
lcd.clear();
lcd.setCursor(3,1);
lcd.print("Testing Done!");
delay(1000);
lcd.clear();
/*Baza.write(0);
delay(2000);
Baza.write(180);
delay(5000);
Cap.write(0);
delay(2000);
Cap.write(180);
delay(5000);*/
Baza.write(90);
Cap.write(90);
delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
Servo_Write();
LCD_write(SP_value());
}
