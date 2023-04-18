#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#define BMP280_ADDRESS 0x76
Adafruit_BMP280 bmp;

uint16_t dig_T1;
short dig_T2;
short dig_T3;
uint16_t dig_P1;
short  dig_P2;
short  dig_P3;
short  dig_P4;
short  dig_P5;
short  dig_P6;
short  dig_P7;
short  dig_P8;
short  dig_P9;
double alt;

#define CALIB_DIG_P1_REG 0x8E


void setup() {
  Serial.begin(9600);
 if (!bmp.begin()) {
     Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
     while (1);
   }
  Wire.begin();
  
  uint8_t buffer[24];

  // Read calibration coefficients from non-volatile memory
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(0x88);
  Wire.endTransmission(false);
  Wire.requestFrom(BMP280_ADDRESS, 24);
  for (int i = 0; i < 24; i++) {
    buffer[i] = Wire.read();
  }


  // Parse calibration coefficients
  dig_T1 = buffer[1] << 8 | buffer[0];
  dig_T2 = buffer[3] << 8 | buffer[2];
  dig_T3 = buffer[5] << 8 | buffer[4];
  dig_P1 = buffer[7] << 8 | buffer[6];
  dig_P2 = buffer[9] << 8 | buffer[8];
  dig_P3 = buffer[11] << 8 | buffer[10];
  dig_P4 = buffer[13] << 8 | buffer[12];
  dig_P5 = buffer[15] << 8 | buffer[14];
  dig_P6 = buffer[17] << 8 | buffer[16];
  dig_P7 = buffer[19] << 8 | buffer[18];
  dig_P8 = buffer[21] << 8 | buffer[20];
  dig_P9 = buffer[23] << 8 | buffer[22];

  // Print calibration coefficients to serial monitor
  Serial.print("dig_T1: ");
  Serial.println(dig_T1);
  
  Serial.print("dig_T2: ");
  Serial.println(dig_T2);

  Serial.print("dig_T3: ");
  Serial.println(dig_T3);

  Serial.print("dig_P1: ");
  Serial.println(dig_P1);
  
  Serial.print("dig_P2: ");
  Serial.println(dig_P2);

  Serial.print("dig_P3: ");
  Serial.println(dig_P3);

  Serial.print("dig_P4: ");
  Serial.println(dig_P4);
  
  Serial.print("dig_P5: ");
  Serial.println(dig_P5);

  Serial.print("dig_P6: ");
  Serial.println(dig_P6);

  Serial.print("dig_P7: ");
  Serial.println(dig_P7);
  
  Serial.print("dig_P8: ");
  Serial.println(dig_P8);

  Serial.print("dig_P9: ");
  Serial.println(dig_P9);

    
}

void Epogee(double newAlt)
{
    if(newAlt>alt)
    {
        Serial.print("Epogee: ");
        Serial.println(newAlt);
        alt=newAlt;
    }
    else {
      Serial.print("Epogee: ");
      Serial.println(alt);
    }
}

void Liftoff(double newAlt, double newPress)
{
    if(newAlt>alt)
    {
      double diff=newAlt-alt;
      if(diff>-1 && diff<1)
      {
         goto stat;
      }
      Serial.println("Lift Off");
      alt=newAlt;
    }
    else if(newAlt<alt)
    {
      double diff=newAlt-alt;
      if(diff>-1 && diff<1)
      {
         goto stat;
      }
        Serial.print("Descent");
        alt=newAlt;
    }
    else {
      goto stat;
    }
    stat:
      Serial.print("Static\n");
}


void loop() {
  
  Serial.print(F("Pressure = ")); 
  Serial.print(bmp.readPressure()); 
  Serial.println(" Pa"); 
  Serial.print(F("altitude = "));
  float press=bmp.readPressure();
  float alt=(101325-press)/10.333187;
  Serial.print(alt);
  Serial.println(" m");
  Liftoff(alt,press);
  Epogee(alt);
  Serial.print("Time:");
  Serial.println(millis()/1000);   
  delay(1000); 
}

