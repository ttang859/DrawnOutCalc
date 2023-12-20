//sourced from https://www.thingiverse.com/thing:248009
// When in calibration mode, adjust the following factor until the servos move exactly 90 degrees
#define SERVOFAKTORLEFT 610
#define SERVOFAKTORRIGHT 620

// Zero-position of left and right servo
// When in calibration mode, adjust the NULL-values so that the servo arms are at all times parallel
// either to the X or Y axis
//#define SERVOLEFTNULL 2250
//#define SERVORIGHTNULL 920
#define SERVOLEFTNULL 1650
#define SERVORIGHTNULL 500

#define SERVOPINLIFT  2
#define SERVOPINLEFT  23
#define SERVOPINRIGHT 18

// lift positions of lifting servo
//#define LIFT0 1080 // on drawing surface
//#define LIFT1 925  // between numbers
//#define LIFT2 725  // going towards sweeper
#define LIFT0 850  // on drawing surface
#define LIFT1 600  // between numbers
#define LIFT2 350  // going towards sweeper
#define LIFT3 900  // down into sweeper

// speed of liftimg arm, higher is slower
//#define LIFTSPEED 1500
#define LIFTSPEED 3000

// length of arms
#define L1 35
#define L2 57.1
#define L3 13.2

// origin points of left and right servo 
//#define O1X 22
//#define O1Y -25
//#define O2X 47
//#define O2Y -25
#define O1X 21
#define O1Y -25
#define O2X 48
#define O2Y -25

#define PARKX 75
#define PARKY 46
#define ERASEMAXX 50

#include <ESP32Servo.h>

int servoLift = 1500;

Servo servo1;  // 
Servo servo2;  // 
Servo servo3;  // 

volatile double lastX = 75;
volatile double lastY = PARKY;

#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(128, 64);

#define B0 34
#define B1 0
#define B2 35

const int pins[] = {T0, T3, T4, T5, T6, T7, T8, T9};

#define TOUCH_THRESHOLD  25
int previndex = -1;
unsigned long timeGap = millis();
double operatorNum = 0.0;
double Ans = 0.0;
char operatorSign = ' ';

boolean clearBoard = false;
double lastOpNum = 0;
unsigned long lastTouch = millis();

volatile unsigned long lastclick = 0;
volatile boolean EQUALS_CLICKED = false;
boolean repeat = false;

void IRAM_ATTR pressAndDraw() {
  if(millis() > lastclick + 1000){
    EQUALS_CLICKED = true;
    lastclick = millis();
  }
}

void equalButton(){
  static byte prev_state=1;
  static unsigned long prev_time=0;
  byte curr_state = digitalRead(B1);
  if(curr_state==0 && prev_state==1) {
    if(millis()>prev_time+100) {
      prev_time=millis();
      display.clearDisplay();
      display.setCursor(0, 0);
      switch(operatorSign){
        case '+':
          Ans+=operatorNum;
          display.print(Ans);
          break;
        case '-':
          Ans-=operatorNum;
          display.print(Ans);
          break;
        case '*':
          Ans*=operatorNum;
          display.print(Ans);
          break;
        case '/':
          Ans/=operatorNum;
          display.print(Ans);
          break;
        default:
          display.clearDisplay();
          display.setCursor(0,0);
          display.print("no operator input");
      }
      operatorSign = ' ';
      operatorNum = 0.0;
      display.display();        
    }
  }
  prev_state=curr_state;
}

void incrementButton(){//increments the digit value
  static byte prev_state1=1;
  static unsigned long prev_time1=0;
  byte curr_state1 = digitalRead(B0);
  if(curr_state1==0 && prev_state1==1) {
    if(millis()>prev_time1+100) {
      prev_time1=millis();
      display.clearDisplay();
      display.setCursor(0, 0);
      operatorNum+=1;
      display.print(operatorNum);
      display.display();
      lastTouch = millis();
      repeat = true;       
    }
  }
  prev_state1=curr_state1;
}

