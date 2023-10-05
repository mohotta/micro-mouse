volatile long counter = 0;

// Define the pins
const int encoderPin_A = 3;
const int encoderPin_B = 2;

const int MotorE_ENA = 10;  // Enable pin for motor E
const int MotorE_IN1 = 9;   // Input pin 1 for motor E
const int MotorE_IN2 = 8;   // Input pin 2 for motor E

// define the pid gains
const float kp = 0.4; // proportional gain. respond more qickly ==> unstable
const float ki = 0.2; // integral gain. eliminate steady state error,converge desired value quickly ==> sluggish
const float kd = 0.1; // derivative gain. prevent overshoot and oscilations ==> sluggish

// define the pid variables
float setpoint = 1000; // desired position of the moter rod
float previous_error = 0; // error in the previous loop (set point - current position)
float integral = 0; // the sum of the error over time
float previous_time = 0; // the time in the previous loop

void setup() {
    // Set the pinMode for the pins
  pinMode(MotorE_ENA, OUTPUT);
  pinMode(MotorE_IN1, OUTPUT);
  pinMode(MotorE_IN2, OUTPUT);
  Serial.begin(9600);

  pinMode(encoderPin_A, INPUT_PULLUP);  // internal pullup input pin 2
  pinMode(encoderPin_B, INPUT_PULLUP);  // internalเป็น pullup input pin 3

  //Setting up interrupt
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  attachInterrupt(digitalPinToInterrupt(encoderPin_A), ai0, RISING);

  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  attachInterrupt(digitalPinToInterrupt(encoderPin_B), ai1, RISING);
}

int PID(int current_position) {
  float error = setpoint - float(current_position); // current position should change to the setpoint accorfing to the error
  float proportional_term = kp * error; // how much the output should chnage based on the error

  integral += error * (millis() - previous_time); // millis returns the time in milliseconds since the arduino board began running the current program
  integral = ki * integral;
  float derivative_term = kd * (error - previous_error) / (millis() - previous_time); // how much the output should chnage based on the rate of change of the error
  float output = proportional_term + integral + derivative_term; // how much controller should change the input

  // update previous values
  previous_error = error;
  previous_time = millis();

  // wait for 10 milliseconds to see the dimming effect
  delay(10);

  return int(output);
}

void loop() {
  Serial.print("Input :");
  Serial.print(counter);
  int speed = PID(counter);
  Serial.print("   Output :");
  Serial.println(speed);
  runMotor(speed);
}

void runMotor(int speed) {
  if (speed > 0) {
  digitalWrite(MotorE_IN1, LOW);
  digitalWrite(MotorE_IN2, HIGH);
  }
  else {
  digitalWrite(MotorE_IN1, HIGH);
  digitalWrite(MotorE_IN2, LOW);
  }

  int m_speed = abs(speed);
  if (m_speed > 255) {
    m_speed = 255;
  }
  m_speed = map(m_speed, 0, 255, 75, 255);
  analogWrite(MotorE_ENA, m_speed);
}

void ai0() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if (digitalRead(encoderPin_B) == LOW) {
    counter++;
  } else {
    counter--;
  }
}

void ai1() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if (digitalRead(encoderPin_A) == LOW) {
    counter--;
  } else {
    counter++;
  }
}
