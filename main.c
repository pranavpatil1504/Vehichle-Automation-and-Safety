#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
#include <TimerOne.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int SW_1 = A0; 
const int SW_2 = A1; 
const int SW_3 = A2; 
const int SW_4 = A3;
const int Buzzer = 7;
const int IR_Sensor = 4;
int SW_1_State = 0; 
int SW_2_State = 0; 
int SW_3_State = 0; 
int SW_4_State = 0; 
int Current_Temp=0;
int System_Mode=0;
int Buzzer_Mode=0;
int Object_Detect=0;
const int trig_1 =6;
const int echo_1 =5;
long duration, distance; 
int ultra_sense = 0;
int ultra_count= 0;
int Final_Distance=0;
int count_update=0;

int distance_1=0;
void setup() {
  Serial.begin(9600);
  pinMode(SW_1, INPUT);
  pinMode(SW_2, INPUT);
  pinMode(SW_3, INPUT);
  pinMode(SW_4, INPUT);
  digitalWrite(SW_1, HIGH);
  digitalWrite(SW_2, HIGH);
  digitalWrite(SW_3, HIGH);
  digitalWrite(SW_4, HIGH);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, HIGH);
  sensors.begin();
  lcd.begin(16, 2); // Provide the number of columns and rows here
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" Sensor System");
  lcd.setCursor(0, 1);
  lcd.print(" ");
  lcd.clear();
  delay(2000);

  pinMode(trig_1, OUTPUT);
  pinMode(echo_1, INPUT);
  pinMode(IR_Sensor, INPUT);
  digitalWrite(IR_Sensor, HIGH);
}

void loop()
{
 Read_Switch();
 LCD_Update();
 
}
void Read_Switch()
{
 SW_1_State = !digitalRead(SW_1);
 SW_2_State = !digitalRead(SW_2);
 SW_3_State = !digitalRead(SW_3);
 SW_4_State = !digitalRead(SW_4);
 
 Serial.print(SW_1_State);
 Serial.print(SW_2_State);
 Serial.print(SW_3_State);
 Serial.println(SW_4_State);

 if(SW_1_State==1 && SW_2_State==0 && SW_3_State==0)System_Mode=1;
 if(SW_1_State==0 && SW_2_State==1 && SW_3_State==0)System_Mode=2;
 if(SW_1_State==0 && SW_2_State==0 && SW_3_State==1)System_Mode=3;
 
 if(SW_4_State==1 && Buzzer_Mode==0){Buzzer_Mode=1;delay(50);}
 else if(SW_4_State==1 && Buzzer_Mode==1){Buzzer_Mode=0;delay(50);}
}
void LCD_Update()
{
 
 lcd.clear();
 lcd.setCursor(0, 0);
 //lcd.print("Mode: ");
 if(System_Mode==1)
 {
 lcd.print("Temp Sensor ");
 Temp_Read();
 lcd.setCursor(0, 1);
 lcd.print("Temp: ");
 lcd.print(Current_Temp);
 lcd.write(223);
 lcd.print('C');
 }
 else if(System_Mode==2)
 {
 lcd.print(" Ultrasonics ");
 Read_Ultrasonics();
 lcd.setCursor(0, 1);
 lcd.print("Distance: ");
 lcd.print(distance_1);
 lcd.print("cm");
 }
 else if(System_Mode==3)
 {
 lcd.print("Object Sensor ");
 Read_IR();
 lcd.setCursor(0, 1);
 if(Object_Detect==0)lcd.print("No Object");
 if(Object_Detect==1)lcd.print("Object Detect");
 
 
 }
 else lcd.print("Select Mode ");
 if(System_Mode>1)
 {
 lcd.setCursor(15, 1);
 if(Buzzer_Mode==1)lcd.print('B');
else lcd.print('-');
 }
 delay(1000);
}
void Read_IR()
{
 Object_Detect = !digitalRead(IR_Sensor);
 
 if(Object_Detect==1 && Buzzer_Mode==1)digitalWrite(Buzzer, 0);
 else digitalWrite(Buzzer, HIGH);
}
void Read_Ultrasonics()
{
 ultra_count=0;
 Final_Distance=0;
 while(ultra_count!=10)
 {
 delay(10);
 ultra_count++;
 digitalWrite(trig_1, LOW);
 delayMicroseconds(2);
 
 digitalWrite(trig_1, HIGH);
 delayMicroseconds(10);
 
 digitalWrite(trig_1, LOW);
 duration = pulseIn(echo_1, HIGH);
 distance = duration / 58.2;
 Final_Distance=Final_Distance+distance;
 }
 
 Final_Distance=Final_Distance/10;
 
 if(Final_Distance>200)Final_Distance=200; 
 
 distance_1=Final_Distance;
 
 Serial.print("distance_1 ");
 Serial.println(distance_1);
//
 if(distance_1<=30 && Buzzer_Mode==1)digitalWrite(Buzzer, 0);
 else digitalWrite(Buzzer, HIGH);
}
void Temp_Read()
{
 Serial.println("Sensor...");
 sensors.requestTemperatures();
Current_Temp=(sensors.getTempCByIndex(0));
}