void decrementButton(){//decrements the digit value **find way t
  static byte prev_state2=1;
  static unsigned long prev_time2=0;
  byte curr_state2 = digitalRead(B2);
  if(curr_state2==0 && prev_state2==1) {
    if(millis()>prev_time2+100) { 
      prev_time2=millis();
      display.clearDisplay();
      display.setCursor(0, 0);
      operatorNum-=1;
      display.print(operatorNum);
      display.display();
      lastTouch = millis();     
      repeat = true;     
    }
  }
  prev_state2=curr_state2;
}

void operatorPressed(int operatorCase){//function that determines what operator sign is (mainly for debugging purposes and printing
  Ans = operatorNum;//stores the first number being operated on
  operatorNum = 0.0;
  switch(operatorCase-4){
    case 0:
      operatorSign = '+';
      break;
    case 1:
      operatorSign = '-';
      break;
    case 2:
      operatorSign = '*';
      break;
    case 3:
      operatorSign = '/';
      break;
  }
  display.clearDisplay();
}

// Writing numeral with bx by being the bottom left originpoint. Scale 1 equals a 20 mm high font. (do scale 0.5)
// The structure follows this principle: move to first startpoint of the numeral, lift down, draw numeral, lift up
void number(float bx, float by, char character, float scale) {

  switch (character) {

  case '0':
    drawTo(bx + 12 * scale, by + 6 * scale);
    lift(0);
    bogenGZS(bx + 7 * scale, by + 10 * scale, 10 * scale, -0.8, 6.7, 0.5);
    lift(1);
    break;
  case '1':

    drawTo(bx + 3 * scale, by + 15 * scale);
    lift(0);
    drawTo(bx + 10 * scale, by + 20 * scale);
    drawTo(bx + 10 * scale, by + 0 * scale);
    lift(1);
    break;
  case '2':
    drawTo(bx + 2 * scale, by + 12 * scale);
    lift(0);
    bogenUZS(bx + 8 * scale, by + 14 * scale, 6 * scale, 3, -0.8, 1);
    drawTo(bx + 1 * scale, by + 0 * scale);
    drawTo(bx + 12 * scale, by + 0 * scale);
    lift(1);
    break;
  case '3':
    drawTo(bx + 2 * scale, by + 17 * scale);
    lift(0);
    bogenUZS(bx + 5 * scale, by + 15 * scale, 5 * scale, 3, -2, 1);
    bogenUZS(bx + 5 * scale, by + 5 * scale, 5 * scale, 1.57, -3, 1);
    lift(1);
    break;
  case '4':
    drawTo(bx + 10 * scale, by + 0 * scale);
    lift(0);
    drawTo(bx + 10 * scale, by + 20 * scale);
    drawTo(bx + 2 * scale, by + 6 * scale);
    drawTo(bx + 12 * scale, by + 6 * scale);
    lift(1);
    break;
  case '5':
    drawTo(bx + 2 * scale, by + 5 * scale);
    lift(0);
    bogenGZS(bx + 5 * scale, by + 6 * scale, 6 * scale, -2.5, 2, 1);
    drawTo(bx + 5 * scale, by + 20 * scale);
    drawTo(bx + 12 * scale, by + 20 * scale);
    lift(1);
    break;
  case '6':
    drawTo(bx + 2 * scale, by + 10 * scale);
    lift(0);
    bogenUZS(bx + 7 * scale, by + 6 * scale, 6 * scale, 2, -4.4, 1);
    drawTo(bx + 11 * scale, by + 20 * scale);
    lift(1);
    break;
  case '7':
    drawTo(bx + 2 * scale, by + 20 * scale);
    lift(0);
    drawTo(bx + 12 * scale, by + 20 * scale);
    drawTo(bx + 2 * scale, by + 0);
    lift(1);
    break;
  case '8':
    drawTo(bx + 5 * scale, by + 10 * scale);
    lift(0);
    bogenUZS(bx + 5 * scale, by + 15 * scale, 5 * scale, 4.7, -1.6, 1);
    bogenGZS(bx + 5 * scale, by + 5 * scale, 5 * scale, -4.7, 2, 1);
    lift(1);
    break;

  case '9':
    drawTo(bx + 9 * scale, by + 11 * scale);
    lift(0);
    bogenUZS(bx + 7 * scale, by + 15 * scale, 5 * scale, 4, -0.5, 1);
    drawTo(bx + 5 * scale, by + 0);
    lift(1);
    break;

  case 'e':

    lift(2);
    drawTo(PARKX-5, PARKY+1);
    lift(0);
    //lift(3);
    drawTo(ERASEMAXX, 43);

    drawTo(ERASEMAXX, 49);
    drawTo(5, 49);
    drawTo(5, 45);
    drawTo(ERASEMAXX, 45);
    drawTo(ERASEMAXX, 40);

    drawTo(5, 40);
    drawTo(5, 35);
    drawTo(ERASEMAXX, 35);
    drawTo(ERASEMAXX, 30);

    drawTo(5, 30);
    drawTo(5, 25);
    drawTo(ERASEMAXX, 25);
    drawTo(ERASEMAXX, 20);

    drawTo(5, 20);
    drawTo(60, 44);

    drawTo(PARKX-6,PARKY-4);
    delay(500);
    drawTo(PARKX, PARKY-2);
    delay(500);
    lift(2);

    break;

  case '.':
    drawTo(bx + 5 * scale, by + 5 * scale);
    lift(0);
    bogenGZS(bx + 5 * scale, by + 5 * scale, 0.1 * scale, 1, -1, 1);
    lift(1);
    break;
    
   case '-':
    double coord = by + 5 * scale;
    drawTo(bx + 5 * scale, coord);
    lift(0);
    drawTo(bx + 15 * scale, coord);
    lift(1);
    break;
  }
}



