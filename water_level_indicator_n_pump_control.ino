//water level indicator and pump control
#define trigger 4 //input port for ultrasonic sensor
#define echo 5 //input port to ultrasonic sensor
float time=0,distance=0;
int digitalPin=2;//(floating pin input)connected through 10k resistor(pull down)..
int digitalvalue;
int countlow=0;
int counthigh=0;
int buzzercount=0;
int flag=0;
void setup() 
{
  Serial.begin(9600);
  pinMode(trigger,OUTPUT);//output from arduino and input to ultrasonic sensor 
  pinMode(echo,INPUT);//output from ultrasonic sensor and input to arduino
  pinMode(12,OUTPUT);//relay + for controlling pump power supply
  pinMode(13,OUTPUT);//(yellowLED)+ for indication of full water level(full is assumed 5cm depth from ultrasonic sensor)
  pinMode(10,OUTPUT);//greenLED+ for indication of mid level(depth more than full but not more than assumed depth 15cm from ultrasonic sensor)
  pinMode(9,OUTPUT);//(redLED)+ for indication of low water level (level more than assumed depth)
  pinMode(8,OUTPUT);// for controlled buzzer
  pinMode(11,OUTPUT);//for indicating water flowing during motor operation
  pinMode(2,INPUT);//for digital input indicating water flow in pipe connected to motor
}

