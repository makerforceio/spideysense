#include <NewPing.h>

#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
#define MAX_HAPTIC 150 // Maximum analogwrite to haptic
#define PERSON_HEIGHT 175 // Height of the person in cm

#define SONAR_1_TRIG 1
#define SONAR_1_ECHO 2

#define SONAR_2_TRIG 3
#define SONAR_2_ECHO 4

#define SONAR_3_TRIG 5
#define SONAR_3_ECHO 6

#define VIBRATOR_MOTOR_PIN_1 9
#define VIBRATOR_MOTOR_PIN_2 10
#define VIBRATOR_MOTOR_PIN_3 11

NewPing sonar[3] = {   // Sensor object array.
  NewPing(SONAR_1_TRIG, SONAR_1_ECHO, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping. 
  NewPing(SONAR_2_TRIG, SONAR_2_ECHO, MAX_DISTANCE), 
  NewPing(SONAR_3_TRIG, SONAR_3_ECHO, MAX_DISTANCE)
};

int haptics[3] = {VIBRATOR_MOTOR_PIN_1, VIBRATOR_MOTOR_PIN_2, VIBRATOR_MOTOR_PIN_3};

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.

  pinMode(VIBRATOR_MOTOR_PIN_1, OUTPUT);
  pinMode(VIBRATOR_MOTOR_PIN_2, OUTPUT);
  pinMode(VIBRATOR_MOTOR_PIN_3, OUTPUT);
}

// Haptic scale is from 0-150, distance measurement is between 0-200cm
void loop() { 
  for (uint8_t i = 0; i < 3; i++) { // Loop through each sensor and display results.
    delay(50); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.

    int dis = sonar[i].ping_cm();
    double out = (dis / MAX_DISTANCE) * MAX_HAPTIC;
    analogWrite(haptics[i], out);
     
    // Debug stuff
    Serial.print(i);
    Serial.print("=");
    Serial.print(sonar[i].ping_cm());
    Serial.print("cm ");
  }
  
  Serial.println();
}
