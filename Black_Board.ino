#include <NewPing.h>
int line_l = 0;
int line_r = 0;
int motorRight1 = 0;
int motorRight2 = 0;
int motorLeft1 = 0;
int motorLeft2 = 0; 
void setup() {
pinMode(A4,INPUT);
pinMode(A5,INPUT);
pinMode(motorRight1,OUTPUT);
pinMode(motorRight2,OUTPUT);
pinMode(motorLeft1,OUTPUT);
pinMode(motorLeft2,OUTPUT);
}
void robotStop(){
analogWrite(motorRight1,0);
analogWrite(motorRight2,0);
analogWrite(motorLeft1,0);
analogWrite(motorLeft2,0);
}
void sense_floor_black(){
line_l = digitalRead(A4);
line_r = digitalRead(A5);

if((line_l == LOW) && (line_r == LOW))
{
analogWrite(motorRight1,0);
analogWrite(motorRight2,250);
analogWrite(motorLeft1,0);
analogWrite(motorLeft2,250);
delay(500);
robotStop();
}
else if (line_l == LOW){
analogWrite(motorRight1,0);
analogWrite(motorRight2,250);
analogWrite(motorLeft1,0);
analogWrite(motorLeft2,250);
delay(500);
analogWrite(motorRight1,0);
analogWrite(motorRight2,250);
analogWrite(motorLeft1,250);
analogWrite(motorLeft2,0);
delay(200);
robotStop();


}
else
{
analogWrite(motorRight1,0);
analogWrite(motorRight2,250);
analogWrite(motorLeft1,0);
analogWrite(motorLeft2,250);
delay(300);
analogWrite(motorRight1,250);
analogWrite(motorRight2,0);
analogWrite(motorLeft1,0);
analogWrite(motorLeft2,250);
delay(200);
robotStop();



}
}
void loop() {
 sense_floor_black();

}