void lift(char lift) {
  switch (lift) {
    // room to optimize  !

  case 0: //850 (down)

      if (servoLift >= LIFT0) {
      while (servoLift >= LIFT0) 
      {
        servoLift--;
        servo1.writeMicroseconds(servoLift);        
        delayMicroseconds(LIFTSPEED);
      }
    } 
    else {
      while (servoLift <= LIFT0) {
        servoLift++;
        servo1.writeMicroseconds(servoLift);
        delayMicroseconds(LIFTSPEED);

      }

    }

    break;

  case 1: //150 (up)

    if (servoLift >= LIFT1) {
      while (servoLift >= LIFT1) {
        servoLift--;
        servo1.writeMicroseconds(servoLift);
        delayMicroseconds(LIFTSPEED);

      }
    } 
    else {
      while (servoLift <= LIFT1) {
        servoLift++;
        servo1.writeMicroseconds(servoLift);
        delayMicroseconds(LIFTSPEED);
      }

    }

    break;

  case 2: //(eraser)

    if (servoLift >= LIFT2) {
      while (servoLift >= LIFT2) {
        servoLift--;
        servo1.writeMicroseconds(servoLift);
        delayMicroseconds(LIFTSPEED);
      }
    } 
    else {
      while (servoLift <= LIFT2) {
        servoLift++;
        servo1.writeMicroseconds(servoLift);        
        delayMicroseconds(LIFTSPEED);
      }
    }
    break;
 case 3:
    if (servoLift >= LIFT3) {
      while (servoLift >= LIFT3) {
        servoLift--;
        servo1.writeMicroseconds(servoLift);
        delayMicroseconds(LIFTSPEED);

      }
    } 
    else {
      while (servoLift <= LIFT3) {
        servoLift++;
        servo1.writeMicroseconds(servoLift);
        delayMicroseconds(LIFTSPEED);
      }

    }
    break;
  }
}


void bogenUZS(float bx, float by, float radius, int start, int ende, float sqee) {
  float inkr = -0.05;
  float count = 0;

  do {
    drawTo(sqee * radius * cos(start + count) + bx,
    radius * sin(start + count) + by);
    count += inkr;
  } 
  while ((start + count) > ende);

}

