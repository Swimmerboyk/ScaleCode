// Working Rocket Scale Code
// Uplaoded: 7-28-2023 By: Kyle Rock
#include <Q2HX711.h>

const byte hx711_data_pin = 3;
const byte hx711_clock_pin = 4;

float y1 = 15.0; // calibrated mass to be added
long x1 = 0L;
long x0 = 0L;
float avg_size = 1.0; // amount of averages for each mass measurement (also speeds up sensor data intake by making the amount smaller)
int count = 0;
Q2HX711 hx711(hx711_data_pin, hx711_clock_pin); // prep hx711

void setup() {
  Serial.begin(9600); // prepare serial port
  delay(1000); // allow load cell and hx711 to settle
  // Tare procedure
  for (int ii = 0; ii < int(avg_size); ii++) {
    delay(10);
    x0 += hx711.read();
  }
  x0 /= long(avg_size);
  Serial.println("Add Calibrated Mass");
  // Calibration procedure (mass should be added equal to y1)
  int ii = 1;
  while(true){
    if (hx711.read()<x0+10000){
    } else {
      ii++;
      delay(2000);
      for (int jj=0;jj<int(avg_size);jj++){
        x1+=hx711.read();
      }
      x1/=long(avg_size);
      break;
    }
  }
  Serial.println("Calibration Complete");
  Serial.println("Press Enter or space to start:");
}

void loop() {
  if (Serial.available() > 0) {
    // Read the input character
    char input = Serial.read();

    // Print the received character
    Serial.print("Input Received: ");
    Serial.println("Start");

    while (count < 201){
      long reading = 0;
      for (int jj=0;jj<int(avg_size);jj++){
        reading+=hx711.read();
      }
      reading /= long(avg_size);
      if (count < 320) {
        // calculating mass based on calibration and linear fit
        float ratio_1 = (float)(reading - x0);
        float ratio_2 = (float)(x1 - x0);
        float ratio = ratio_1 / ratio_2;
        float mass = y1 * ratio;
        count = count + 1;
        Serial.print(count);
        Serial.print(" | Weight(lbs): ");
        Serial.println(mass);
      }
      else{
        Serial.println("DONE");
        count = count + 1;
        break;
      }
      delay(13); // Delay for 10 milliseconds (100 times per second)  
    }
  }
  
}
