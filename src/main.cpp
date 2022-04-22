#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Stepper-master/src/Stepper.h>
// initialize the library with the numbers of the interface pins
int rs = 8, en=9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
// define some values used by the panel and buttons
int lcd_key = 0;
#define btnUP 0
#define btnDOWN 1
#define btnRIGHT 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5

#define modeStart 10
#define modeDebug 20
#define modeIR 30

#define sensorIR A0

#define modeCM 40
#define modeCM_run 45

#define modePM 50

const int STEPS_PER_REV = 2048;
float angle = 0;
unsigned long start, current;

Stepper steppermotor(32, 13, 11, 12, 3);

int currentMode = 10;
//Setting up arduino
void setup() {
  start = millis();
  lcd.begin(16, 2);
  Serial.begin(9600);
  steppermotor.setSpeed(400);
  
}

// The main loop program
void loop() {
  
  lcd.setCursor(0,1);  
  lcd_key = read_LCD_buttons();
  
  switch(currentMode) {
    case modeStart:
      startMode();
      break;
    case modeDebug:
      debugMode();
      break;
    case modeIR:
      irMode();
      break;
    case modeCM:
      cmMode();
      break;
    case modeCM_run:
      cmModeRun();
      break;
    case modePM:
      pmMode();
      break;

  }
}


unsigned long previousMillis = millis();
bool blink = true;
int selection = 1;
int speed = 2;

// DEBUG MODE 
void debugMode() {
  lcd.setCursor(0,0);
  lcd.print("Debug Mode                                ");
  lcd.setCursor(0,1);
  if (millis() - previousMillis > 400){
    blink = !blink;
    previousMillis = millis();
  }
  
  switch(selection) {
    case 1: 
      if (blink) {
        lcd.print("IR CM PM");
      } else {
        lcd.print("__ CM PM");
      }
      break;
    case 2: 
      if (blink) {
        lcd.print("IR CM PM");
      } else {
        lcd.print("IR __ PM");
      }
      break;
    case 3: 
      if (blink) {
        lcd.print("IR CM PM");
      } else {
        lcd.print("IR CM __");
      }
      break;
  }
  
  if(read_LCD_buttons() == btnRIGHT && selection < 3) {
    selection++;
    delay(100);
  }
  
  if(read_LCD_buttons() == btnLEFT && selection > 1) {
    selection--;
    delay(100);
  }
  
  if(read_LCD_buttons() == btnSELECT && selection == 1) {
    currentMode = modeIR;
    delay(200);
  }
    if(read_LCD_buttons() == btnSELECT && selection == 2) {
    currentMode = modeCM;
    delay(100);
  }
    if(read_LCD_buttons() == btnSELECT && selection == 3) {
    currentMode = modePM;
    delay(100);
  }
}


// START MODE
void startMode() {
  current = millis();
  int time = (current - start)/1000;
  lcd.setCursor(0,0);
  lcd.print(time);
  lcd.setCursor(0,1);
  lcd.print("Raj                    ");
  if(read_LCD_buttons() == btnSELECT) {
    currentMode = modeDebug;
    delay(100);
  }
} 


//PM MODE
void pmMode(){
  float turn = (angle/360)*2048;
  int count = 0;
  Serial.println(turn);
  lcd.setCursor(0,0);
  lcd.print("PM Mode                     ");
  lcd.setCursor(0,1);
  lcd.print("angle: ");

  if(read_LCD_buttons() == btnUP){
    angle = angle + 15;
    lcd.print(angle);
    delay(200);
  }
  if(read_LCD_buttons() == btnDOWN){
    angle = angle - 15;
    lcd.print(angle);
    delay(200);
  }
  if(read_LCD_buttons() == btnRIGHT){
    while((read_LCD_buttons() != btnSELECT) && count < turn){
      steppermotor.setSpeed(700);
      steppermotor.step(1);
      count++;
    }
  if(read_LCD_buttons() == btnSELECT){
    currentMode = modeDebug;
    delay(100);
  }
  }
  if(read_LCD_buttons() == btnLEFT){
    angle = 0;
    lcd.print(angle);
    delay(100);
  }
    
}

