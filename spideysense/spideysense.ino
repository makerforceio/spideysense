#include <NewPing.h>

#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
#define MAX_DISTANCE_HEIGHT 400
#define MAX_HAPTIC 150 // Maximum analogwrite to haptic
#define EXPECTED_HEIGHT 200 // Height of the person in cm

#define SONAR_1_TRIG 7
#define SONAR_1_ECHO 8

#define SONAR_2_TRIG 5
#define SONAR_2_ECHO 6

#define SONAR_3_TRIG 3
#define SONAR_3_ECHO 4

#define VIBRATOR_MOTOR_PIN_1 11
#define VIBRATOR_MOTOR_PIN_2 10
#define VIBRATOR_MOTOR_PIN_3 9

#define PULSE_SHORTEST 1
#define PULSE_LONGEST 10

NewPing sonar[3] = {   // Sensor object array.
  NewPing(SONAR_1_TRIG, SONAR_1_ECHO, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping. 
  NewPing(SONAR_2_TRIG, SONAR_2_ECHO, MAX_DISTANCE_HEIGHT), 
  NewPing(SONAR_3_TRIG, SONAR_3_ECHO, MAX_DISTANCE)
};

int haptics[3] = {VIBRATOR_MOTOR_PIN_1, VIBRATOR_MOTOR_PIN_2, VIBRATOR_MOTOR_PIN_3};

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.

  pinMode(VIBRATOR_MOTOR_PIN_1, OUTPUT);
  pinMode(VIBRATOR_MOTOR_PIN_2, OUTPUT);
  pinMode(VIBRATOR_MOTOR_PIN_3, OUTPUT);
}

// Freq from 4000 - 50000
long scheduleCount = 0L;
int sampleFreq = 5;
int pulseFreq[3] = {0, 0, 0};

// Haptic scale is from 0-150, distance measurement is between 0-200cm
void loop() { 
  if(scheduleCount % sampleFreq == 0) {
    for (uint8_t i = 0; i < 3; i++) { // Loop through each sensor and display results.
      int dis = sonar[i].ping_cm();

      if (!sonar[i].check_timer()) {
        pulseFreq[i] = 0;
      } else {
        if (i == 1) {
          // Special logic for downward facing distance sensor, from 0-200
          int spill = abs(EXPECTED_HEIGHT - dis);
          pulseFreq[i] = map(dis, 0, MAX_DISTANCE, PULSE_SHORTEST, PULSE_LONGEST);
        } else {
          pulseFreq[i] = map(dis, 0, MAX_DISTANCE, PULSE_SHORTEST, PULSE_LONGEST);
        }
      }
      
      // Debug stuff
      Serial.print(i);
      Serial.print("=");
      Serial.print(pulseFreq[i]);
      Serial.print("pulses ");
    }

    Serial.println();
  }

  for (uint8_t j = 0; j < 3; j++) {
    if ((pulseFreq[j] != 0) && (scheduleCount % pulseFreq[j] == 0)) {
      analogWrite(haptics[j], 150);
    }
  }
  
  scheduleCount++;
  delay(100);
  analogWrite(haptics[0], 0);
  analogWrite(haptics[1], 0);
  analogWrite(haptics[2], 0);
}
