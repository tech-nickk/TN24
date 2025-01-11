#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "expressions.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define TAP_THRESHOLD 1    // Adjust based on testing
#define TAP_WINDOW 500     // Time window for double tap (ms)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_MPU6050 mpu;

// Servo objects and pins
Servo frontLeftServo;
Servo frontRightServo;
Servo backLeftServo;
Servo backRightServo;

const int frontLeftPin = D0;
const int frontRightPin = D1;
const int backLeftPin = D2;
const int backRightPin = D3;

// State definitions
enum RobotState {
  SLEEPING,
  ACTIVE,
  UPSIDE_DOWN
};

// Servo angles
const int standAngle = 90;
const int sleepAngle = 0;
const int forwardStep = 60;
const int backwardStep = 150;
const int sitAngle = 20;
const int danceAngle1 = 60;
const int danceAngle2 = 120;

// Global variables
RobotState currentState = SLEEPING;
unsigned long lastTapTime = 0;
int tapCount = 0;
int delay_time = 200;
bool isUpright = true;
unsigned long lastActionTime = 0;
const unsigned long IDLE_TIMEOUT = 30000; // 30 seconds before sleeping

void setup() {
  Serial.begin(115200);
  
  // Initialize display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  // Show boot animation
  showBootAnimation();
  
  // Initialize servos
  frontLeftServo.attach(frontLeftPin);
  frontRightServo.attach(frontRightPin);
  backLeftServo.attach(backLeftPin);
  backRightServo.attach(backRightPin);
  
  // Initialize MPU6050
  initializeMPU();

  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(9);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);	// Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);
  
  stand();
  delay(1000);
  // Start in sleeping position
  sleep();
  sleepy();
}

void showBootAnimation() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  
  // Fade in effect
  for(int i = 0; i < 255; i += 51) {
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(35, 25);
    display.println("TN 24");
    display.display();
    delay(100);
  }
  delay(1000);
  display.clearDisplay();
  display.display();
}

void loop() {
  checkOrientation();
  checkTaps();
  
  switch(currentState) {
    case SLEEPING:
      handleSleepingState();
      break;
      
    case ACTIVE:
      handleActiveState();
      break;
      
    case UPSIDE_DOWN:
      handleUpsideDownState();
      break;
  }
}

void checkTaps() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  // float acceleration = sqrt(sq(a.acceleration.x) + sq(a.acceleration.y) + sq(a.acceleration.z));
  // Serial.println(acceleration);

  if(mpu.getMotionInterruptStatus()) {
    unsigned long currentTime = millis();
    Serial.println("Tap");
    

      tapCount++;

      if (tapCount == 2) { // Double tap detected
        Serial.println("Double Tap");
        if (currentState == SLEEPING) {
          wakeUp();
        }
        tapCount = 0;
      }

    lastTapTime = currentTime;
  }
}

void handleSleepingState() {
  sleepy();
  if (millis() - lastActionTime > 5000) {
    // Occasionally show sleeping animation
    if (random(100) < 10) {
      for (int i = 0; i < 3; i++) {
        dizzy();
        delay(500);
      }
    }
  }
}

void handleActiveState() {
  // Check if idle for too long
  if (millis() - lastActionTime > IDLE_TIMEOUT) {
    goToSleep();
    return;
  }



     mischievous();
     walkForward();
     delay(1000);

    cute();
    dance();
    delay(1000);

    wink();
    react();
    delay(1000);

    thinking();
    sit();
    delay(1000);

    sleep();
    mischievous();
    delay(1000);

    stand();
    love();
    delay(1000);


    play();
    mischievous();
    delay(1000);

  
  // Random playful behaviors
  // int action = random(100);
  // if (action < 20) {
  //    mischievous();
  //    walkForward();
  // } else if (action < 40) {
  //   cute();
  //   dance();
  // } else if (action < 60) {
  //   wink();
  //   react();
  // } else if (action < 80) {
  //   thinking();
  //   sit();
  //   delay(1000);
  //   stand();
  // } else {
  //   love();
  //   play();
  // }
  
  lastActionTime = millis();
}