// IR MODE
void irMode() {
  lcd.setCursor(0,0);
  lcd.print("IR Mode    ");
  lcd.setCursor(0,1);
  float sensor = analogRead(2);
  float dist = 15*(sensor/560);
  lcd.print(dist);
  lcd.print("cm                       ");
  delay(2000);
  if(read_LCD_buttons() == btnSELECT) {
    currentMode = modeDebug;
    delay(100);
  }
}
//CM MODE
void cmMode(){
  //menu options display
  lcd.setCursor(0,0);
  lcd.print("CM Mode   ");
  lcd.setCursor(0,1);
  if (millis() - previousMillis > 400){
    blink = !blink;
    previousMillis = millis();
  }

  switch(selection) {
    case 1: 
      if (blink) {
        lcd.print("Start  Exit");
      } else {
        lcd.print("____   Exit");
      }
      break;
    case 2: 
      if (blink) {
        lcd.print("Start  Exit");
      } else {
        lcd.print("Start  ____");
      }
      break;
    }

    if(read_LCD_buttons() == btnRIGHT && selection == 1) {
      selection++;
      delay(100);
    }
  
    if(read_LCD_buttons() == btnLEFT && selection == 2) {
      selection--;
      delay(100);
    }
    if(read_LCD_buttons() == btnSELECT && selection == 1) {
      currentMode = modeCM_run;
      delay(100);
    }
    if(read_LCD_buttons() == btnSELECT && selection == 2){
      currentMode = modeStart;
      delay(100);
    }
}

void cmModeRun(){
  lcd.setCursor(0,1);

  if(read_LCD_buttons() == btnLEFT){
    lcd.setCursor(0,1);
    lcd.print("CCW :           ");
    while(read_LCD_buttons() != btnRIGHT){
      if(read_LCD_buttons() == btnDOWN && speed == 2){
        speed--;
        steppermotor.setSpeed(100);
        steppermotor.step(1);
      }
      if(read_LCD_buttons() == btnDOWN && speed == 3){
        speed--;
        steppermotor.setSpeed(400);
        steppermotor.step(1);
        delay(100);
      }
      if(read_LCD_buttons() == btnDOWN && speed == 1){
        steppermotor.setSpeed(100);
        steppermotor.step(1);
      }
      if(read_LCD_buttons() == btnUP && speed == 1){
        speed++;
        steppermotor.setSpeed(400);
        steppermotor.step(1);
        delay(100);
      }
      if(read_LCD_buttons() == btnUP && speed == 2){
        speed++;
        steppermotor.setSpeed(700);
        steppermotor.step(1);
      }
      if(read_LCD_buttons() == btnUP && speed == 3){
        steppermotor.setSpeed(700);
        steppermotor.step(1);
      }
      steppermotor.step(1);
    }
  }
  if(read_LCD_buttons() == btnRIGHT){
    lcd.setCursor(0,1);
    lcd.print("CW :           ");
    while(read_LCD_buttons() != btnLEFT){
      if(read_LCD_buttons() == btnDOWN && speed == 2){
        speed--;
        steppermotor.setSpeed(100);
        steppermotor.step(-1);
      }
      if(read_LCD_buttons() == btnDOWN && speed == 3){
        speed--;
        steppermotor.setSpeed(400);
        steppermotor.step(-1);
        delay(100);
      }
      if(read_LCD_buttons() == btnDOWN && speed == 1){
        steppermotor.setSpeed(100);
        steppermotor.step(-1);
      }
      if(read_LCD_buttons() == btnUP && speed == 1){
        speed++;
        steppermotor.setSpeed(400);
        steppermotor.step(-1);
        delay(100);
      }
      if(read_LCD_buttons() == btnUP && speed == 2){
        speed++;
        steppermotor.setSpeed(700);
        steppermotor.step(-1);
      }
      if(read_LCD_buttons() == btnUP && speed == 3){
      steppermotor.step(-1);
    }
    steppermotor.step(-1);
    }
  }
  if(read_LCD_buttons() == btnSELECT){
    currentMode = modeCM;
    delay(100);
  }
}
// Button input read function
int read_LCD_buttons() {
  int adc = analogRead(A0);
  if(adc == 0) return btnRIGHT;
  if(adc>=206 && adc<=215) return btnUP;
  if(adc<=408 && adc>=405) return btnDOWN;
  if(adc==623) return btnLEFT;
  if(adc<=825 && adc>=822) return btnSELECT;
  return btnNONE; 
}