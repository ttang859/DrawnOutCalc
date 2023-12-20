
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(128, 64);

#define LED 2
#define B0 34
#define B1 0
#define B2 35

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(B0, INPUT);
  pinMode(B1, INPUT);
  pinMode(B2, INPUT);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Screen Initialized");
  display.display();  
}

const int pins[] = {T0, T3, T4, T5, T6, T7, T8, T9};

#define TOUCH_THRESHOLD  25
int previndex = -1;
unsigned long timeGap = millis();
double operatorNum = 0.0;
double Ans = 0.0;
char operatorSign = ' ';

void loop() {
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
    //digitalWrite(LED, HIGH);
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
        break;
        
       case 1://set Ans to operatorNum to chain computing
        operatorNum = Ans;
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
        break;
        
       case 3://shifts a digit to the left 
        operatorNum/=10.0;
        display.clearDisplay();
        display.setCursor(0,0);
        display.print(operatorNum);
        display.display();
        break;
//****IMPLEMENT THE ACTUAL MATHEMATICAL OPERATION EXECUTION****
       case 4:// add operator
        operatorPressed(minindex);
        display.clearDisplay();
        display.setCursor(0,0);
        display.print(operatorNum);
        display.print(operatorSign);
        break;

       case 5:// subtract operator
        operatorPressed(minindex);
        display.clearDisplay();
        display.setCursor(0,0);
        display.print(operatorNum);
        display.print(operatorSign);
        break;

       case 6:// multiply operator
        operatorPressed(minindex);
        display.clearDisplay();
        display.setCursor(0,0);
        display.print(operatorNum);
        display.print(operatorSign);
        break;

       case 7:// divide operator
        operatorPressed(minindex);
        display.clearDisplay();
        display.setCursor(0,0);
        display.print(operatorNum);
        display.print(operatorSign);
        break;
 
       
    }
    previndex = minindex;
    timeGap = millis();
    display.display();
  } else {
    digitalWrite(LED, LOW);
    previndex = -1;
  }

  //will use 3 buttons (1 for increment, 1 for decrement, and 1 for equals)
  display.setCursor(0,0);
  equalButton();
  incrementButton();
  decrementButton();
  //digitShifting();
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
      operatorNum = 0.0;//change later, prob substitute for a function that contains all the mathematical operations using a switch case
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
