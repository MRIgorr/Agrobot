#include <NewPing.h>
#include <Servo.h>

#define PIN_TRIG_left 13  // PIN 55, 54 Ultra Sonic (left)
#define PIN_ECHO_left 12

#define PIN_TRIG_right 11  // PIN 11, 10 Ultra Sonic (right)
#define PIN_ECHO_right 10

#define PIN_TRIG_gate 8  // // PIN 9, 8 Ultra Sonic (gate)
#define PIN_ECHO_gate 9

//#define PIN_TRIG_opt 7  // PIN 7, 6 Ultra Sonic (ground)
//#define PIN_ECH_opt 6
int led1 = 38;
int led2 = 34;

int count = 0;

int bush = 50;
int uz_right = 0;
int uz_left = 0;
int er = 10;

const uint8_t EN_left = 2;
const uint8_t L_PWM_left = 41;
const uint8_t R_PWM_left = 40;

const uint8_t EN_right = 5;
const uint8_t L_PWM_right = 44;
const uint8_t R_PWM_right = 45;

const uint8_t EN_rotateZ = 4;
const uint8_t L_PWM_rotateZ = 47;
const uint8_t R_PWM_rotateZ = 46;

const uint8_t EN_rotateY = 3;
const uint8_t L_PWM_rotateY = 51;
const uint8_t R_PWM_rotateY = 50;
//концевики
const int top_button = 22, bottom_button = 24;  //верхняя и нижняя кнопка рельсовой направляющей
const int far_button = 26, near_button = 28;    //верхняя и нижняя кнопка повортного механизма
const int button_led1 = 36, button_led2 = 32;   //нижняя кнопки панели и верхняя

int sensorValue;

bool top_buttonState = true, top_buttonPast = true;
bool bottom_buttonState = true, bottom_buttonPast = true;
bool far_buttonState = true, far_buttonPast = true;
bool near_buttonState = true, near_buttonPast = true;
bool led1_buttonState = true, led1_buttonPast = true;
bool led2_buttonState = true, led2_buttonPast = true;

#define MAX_DISTANCE 2000

NewPing sonar_left(PIN_TRIG_left, PIN_ECHO_left, MAX_DISTANCE);
NewPing sonar_right(PIN_TRIG_right, PIN_ECHO_right, MAX_DISTANCE);
NewPing sonar_gate(PIN_TRIG_gate, PIN_ECHO_gate, MAX_DISTANCE);
//NewPing sonar(PIN_TRIG, PIN_ECHO, MAX_DISTANCE);

Servo myServo_grab;
Servo myServo_cut;

void setup() {
  myServo_grab.attach(7);

  myServo_cut.attach(6);
  myServo_cut.write(180);
  myServo_grab.write(0);
  myServo_cut.write(180);

  pinMode(top_button, INPUT_PULLUP);  //инициализация подключенных устройств
  pinMode(far_button, INPUT_PULLUP);
  pinMode(near_button, INPUT_PULLUP);
  pinMode(bottom_button, INPUT_PULLUP);
  pinMode(button_led1, INPUT_PULLUP);
  pinMode(button_led2, INPUT_PULLUP);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  analogWrite(EN_right, LOW);
  analogWrite(EN_left, LOW);
  analogWrite(EN_rotateZ, LOW);
  analogWrite(EN_rotateY, LOW);

  //Serial.begin(9600);
}

unsigned int dist_left;
unsigned int dist_right;
unsigned int dist_gate;

int NORMAL_dist = 20;

int top_val = -1, bottom_val = -1;
int near_val = -1, far_val = -1;
int led1_val = 0, led2_val = 0;