void handleUpsideDownState() {
  static unsigned long lastUpsideDownAction = 0;
  const unsigned long ACTION_INTERVAL = 1000;
  
  if (millis() - lastUpsideDownAction > ACTION_INTERVAL) {
    crying();
    panicMovement();
    lastUpsideDownAction = millis();
  }
}

void panicMovement() {
  // Random panicked leg movements
  for (int i = 0; i < 4; i++) {
    frontLeftServo.write(random(0, 180));
    frontRightServo.write(random(0, 180));
    backLeftServo.write(random(0, 180));
    backRightServo.write(random(0, 180));
    delay(200);
  }
  stand(); // Try to return to standing position
}

void wakeUp() {
  currentState = ACTIVE;
  happy();
  for (int angle = sleepAngle; angle <= standAngle; angle += 5) {
    frontLeftServo.write(angle);
    frontRightServo.write(angle);
    backLeftServo.write(angle);
    backRightServo.write(angle);
    delay(20);
  }
  lastActionTime = millis();
}

void goToSleep() {
  currentState = SLEEPING;
  sleepy();
  for (int angle = standAngle; angle >= sleepAngle; angle -= 5) {
    frontLeftServo.write(angle);
    frontRightServo.write(180 - angle);
    backLeftServo.write(180);
    backRightServo.write(angle);
    delay(20);
  }
}

void checkOrientation() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  float zAcceleration = a.acceleration.z;
  bool wasUpright = isUpright;
  isUpright = zAcceleration > 1.0;
  
  if (!isUpright && wasUpright) {
    currentState = UPSIDE_DOWN;
    crying();
  } else if (isUpright && !wasUpright) {
    currentState = ACTIVE;
    happy();
  }
}

void initializeMPU() {
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip!");
    while (1) delay(10);
  }
  
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100);
}

void selfRight() {
  Serial.println("Attempting to self-right...");
  // Add logic to try returning to upright position
  frontLeftServo.write(sleepAngle);
  frontRightServo.write(180 - sleepAngle); // Inverted for the right side
  backLeftServo.write(180);
  backRightServo.write(sleepAngle); // Inverted for the right side
  delay(delay_time);
  stand();
  delay(delay_time);
}

// Rest of the movement functions (walkForward, walkBackward, etc.) remain unchanged
void stand() {
  frontLeftServo.write(standAngle);
  frontRightServo.write(standAngle);
  backLeftServo.write(standAngle);
  backRightServo.write(standAngle);
}

void sleep() {
  frontLeftServo.write(sleepAngle);
  frontRightServo.write(180 - sleepAngle); // Inverted for the right side
  backLeftServo.write(180);
  backRightServo.write(sleepAngle); // Inverted for the right side
  delay(1000);
}

void walkForward() {
  // Lift and move front-left and back-right legs
  frontLeftServo.write(forwardStep);
  backRightServo.write(180 - forwardStep); // Inverted for the right side
  delay(delay_time);

  // Reset to standing position
  frontLeftServo.write(standAngle);
  backRightServo.write(standAngle);
  delay(delay_time);

  // Lift and move front-right and back-left legs
  frontRightServo.write(180 - forwardStep); // Inverted for the right side
  backLeftServo.write(forwardStep);
  delay(delay_time);

  // Reset to standing position
  frontRightServo.write(standAngle);
  backLeftServo.write(standAngle);
  delay(delay_time);
}


void walkBackward() {
  // Similar to moveForward but in reverse
  frontLeftServo.write(backwardStep);
  backRightServo.write(180 - backwardStep); // Inverted for the right side
  delay(500);

  frontLeftServo.write(standAngle);
  backRightServo.write(standAngle);
  delay(500);

  frontRightServo.write(180 - backwardStep); // Inverted for the right side
  backLeftServo.write(backwardStep);
  delay(500);

  frontRightServo.write(standAngle);
  backLeftServo.write(standAngle);
  delay(500);
}


