#include <LiquidCrystal.h>

unsigned long lastRandomTime = 0;
float randomOffset = 0.0;
float RLDR;                   
float Vout; 
float lightValue;                  
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int sensor=A0;
int soil;

void setup() {
  Serial.begin(9600);
  pinMode(6,INPUT); // pir sensor output pin connected  
  pinMode(7,OUTPUT);  
  pinMode(5,OUTPUT);   
  digitalWrite(7,HIGH);  
  Serial.println("Serial begin");
  lcd.begin(16, 2);  
}

void loop() {
  // Light Intensity
  int sensorValue = analogRead(A1);
  Vout = (sensorValue * 0.0048828125); 
  RLDR = (10000.0 * (6 - Vout))/Vout;     
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LIGHT INTENSITY : ");
  delay(100);
  lcd.setCursor(0, 1);
   if (millis() - lastRandomTime >= 10000) {
    // generate a new random value
    randomOffset = random(-5, 5) / 100.0;
    lastRandomTime = millis();
  }
  
  // calculate the light value with the random offset
  lightValue = (RLDR / 500) + randomOffset;
  lightValue = constrain(lightValue, 0, 100); // make sure the value is within 0-100%
  lcd.print(lightValue, 1);
  lcd.setCursor(4, 1);
  lcd.print("%");
  if (lightValue >= 0 && lightValue <= 25)
  {
    lcd.print(" LOW"); 
  }
  else if (lightValue > 25 && lightValue <= 35)
  {
    lcd.print(" MEDIUM "); 
  }
  else
  {
    lcd.print(" GREAT"); 
  }

  // Soil Moisture
  soil = analogRead(sensor);
  Serial.println(String("Soil Val: ") + soil);
  soil=map(soil, 1023, 650, 0,100);
  Serial.println(String(" Value after mapped: ")+soil);
  if(soil>=120)
  {
    Serial.println("Sufficient water for the plant");
    digitalWrite(7, LOW);
    digitalWrite(5, HIGH);
  }
  else if(soil <= 100)
  {
    Serial.println("water level is low");
    digitalWrite(7, HIGH);
    digitalWrite(5, HIGH);
  }
  else
  {
    Serial.println("Moisture level is OK");
  }

  delay(700);
}