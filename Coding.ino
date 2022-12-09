#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define LIGHTDISTANCE  28.0
#define LIGHTMAX       255.0
#define LIGHTMIN       0.0

#define PIN            10
#define NUMPIXELS      34

#define Trig           13 
#define Echo           12 

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

float cm0;
float cm;
float temp = 0;
float ledValue;
float readValue;
int j=0;
int light = 0;
int servopin = 3;
const int motor = A0;
const int rotate = A2;

void servopulse(int angle){
  for(int i = 0; i<50; i++){
 int pulsewidth=(angle*11)+500;  
 digitalWrite(servopin,HIGH);    
 delayMicroseconds(pulsewidth);  
 digitalWrite(servopin,LOW);
 delayMicroseconds(20000-pulsewidth);
  }   
}    

float clean(float num){
  float a;
  if(num > 900.0){
  a = 2.5;
  }
  
  else if(num >= LIGHTDISTANCE-0.6){
  a = LIGHTDISTANCE;
  }

  else{
  a = num;
  }
  return a;
}

float mapclean(float input1, float range1start, float range1end, float range2start, float range2end){
  float output1 = (input1-range1start)/(range1end - range1start)*(range2end-range2start) + range2start;
  if(output1 < range2end){
    output1 = range2end;
  }
  else if(output1 > range2start){
    output1 = range2start;
  }
  return output1;
}

void setup(){
  Serial.begin(9600);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(motor, OUTPUT);
  pinMode(servopin, OUTPUT);
  pinMode(10,OUTPUT);
  pixels.begin(); // This initializes the NeoPixel library.
}


void loop(){
  digitalWrite(Trig, LOW); 
  delayMicroseconds(2);    
  digitalWrite(Trig,HIGH); 
  delayMicroseconds(10);   
  digitalWrite(Trig, LOW); 
  temp = float(pulseIn(Echo, HIGH)); 
  cm0 = (temp * 17 )/1000;
  
  if(j == 0){
  cm = clean(cm0);
  }
  else if((j > 0) && (clean(cm0) != 2.5)){
  cm = (clean(cm) + clean(cm0))/2;
  }
  else if(clean(cm0) == 2.5){
  cm = cm;    
  }
 


  light = mapclean(cm, 2.5, LIGHTDISTANCE, LIGHTMAX, LIGHTMIN);;
  readValue = analogRead(rotate);
  float lightr = mapclean(light, LIGHTMAX, LIGHTMIN, 1.00, 0.00);
  
  ledValue = map(readValue, 22, 1021, 0, 180);
  

  servopulse(ledValue);
  Serial.println(ledValue);

  digitalWrite(10, HIGH);
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(lightr*255,lightr*220,lightr*127)); 
    pixels.show(); 
  }
  
  j++;
  delay(1);
}
