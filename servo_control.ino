#include <Servo.h>
#include <math.h>

// Initializing servos - name and pin number
Servo yaw_servo;
Servo pitch_servo;

int yaw_pin = 3; // TODO: get actual
int pitch_pin = 5; // TODO: get actual

float p_rad = 0;
float y_rad = 0;

void setup() {
  // Setting up servos
  yaw_servo.attach(yaw_pin);
  pitch_servo.attach(pitch_pin);
  
  // This code is for manually entering p and y until we have a way to receive them from the camera
  // TODO: figure out how to receive p from the camera
  Serial.begin(9600);

}

void loop() {
  // This code is for manually entering p and y until we have a way to receive them from the camera
  Serial.println("Enter p (degrees): ");
  while (Serial.available() == 0) {
    delay(100);
  }
  p_rad = Serial.parseFloat() * (M_PI / 180);
  while (Serial.available() > 0) {
    Serial.read();
  }
  Serial.println("Enter y (degrees): ");
  while (Serial.available() == 0) {
    delay(100);
  }
  y_rad = Serial.parseFloat() * (M_PI / 180);
  while (Serial.available() > 0) {
    Serial.read();
  }
  Serial.println(p_rad);
  Serial.println(y_rad);

  // Constants: v = base velocity of pumped water, h = height above ground of camera (same as nozzle)
  float v = 20.3;
  float h = 0.25;

  // Distance to target: required for theta calculation and derived from height of camera and pitch to target
  float d = h / tanf(p_rad);

  // This code is for retrieving the current p and y from the servo. Note: it doesn't use an encoder, it's just the last p and y written.
  float c_p_deg = pitch_servo.read();
  float c_y_deg = yaw_servo.read();

  // Theta formula: this code converts the elevation angle given by the camera to the elevation angle for the nozzle, accounting for the
  // effects of gravity on water trajectory. See the simulation code in the team drive for a demonstration.
  float theta_rad = atanf((-(v*v) + sqrtf(powf(v,4) + (9.81 * ((2 * h * v * v) + (9.81 * d * d))))) / (9.81 * d));
  // The input angles are in radians for the trig functions, but we convert the output angle to degrees for the servos
  float theta_deg = theta_rad * (180 / M_PI);
  float y_deg = y_rad * (180 / M_PI);

  // Pass angles to servos
  pitch_servo.write(roundf(theta_deg));
  yaw_servo.write(roundf(y_deg));
}
