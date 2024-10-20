#include <Stepper.h>
#include "esp_task_wdt.h"



//STEPS PER REVOLUTIONS
int rev_step = 2048; //steps per revolution of 28-byj-48
int r1 = rev_step*10; //number of steps for joint 1 (spur reduction)
int r2 = r1; //subject to change based on shoulder reduction chosen
int r3 = rev_step*21.25; //number of steps for joint 3 (planetary reduction)
int revs[] = {r1,r2,r3,rev_step,rev_step}; //steps per revs in an array 
//STEPPER OBJECT(S)
Stepper base = Stepper(rev_step,23,21,22,19);
Stepper shoulder =  Stepper(rev_step,18,17,5,4);
Stepper elbow =  Stepper(rev_step,0,15,2,8);
//NEGLECT FOR NOW THE BELOW STEPPER OBJECTS
//Stepper wrist1=  Stepper(rev_step,11,9,10,13);
//Stepper wrist2 = Stepper(rev_step,11,9,10,13);


//BOOLEANS
bool firstRun = true;
//JOINT PINS 
int J1 = 36; //Joint 1 potentiometer
int J2 = 39; //Joint 2 potentiometer
int J3 = 34; //Joint 3 potentiometer
int J4 = 35; //Joint 4 potentiometer
int J5 = 32; //Joint 5 potentiometer
/*
INITIAL CONDITIONS: set everything equal to initial potentiometer readings so the motor doesn't move on start up
*/
int base_i = map(analogRead(J1), 0, 4096, 0, r1);
int shoulder_i = map(analogRead(J2),0, 4096, 0, r2);
int elbow_i = map(analogRead(J3),0, 4096, 0, r3);
//command variables [CHANGE]
//FILTERING (CHANGE ACCORDINGLY)
//update pass depending on motor behavior

//returns number of steps based on readings from potentiometer from base
//computes command for each joint on the arm
int base_j() {
  int base_pot = analogRead(J1);
  int steps = map(base_pot, 0, 4096, 0, rev_step);
  int base_now = steps-base_i;
  Serial.print("Base: ");
  Serial.print(base_now);
  base_i = steps;
  return base_now;
}

int shoulder_j() {
  int shldr_pot = analogRead(J2);
  int steps = map(shldr_pot, 0, 4096, 0, rev_step);
  int shldr_now = steps-shoulder_i;
  Serial.print(" Shoulder: ");
  Serial.print(shldr_now);
  shoulder_i = steps;
  return shldr_now;
}

int elbow_j() {
  int elbow_pot = analogRead(J2);
  int steps = map(elbow_pot, 0, 4096, 0, rev_step);
  int elbow_now = steps-elbow_i;
  Serial.print(" Elbow: ");
  Serial.print(elbow_now);
  elbow_i = steps;
  return elbow_now;
}





//a previous reading will read what the position was before any potentiol change in analog signal occurred. 
//program then calculates required steps from change in steps and applies difference in step command through whatever joint changed.

void posControl() {
  //GET INITIAL VALUES 
    //Serial.print("Getting command: ");
    for(int k=0; k<5; k++){
        //Serial.println("In switch statement.");
        switch(k){
          case 0: {
            base.step(-1*base_j());
            yield();
            break;
          }
         
          case 1: {
            shoulder.step(-1*shoulder_j());
            yield();
            break;
          }
          /*
          case 2: {
            elbow.step(-1*elbow_j());
            yield();
            break;
          }
          case 3: {
            wrist1.setSpeed(10);
            wrist1.step(step_final[k]);
            yield();
            Serial.println("Joint 4 activated");
            break;
          }
          case 4: {
            wrist2.setSpeed(10);
            wrist2.step(step_final[k]);
            yield();
            Serial.println("Joint 5 activated");
            break;
          }
          */
          }
          yield();
          //Serial.println("Commands Booted Successfully!");
  }
}
    

void setup() {
  //DEFINE PINS
  Serial.begin(115200);
  base.setSpeed(10);
  shoulder.setSpeed(10);
  elbow.setSpeed(10);
}

void loop() {
  // put your main code here, to run repeatedly:
  posControl();
}