void move_robot(int direction, int en_speed, int time, int k) {
  if (direction == 1) {
    digitalWrite(L_PWM_right, LOW);
    digitalWrite(L_PWM_left, HIGH);
    digitalWrite(R_PWM_right, HIGH);
    digitalWrite(R_PWM_left, LOW);
    analogWrite(EN_right, en_speed - k);
    analogWrite(EN_left, en_speed);
    delay(time);
    analogWrite(EN_right, int(en_speed - en_speed / 3) - k);
    analogWrite(EN_left, int(en_speed - en_speed / 3));
    delay(time);
    analogWrite(EN_right, int(en_speed - en_speed / 1.5) - k);
    analogWrite(EN_left, int(en_speed - en_speed / 1.5));
    delay(time);
    analogWrite(EN_right, LOW);
    analogWrite(EN_left, LOW);
  }
  if (direction == 0) {
    digitalWrite(L_PWM_right, HIGH);
    digitalWrite(L_PWM_left, LOW);
    digitalWrite(R_PWM_right, LOW);
    digitalWrite(R_PWM_left, HIGH);
    analogWrite(EN_right, en_speed - k);
    analogWrite(EN_left, en_speed);
    delay(time);
    analogWrite(EN_right, int(en_speed - en_speed / 3) - k);
    analogWrite(EN_left, int(en_speed - en_speed / 3));
    delay(time);
    analogWrite(EN_right, int(en_speed - en_speed / 1.5) - k);
    analogWrite(EN_left, int(en_speed - en_speed / 1.5));
    delay(time);
    analogWrite(EN_right, LOW);
    analogWrite(EN_left, LOW);
  }
  if (direction == 2) {
    analogWrite(EN_right, LOW);
    analogWrite(EN_left, LOW);
  }
}

void drive(int direction, int en_speed_R, int en_speed_L) {
  if (direction == 1) {
    digitalWrite(L_PWM_right, LOW);
    digitalWrite(L_PWM_left, HIGH);
    digitalWrite(R_PWM_right, HIGH);
    digitalWrite(R_PWM_left, LOW);
    analogWrite(EN_right, en_speed_R);
    analogWrite(EN_left, en_speed_L);
  }
  if (direction == 0) {
    digitalWrite(L_PWM_right, HIGH);
    digitalWrite(L_PWM_left, LOW);
    digitalWrite(R_PWM_right, LOW);
    digitalWrite(R_PWM_left, HIGH);
    analogWrite(EN_right, en_speed_R);
    analogWrite(EN_left, en_speed_L);
  }
}

void cutting(int n) {
  for (int i = 0; i < n; i++) {
    myServo_cut.write(150);
    delay(200);
    myServo_cut.write(180);
    delay(200);
    myServo_cut.write(150);
    delay(200);
    myServo_cut.write(180);
    delay(200);
    myServo_cut.write(135);
    delay(200);
    myServo_cut.write(165);
    delay(200);
    myServo_cut.write(135);
    delay(200);
    myServo_cut.write(165);
    delay(200);
    myServo_cut.write(120);
    delay(200);
    myServo_cut.write(150);
    delay(200);
    myServo_cut.write(120);
    delay(200);
    myServo_cut.write(150);
    delay(200);
    myServo_cut.write(105);
    delay(200);
    myServo_cut.write(135);
    delay(200);
    myServo_cut.write(105);
    delay(200);
    myServo_cut.write(135);
    delay(200);
  }
}