void bogenGZS(float bx, float by, float radius, int start, int ende, float sqee) {
  float inkr = 0.05;
  float count = 0;

  do {
    drawTo(sqee * radius * cos(start + count) + bx,
    radius * sin(start + count) + by);
    count += inkr;
  } 
  while ((start + count) <= ende);
}

double return_angle(double a, double b, double c) {
  // cosine rule for angle between c and a
  return acos((a * a + c * c - b * b) / (2 * a * c));
}


void set_XY(double Tx, double Ty) 
{
  delay(1);
  double dx, dy, c, a1, a2, Hx, Hy;

  // calculate triangle between pen, servoLeft and arm joint
  // cartesian dx/dy
  dx = Tx - O1X;
  dy = Ty - O1Y;

  // polar lemgth (c) and angle (a1)
  c = sqrt(dx * dx + dy * dy); // 
  a1 = atan2(dy, dx); //
  a2 = return_angle(L1, L2, c);

  servo2.writeMicroseconds(floor(((a2 + a1 - M_PI) * SERVOFAKTORLEFT) + SERVOLEFTNULL));

  // calculate joinr arm point for triangle of the right servo arm
  a2 = return_angle(L2, L1, c);
  Hx = Tx + L3 * cos((a1 - a2 + 0.621) + M_PI); //36,5°
  Hy = Ty + L3 * sin((a1 - a2 + 0.621) + M_PI);

  // calculate triangle between pen joint, servoRight and arm joint
  dx = Hx - O2X;
  dy = Hy - O2Y;

  c = sqrt(dx * dx + dy * dy);
  a1 = atan2(dy, dx);
  a2 = return_angle(L1, (L2 - L3), c);

  servo3.writeMicroseconds(floor(((a1 - a2) * SERVOFAKTORRIGHT) + SERVORIGHTNULL));

}

void drawTo(double pX, double pY) {
  double dx, dy, c;
  int i;

  // dx dy of new point
  dx = pX - lastX;
  dy = pY - lastY;
  //path lenght in mm, times 4 equals 4 steps per mm
  c = floor(10 * sqrt(dx * dx + dy * dy));

  if (c < 1) c = 1;

  for (i = 0; i <= c; i++) {
    // draw line point by point
    set_XY(lastX + (i * dx / c), lastY + (i * dy / c));

  }

  lastX = pX;
  lastY = pY;
}
void wipe(){
    lift(2);
    number(3,3,'e',1);
}

void setup() 
{ 
  drawTo(PARKX, PARKY);
  lift(0);
  Serial.begin(115200);
  servo1.attach(SERVOPINLIFT);  //  lifting servo
  servo2.attach(SERVOPINLEFT);  //  left servo
  servo3.attach(SERVOPINRIGHT);  //  right servo

  pinMode(B0, INPUT_PULLUP);
  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Screen Initialized");
  display.display();  

  lift(2);
  drawTo(8,45);
  lift(1);
  attachInterrupt(B1, pressAndDraw, FALLING);
  delay(1000);
} 
//#define CALIBRATION
#define defaultPosX 8
#define defaultPosY 30
#define drawScale 0.65
#define maxX 55

void armDraw(double numToString){
    String numParse = String(numToString,2);
    int startX = defaultPosX;
    int startY = defaultPosY;
    for(int i = 0; i < sizeof(numParse); i++){
      if(startX > maxX && startY >= 0){
        startX = 8;
        startY -= 13;
      }
      number(startX,startY,numParse[i],drawScale);
      startX += 8;
    }
    delay(750);
    drawTo(defaultPosX,defaultPosY);
}