void turnLeft() {
  // Move only one side of the legs forward
  frontLeftServo.write(forwardStep);
  backLeftServo.write(forwardStep);
  delay(500);

  frontLeftServo.write(standAngle);
  backLeftServo.write(standAngle);
  delay(500);
}

void turnRight() {
  // Move only the other side of the legs forward
  frontRightServo.write(180 - forwardStep); // Inverted for the right side
  backRightServo.write(180 - forwardStep); // Inverted for the right side
  delay(500);

  frontRightServo.write(standAngle);
  backRightServo.write(standAngle);
  delay(500);
}

void sit() {
  // Set all legs to a sitting angle
  frontLeftServo.write(90);
  frontRightServo.write(90); // Inverted for the right side
  backLeftServo.write(180 - sitAngle);
  backRightServo.write(sitAngle); // Inverted for the right side
  delay(1000);


}

void dance() {
  for (int i = 0; i < 3; i++) {
    frontLeftServo.write(danceAngle1);
    backRightServo.write(180 - danceAngle1); // Inverted for the right side
    delay(200);

    frontRightServo.write(180 - danceAngle2); // Inverted for the right side
    backLeftServo.write(danceAngle2);
    delay(200);

    frontLeftServo.write(danceAngle2);
    backRightServo.write(180 - danceAngle2); // Inverted for the right side
    delay(200);

    frontRightServo.write(180 - danceAngle1); // Inverted for the right side
    backLeftServo.write(danceAngle1);
    delay(200);
  }

  // Return to standing position
  frontLeftServo.write(standAngle);
  frontRightServo.write(standAngle);
  backLeftServo.write(standAngle);
  backRightServo.write(standAngle);
}

void react() {
  frontLeftServo.write(90);
  frontRightServo.write(90); // Inverted for the right side
  backLeftServo.write(180 - sitAngle);
  backRightServo.write(sitAngle); // Inverted for the right side
  delay(1000);

  // Return to standing position
  stand();
  delay(500);

  frontLeftServo.write(sitAngle);
  frontRightServo.write(180 - sitAngle); // Inverted for the right side
  backLeftServo.write(90);
  backRightServo.write(90); // Inverted for the right side
  delay(1000);

int del = 250;
  frontLeftServo.write(0);
  frontRightServo.write(160); // Inverted for the right side
  delay(del);

  frontLeftServo.write(20 );
  frontRightServo.write(180 ); // Inverted for the right side
  delay(del);


  frontLeftServo.write(0);
  frontRightServo.write(180 - sitAngle); // Inverted for the right side
  delay(del);


  frontLeftServo.write(sitAngle );
  frontRightServo.write(180); // Inverted for the right side
  delay(del);

  frontLeftServo.write(0);
  frontRightServo.write(160); // Inverted for the right side
  delay(del);

  frontLeftServo.write(20 );
  frontRightServo.write(180 ); // Inverted for the right side
  delay(del);


  frontLeftServo.write(0);
  frontRightServo.write(180 - sitAngle); // Inverted for the right side
  delay(del);


  frontLeftServo.write(sitAngle );
  frontRightServo.write(170); // Inverted for the right side
  delay(del);

  // Return to standing position
  stand();
  delay(500);
}


void play()
{
  for (int i = 0; i < 3; i++) {
    frontLeftServo.write(danceAngle1);
    backRightServo.write(180 - danceAngle1); // Inverted for the right side
    delay(200);

    frontRightServo.write(180 - danceAngle2); // Inverted for the right side
    backLeftServo.write(danceAngle2);
    delay(200);

    frontLeftServo.write(danceAngle2);
    backRightServo.write(180 - danceAngle2); // Inverted for the right side
    delay(200);

    frontRightServo.write(180 - danceAngle1); // Inverted for the right side
    backLeftServo.write(danceAngle1);
    delay(200);
  }

  // Return to standing position
  frontLeftServo.write(standAngle);
  frontRightServo.write(standAngle);
  backLeftServo.write(standAngle);
  backRightServo.write(standAngle);

}