void loop() {

  unsigned int dist_left = sonar_left.ping_cm();
  unsigned int dist_right = sonar_right.ping_cm();
  unsigned int dist_gate = sonar_gate.ping_cm();

  top_buttonState = digitalRead(top_button);

  if (!top_buttonState && top_buttonPast) {

    top_buttonState = digitalRead(top_button);
    if (!top_buttonState && top_buttonPast) top_val++;
  }
  top_buttonPast = top_buttonState;

  bottom_buttonState = digitalRead(bottom_button);

  if (!bottom_buttonState && bottom_buttonPast) {

    bottom_buttonState = digitalRead(bottom_button);
    if (!bottom_buttonState && bottom_buttonPast) bottom_val++;
  }
  bottom_buttonPast = bottom_buttonState;

  far_buttonState = digitalRead(far_button);

  if (!far_buttonState && far_buttonPast) {

    far_buttonState = digitalRead(far_button);
    if (!far_buttonState && far_buttonPast) far_val++;
  }
  far_buttonPast = far_buttonState;

  near_buttonState = digitalRead(near_button);

  if (!near_buttonState && near_buttonPast) {
    near_buttonState = digitalRead(near_button);
    if (!near_buttonState && near_buttonPast) near_val++;
  }
  near_buttonPast = near_buttonState;

  led1_buttonState = digitalRead(button_led1);

  if (!led1_buttonState && led1_buttonPast) {

    led1_buttonState = digitalRead(button_led1);
    if (!led1_buttonState && led1_buttonPast) led1_val++;
  }
  led1_buttonPast = led1_buttonState;

  led2_buttonState = digitalRead(button_led2);

  if (!led2_buttonState && led2_buttonPast) {

    led2_buttonState = digitalRead(button_led2);
    if (!led2_buttonState && led2_buttonPast) led2_val++;
  }
  led2_buttonPast = led2_buttonState;

  if (((led1_val + 1) % 2 == 0) && ((led2_val + 1) % 2 == 0)) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    uz_right = map(dist_right, bush, er, 0, 140);
    uz_left = map(dist_left, bush, er, 0, 140);
    if (dist_right < bush && dist_right > er && dist_left > bush) {
      drive(1, 60 + uz_right, 70);
    } else if (dist_left < bush && dist_left > er && dist_right > bush) {
      drive(1, 60, 70 + uz_left);
    } else {
      drive(1, 60, 70);
    }
  } else if ((led1_val + 1) % 2 == 0) {  //led1
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    move_robot(1, 150, 500, 25);
    move_robot(0, 150, 500, 25);
    move_robot(2, 0, 600, 20);
  } else if ((led2_val + 1) % 2 == 0) {
    analogWrite(EN_right, 0);
    analogWrite(EN_left, 0);
    digitalWrite(led2, HIGH);
    digitalWrite(led1, LOW);
    if (dist_gate < 10) {
      myServo_grab.write(40);
      cutting(1);
      myServo_cut.write(180);
      while (!bottom_buttonState) {
        digitalWrite(L_PWM_rotateY, LOW);
        digitalWrite(R_PWM_rotateY, HIGH);
        analogWrite(EN_rotateY, 50);
        bottom_buttonState = digitalRead(bottom_button);
      }
      analogWrite(EN_rotateY, LOW);
      delay(500);
      while (!far_buttonState) {
        digitalWrite(L_PWM_rotateZ, LOW);
        digitalWrite(R_PWM_rotateZ, HIGH);
        analogWrite(EN_rotateZ, 140);
        delay(10);
        analogWrite(EN_rotateZ, 10);
        delay(5);
        far_buttonState = digitalRead(far_button);
      }
      analogWrite(EN_rotateZ, LOW);
      delay(500);
      myServo_grab.write(5);
      delay(1000);
      while (!near_buttonState) {
        digitalWrite(L_PWM_rotateZ, HIGH);
        digitalWrite(R_PWM_rotateZ, LOW);
        analogWrite(EN_rotateZ, 140);
        delay(10);
        analogWrite(EN_rotateZ, 10);
        delay(5);
        near_buttonState = digitalRead(near_button);
      }
      analogWrite(EN_rotateZ, LOW);
      while (!top_buttonState) {
        digitalWrite(L_PWM_rotateY, HIGH);
        digitalWrite(R_PWM_rotateY, LOW);
        analogWrite(EN_rotateY, 50);
        top_buttonState = digitalRead(top_button);
      }
      analogWrite(EN_rotateY, LOW);
    }
  } else {
    analogWrite(EN_right, 0);
    analogWrite(EN_left, 0);
    digitalWrite(led2, LOW);
    digitalWrite(led1, LOW);
    myServo_grab.write(0);
    myServo_cut.write(180);
    while (!near_buttonState) {
      digitalWrite(L_PWM_rotateZ, HIGH);
      digitalWrite(R_PWM_rotateZ, LOW);
      analogWrite(EN_rotateZ, 140);
      delay(10);
      analogWrite(EN_rotateZ, 10);
      delay(5);
      near_buttonState = digitalRead(near_button);
    }
    analogWrite(EN_rotateZ, LOW);
    while (!top_buttonState) {
      digitalWrite(L_PWM_rotateY, HIGH);
      digitalWrite(R_PWM_rotateY, LOW);
      analogWrite(EN_rotateY, 50);
      top_buttonState = digitalRead(top_button);
    }
    analogWrite(EN_rotateY, LOW);
  }


  /**
  digitalWrite(L_PWM_rotateZ, HIGH);
  digitalWrite(R_PWM_rotateZ, LOW);
  analogWrite(EN_rotateZ, 80);
  delay(1000);
  analogWrite(EN_rotateZ, LOW);
  while (true) {
    delay(100);
  }
  **/

  /**if (dist_gate < 5) {
    digitalWrite(L_PWM_right, LOW);
    digitalWrite(L_PWM_left, HIGH);
    digitalWrite(R_PWM_right, HIGH);
    digitalWrite(R_PWM_left, LOW);
    analogWrite(EN_right, 50);
    analogWrite(EN_left, 50);
    delay(1000);
    analogWrite(EN_right, LOW);
    analogWrite(EN_left, LOW);
    while (!far_buttonState) {
      digitalWrite(L_PWM_rotateZ, LOW);
      digitalWrite(R_PWM_rotateZ, HIGH);
      analogWrite(EN_rotateZ, 50);
      far_buttonState = digitalRead(far_button);
    }
    analogWrite(EN_rotateZ, LOW);
    while (!top_buttonState) {
      digitalWrite(L_PWM_rotateY, HIGH);
      digitalWrite(R_PWM_rotateY, LOW);
      analogWrite(EN_rotateY, 50);
      top_buttonState = digitalRead(top_button);
    }
    analogWrite(EN_rotateY, LOW); 
    while (myServo_grab.read() != MINcorner) {
      REALcorner--;
      myServo_grab.write(REALcorner);
    }
    for (int i = 0; i < 3; i++) { //cutting 
      while (myServo_cut.read() != MAXangle) {
        REALangle++;
        myServo_cut.write(REALangle);
      }
      while (myServo_cut.read() != MINangle) {
        REALangle--;
        myServo_cut.write(REALangle);
      }
    }
    while (!bottom_buttonState) { //rotating to bag 
      digitalWrite(L_PWM_rotateY, LOW);
      digitalWrite(R_PWM_rotateY, HIGH);
      analogWrite(EN_rotateY, 50);
      bottom_buttonState = digitalRead(bottom_button);
    }
    analogWrite(EN_rotateY, LOW);
    while (!near_buttonState) {
      digitalWrite(L_PWM_rotateZ, HIGH);
      digitalWrite(R_PWM_rotateZ, LOW);
      analogWrite(EN_rotateZ, 50);
      near_buttonState = digitalRead(near_button);
    }
    analogWrite(EN_rotateZ, LOW);
    while (myServo_grab.read() != MAXcorner) {
      REALcorner++;
      myServo_grab.write(REALcorner);
    }
    while (myServo_cut.read() != MINangle) {
      REALangle--;
      myServo_cut.write(REALangle);
    }
    while (!far_buttonState) {
      digitalWrite(L_PWM_rotateZ, LOW);
      digitalWrite(R_PWM_rotateZ, HIGH);
      analogWrite(EN_rotateZ, 50);
      far_buttonState = digitalRead(far_button);
    }
  } else {
    digitalWrite(L_PWM_right, LOW);
    digitalWrite(L_PWM_left, HIGH);
    digitalWrite(R_PWM_right, HIGH);
    digitalWrite(R_PWM_left, LOW);
    analogWrite(EN_right, EN_right_val);
    analogWrite(EN_left, EN_left_val);
    if (dist_right < NORMAL_dist) {
      EN_right_val = EN_right_val + 10;
    } else if (dist_left < NORMAL_dist) {
      EN_left_val = EN_left_val + 10;
    } else {
      EN_right_val, EN_left_val = 127;
    }
  }
  **/
}