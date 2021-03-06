#include <phys253.h>
#include <LiquidCrystal.h>

int leftDark = 20;
int rightDark = 20;
int motorLeft = 2;
int motorRight = 3;


double current_time = 0;
double prev_time = 0;

double error = 0;
double prev_error = 0;  

double pid;

int default_speed = 800;

int gain = 1;
int derivative = 0;
int k_p = 0;
int k_d = 0;

boolean p_disp = false;
boolean gain_disp = false;
boolean d_disp = true;
boolean leftError = true;

void setup(){
  #include <phys253setup.txt>
}


void loop() {

  while(!(startbutton())){
    LCD.clear(); LCD.home();
    LCD.print("Press Start");
    delay(100);
    }
  delay(500);
  while(!(startbutton())) {

     double valuePID = knob(7);
      k_p = valuePID/6;
      LCD.setCursor(0,1);
      LCD.clear();
      LCD.print(String("K_p:") + k_p);
      delay(100);
  }
  delay(500);
  while(!(startbutton())) {

     double valuePID = knob(7);
      k_d = valuePID/6;
      LCD.setCursor(0,1);
      LCD.clear();
      LCD.print(String("K_d:") + k_d);
      delay(100);
  }
  delay(500);
  /*
  while(!(startbutton())) {

     double valuePID = knob(7);
      leftDark = valuePID/32;
      LCD.setCursor(0,1);
      LCD.clear();
      LCD.print(String("leftDark: ") + leftDark);
      delay(100);
  }
  delay(500);

  while(!(startbutton())) {

     double valuePID = knob(7);
      rightDark = valuePID/32;
      LCD.setCursor(0,1);
      LCD.clear();
      LCD.print(String("rightDark: ") + rightDark);
      delay(100);
  }
  delay(500);
*/
  while(!(startbutton())) {

     double valuePID = knob(7);
      default_speed = valuePID;
      LCD.setCursor(0,1);
      LCD.clear();
      LCD.print(String("speeeeed: ") + default_speed);
      delay(100);
  }
  delay(500);
/*
  while(!(startbutton())) {

     double valuePID = knob(7);
      gain = valuePID/6;
      LCD.setCursor(0,1);
      LCD.clear();
      LCD.print(String("gain:") + gain);
      delay(100);
  }
    delay(500);
*/
    
  

    prev_time = micros();
    while(!(stopbutton()) && !(startbutton())){
      int sensorLeft = analogRead(2);
      int sensorRight = analogRead(3);
      current_time = micros();
      LCD.clear();
      LCD.setCursor(0,0); 
      LCD.print((int)sensorLeft + String(" ") + (int)sensorRight + String(" ") + error + String(" ") + derivative);
       
      LCD.setCursor(0,1);

      
      //error = (sensorRight - rightDark) + (leftDark - sensorLeft);
      if (sensorRight<rightDark && !(sensorLeft<leftDark)){
        leftError = false;
        error = rightDark - sensorRight;
      }
      else if ( !(sensorRight<rightDark) && sensorLeft<leftDark){
        leftError = true;
        error = sensorLeft - leftDark;
      }
      else if (sensorRight<rightDark && sensorLeft<leftDark){
        if(leftError){
          error = (sensorRight - rightDark) + (sensorLeft - leftDark) ;
        }
        else{
          error = (rightDark - sensorRight) + (leftDark - sensorLeft);
        }
      }
      else{
        error = 0;
      }
      derivative = 100000*(error - prev_error)/(current_time - prev_time);

      pid = gain * (k_p*error + k_d*derivative);

      prev_error = error;
      prev_time = current_time;
    
      if( pid < 0 ){
        motor.speed(motorLeft,default_speed);
        motor.speed(motorRight,default_speed + pid);
        LCD.print(default_speed + String(" ") + (default_speed+pid));
              
      }
      else{
        motor.speed(motorLeft,default_speed - pid);
        motor.speed(motorRight,default_speed);
        LCD.print((default_speed-pid) + String(" ") + (default_speed));
      }
      delay(50);

    
    }

  LCD.clear(); LCD.print("reeeeeee");

  motor.stop_all();
  delay(100);
}
