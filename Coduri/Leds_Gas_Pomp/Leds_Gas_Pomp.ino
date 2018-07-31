#define Water_pin A1
#define Gas_pin A0
#define Fan_pin 6
#define Pomp_pin 7
int Led_pin[4]={2,5,4,3};
bool Led_stat[4]={false,false,false,false};

void setup() {
           pinMode(Water_pin, INPUT);
           pinMode(Gas_pin, INPUT);
           pinMode(Fan_pin, OUTPUT);
           pinMode(Pomp_pin,OUTPUT);
           
           for(int i=0;i<4;i++)
             pinMode(Led_pin[i], OUTPUT);

            Serial.begin(9600);
            
            for(int i=0;i<4;i++)
               {digitalWrite(Led_pin[i], HIGH);
               delay(1000);}
            for(int i=0;i<4;i++)
               {digitalWrite(Led_pin[i], LOW);
               delay(1000);}
               
}
void BT(){
     
    if(Serial.available()>0)
      {
        char BT_state=Serial.read();
        switch(BT_state){
          case '1':Led_stat[0]=!Led_stat[0];break;
          case '2':Led_stat[1]=!Led_stat[1];break;
          case '3':Led_stat[2]=!Led_stat[2];break;
          case '4':Led_stat[3]=!Led_stat[3];break;          
        }
        for(int i=0;i<4;i++)digitalWrite(Led_pin[i],Led_stat[i]);
      }
}

void Gas(){
    int Gas_state=analogRead(Gas_pin);
    int Water_state=analogRead(Water_pin);
    if(Gas_state<200 && Water_state==1023)digitalWrite(Fan_pin,HIGH);
    else 
    if(Gas_state>400)digitalWrite(Fan_pin, LOW);
    else digitalWrite(Fan_pin, HIGH);
    Serial.print("Gas value: ");
    Serial.println(Gas_state);
}
void Pomp(){
   int Water_state=analogRead(Water_pin);
   if(Water_state!=1023)digitalWrite(Pomp_pin, HIGH);
   else digitalWrite(Pomp_pin, LOW);
   Serial.print("Water value: ");
   Serial.println(Water_state);
}
void loop() {
  BT();
  Gas();
  Pomp();
delay(200);
}