void loop() 
{
  digitalWrite(trigger,LOW);
  delayMicroseconds(2);
  digitalWrite(trigger,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger,LOW);
  delayMicroseconds(2);
  time=pulseIn(echo,HIGH);//in microseconds
  distance=340*time/20000;//in cm
  Serial.println("Distance:");
  Serial.print(distance);//serial monitor display
  Serial.println("cm");
digitalvalue = digitalRead(digitalPin);    // read the input pin
  Serial.println("DIGITAL INPUT INTEGER VALUE(0-1):");
  Serial.println(digitalvalue);             // debug value
if(distance<=5&&distance!=0)                       //level high(top)full
{
  digitalWrite(12,LOW); //turning motor off
   digitalWrite(11,LOW);//water flow indication off
   digitalWrite(10,LOW);
   digitalWrite(9,LOW); 
  digitalWrite(13,HIGH);//turning indication(yellow)
    while(buzzercount!=3)//buzzer-light sequence --tu-tu--tat
{
  digitalWrite(8,HIGH);
  delay(50);
  digitalWrite(8,LOW);digitalWrite(13,LOW);delay(30);
   digitalWrite(8,HIGH); digitalWrite(13,HIGH);
 delay(25);
  digitalWrite(8,LOW);digitalWrite(13,LOW);delay(30);
   digitalWrite(8,HIGH);digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(8,LOW);digitalWrite(13,LOW); delay(600); 
  buzzercount++;
  digitalWrite(13,HIGH);
  }
  buzzercount=0;//reset  
  while(distance<5)
  { //for sensing full  water level and displaying in serial monitor
   digitalWrite(8,LOW);//buzzer off
   digitalWrite(trigger,LOW);
  delayMicroseconds(2);
  digitalWrite(trigger,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger,LOW);
  delayMicroseconds(2);
  time=pulseIn(echo,HIGH);//in microseconds
  distance=340*time/20000;//in cm
  Serial.println("Distance:");
  Serial.print(distance);//serial monitor display
  Serial.println("cm");
   }
}
else if(distance>5&&distance<15)                                 //medium level
{
  
  digitalWrite(8,LOW);
   digitalWrite(12,LOW);//relay off->motor off
   // digitalWrite(11,LOW);//water flow indication off
    digitalWrite(9,LOW);
    digitalWrite(13,LOW);
    digitalWrite(10,HIGH);//notify
   
}
else if(distance>=15)                                           //level low(bottom)water almost empty
{ 
 countlow=0;
 counthigh=0; 
  Serial.println("first low catch");
  digitalWrite(13,LOW);
  digitalWrite(10,LOW);
  digitalWrite(9,HIGH);// red led notify
  buzzercount=0;//reset
   while(buzzercount!=2)//buzzersequence tu-tu---tu-tu
{ delay(100);
  digitalWrite(8,HIGH); digitalWrite(9,HIGH);
  delay(100);
  digitalWrite(8,LOW); digitalWrite(9,LOW); delay(1000); 
  buzzercount++;
   digitalWrite(9,HIGH);
  }
  buzzercount=0;//reset  
   digitalWrite(12,HIGH);//relay on->motor on 
  while(distance>5)//for using pump upto full level
  {
    //repeating sensing and displaying in serial monitor

 Serial.println("second low catch");
  digitalWrite(trigger,LOW);
    delayMicroseconds(2);
    digitalWrite(trigger,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger,LOW);
    delayMicroseconds(2);
  time=pulseIn(echo,HIGH);
    distance=340*time/20000;
    Serial.println("READING Distance after second low catch:");
    Serial.print(distance);
    Serial.println("cm");
  digitalvalue = digitalRead(digitalPin);
   Serial.println("Digital value:");
     Serial.println(digitalvalue); 
     delay(120);
  if(digitalvalue==0)
  {
  countlow++;
  }
  else
  {
    while(digitalvalue==1)
    {counthigh++;
     Serial.println("continuous water flow through pipe");
           digitalvalue = digitalRead(digitalPin);
            Serial.println("Digital value:");
            Serial.println(digitalvalue);
             if(counthigh==15){
              digitalWrite(11,HIGH);
              counthigh=0;
                break;}
 
      }counthigh=0;
    }
   
if(distance>5&&distance<15)//case in the mid level
      {
       digitalWrite(10,HIGH); 
        digitalWrite(9,LOW);
         if(countlow==15)//critical case when motor is on but no water pumped
           {
                digitalWrite(11,LOW);//water flow indication off
                digitalWrite(12,LOW);//turning motor off from unnecessary usage
                while(buzzercount!=6)//buzzer-led sequence tu---tu---tut
              {  digitalWrite(8,HIGH);digitalWrite(9,HIGH);
                delay(200);
                digitalWrite(8,LOW);digitalWrite(9,LOW); delay(100);
                digitalWrite(8,HIGH);digitalWrite(9,HIGH);
                delay(100);
                digitalWrite(8,LOW);digitalWrite(9,LOW); delay(200);
                digitalWrite(8,HIGH);digitalWrite(9,HIGH);
                delay(50);
                digitalWrite(8,LOW); digitalWrite(9,LOW);delay(600);
                buzzercount++;
                digitalWrite(9,HIGH);
                }
               digitalWrite(9,LOW);
                buzzercount=0;//reseting from beginning 
               counthigh=0;//reseting from beginning 
                countlow=0;
              }
}
if(distance>15)
{
  digitalWrite(10,LOW);    
  digitalWrite(9,HIGH); 
} 
  if(countlow==15)//critical case when motor is ON but no water pumped
{
  digitalWrite(10,LOW);
   digitalWrite(11,LOW);//water flow indication off 
  digitalWrite(12,LOW);//turning motor off from unnecessary usage*/
  
  
 while(buzzercount!=5)//buzzer-led sequence tu---tu---tut
{  digitalWrite(8,HIGH);digitalWrite(9,HIGH);
  delay(200);
  digitalWrite(8,LOW);digitalWrite(9,LOW); delay(100);
   digitalWrite(8,HIGH);digitalWrite(9,HIGH);
 delay(100);
  digitalWrite(8,LOW);digitalWrite(9,LOW); delay(200);
   digitalWrite(8,HIGH);digitalWrite(9,HIGH);
  delay(50);
  digitalWrite(8,LOW); digitalWrite(9,LOW);delay(600);
  buzzercount++;
  digitalWrite(9,HIGH);
  } 
  buzzercount=0;     // reseting from beginning 
  countlow=0;
  counthigh=0;//staring from beginning
}
}
}
delay(50);
}