void loop() 
{ 
  lastOpNum = operatorNum;
  int nPins = sizeof(pins) / sizeof(int);
  int i;
  int minvalue = 1000;
  int minindex = -1;
  for (int i = 0; i < nPins; i++) {
    int value = 0;
    // do average filter to smooth out noisy readings
    for(int k=0;k<5;k++) {
      value+= touchRead(pins[i]);
    }
    value/=5;
    if(value < minvalue) {
      minvalue = value;
      minindex = i;
    }
  }

if(minvalue < TOUCH_THRESHOLD && previndex != minindex && millis() - timeGap > 1000) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.print(operatorNum);
    switch(minindex){
      case 0://clear Ans and operatorNum
        Ans = 0.0;
        operatorNum = 0.0;
        display.clearDisplay();
        display.setCursor(0,0);
        display.print("ANS HAS BEEN CLEARED");
        display.display();
        delay(500);
        display.clearDisplay();
        display.setCursor(0,0);
        display.print(operatorNum);
        clearBoard = true;
        break;
        
       case 1://set Ans to operatorNum to chain computing
        operatorNum = Ans;
        Ans = 0.0;
        display.clearDisplay();
        display.setCursor(0,0);
        display.print("ANS SET TO OPTNUM");
        display.display();
        delay(500);
        display.clearDisplay();
        display.setCursor(0,0);
        display.print(operatorNum);
        break;
        
       case 2://shifts a digit to the right 
        operatorNum*=10.0;
        display.clearDisplay();
        display.setCursor(0,0);
        display.print(operatorNum);
        display.display();
        lastTouch = millis();
        repeat = true;  
        break;
        
       case 3://shifts a digit to the left 
        operatorNum/=10.0;
        display.clearDisplay();
        display.setCursor(0,0);
        display.print(operatorNum);
        display.display();
        lastTouch = millis();
        repeat = true;  
        break;
       case 4:// add operator
        operatorPressed(minindex);
        display.clearDisplay();
        display.setCursor(0,0);
        display.print(operatorNum);
        display.print(operatorSign);
        clearBoard = true;
        break;

       case 5:// subtract operator
        operatorPressed(minindex);
        display.clearDisplay();
        display.setCursor(0,0);
        display.print(operatorNum);
        display.print(operatorSign);
        clearBoard = true;
        break;

       case 6:// multiply operator
        operatorPressed(minindex);
        display.clearDisplay();
        display.setCursor(0,0);
        display.print(operatorNum);
        display.print(operatorSign);
        clearBoard = true;
        break;

       case 7:// divide operator
        operatorPressed(minindex);
        display.clearDisplay();
        display.setCursor(0,0);
        display.print(operatorNum);
        display.print(operatorSign);
        clearBoard = true;
        break;
 
       
    }
    previndex = minindex;
    timeGap = millis();
    display.display();
}
else{
  previndex = -1;
}
#ifdef CALIBRATION

  // Servohorns will have 90° between movements, parallel to x and y axis
  drawTo(-3, 29.2);
  delay(500);
  drawTo(74.1, 28);
  delay(500);

#else 
  if(EQUALS_CLICKED){//for operator numbers
    if (!servo1.attached()) servo1.attach(SERVOPINLIFT);
    if (!servo2.attached()) servo2.attach(SERVOPINLEFT);
    if (!servo3.attached()) servo3.attach(SERVOPINRIGHT);

    wipe();
    armDraw(Ans);
    EQUALS_CLICKED = false;
  }
  else if(repeat && millis() > lastTouch + 3000){//when EQUALS is clicked, for ANS
    if (!servo1.attached()) servo1.attach(SERVOPINLIFT);
    if (!servo2.attached()) servo2.attach(SERVOPINLEFT);
    if (!servo3.attached()) servo3.attach(SERVOPINRIGHT);

    wipe();
    armDraw(operatorNum);
    lastTouch = millis();
    repeat = false;
  }
  else if(clearBoard){//to clear board
    if (!servo1.attached()) servo1.attach(SERVOPINLIFT);
    if (!servo2.attached()) servo2.attach(SERVOPINLEFT);
    if (!servo3.attached()) servo3.attach(SERVOPINRIGHT);
    
    wipe();
    //wipe();
    clearBoard = false;
    drawTo(defaultPosX,defaultPosY);
    lift(1);
  }
  else{
    drawTo(defaultPosX,defaultPosY);
    lift(1);
  }
    display.setCursor(0,0);
    equalButton();
    incrementButton();
    decrementButton();
    servo1.detach();
    servo2.detach();
    servo3.detach();

#endif

  }
