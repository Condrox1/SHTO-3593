#include <IRremote.h>

//////////////////////////////////////////////////////
class CLASS_MOTORS
{
 int POWER; int PIN; int INDEX;
 // Read the command and apply the POWER
 int Get_index(int Binary, int Position) {
 return POWER * ((Binary >> Position) & 1); }

 public:
 CLASS_MOTORS(int _PIN,int _POWER,int _INDEX)
 {PIN=_PIN;POWER=_POWER;INDEX=_INDEX;}
 CLASS_MOTORS()
 {PIN=0;POWER=0;INDEX=0;}

 void Set_MOTOR(int _COMMAND){
 analogWrite(PIN, Get_index(_COMMAND, INDEX));
 }

};
//////////////////////////////////////////////////////

/////MOTORS/////
CLASS_MOTORS Motor1_1;
CLASS_MOTORS Motor1_2;
CLASS_MOTORS Motor2_3;
CLASS_MOTORS Motor2_4;
//Power given to the motors
#define MOVE_FRONT  9   // 1001
#define MOVE_BACK   6    // 0110
#define MOVE_RIGHT 10  // 1010
#define MOVE_LEFT   5    // 0101

void ACTION_MOTORS(int Command) 
{
  Motor1_1.Set_MOTOR(Command);
  Motor1_2.Set_MOTOR(Command);
  Motor2_3.Set_MOTOR(Command);
  Motor2_4.Set_MOTOR(Command);
  
}
/////MOTORS/////


//////////////////////////////////////////////////////
class ARENA_SENSORS
{
  int GND_L;int POW_L;int OUT_L;int POWER_VAL=130;
  int GND_R;int POW_R;int OUT_R;
  void setPINS()
  {
    pinMode(GND_L, OUTPUT);pinMode(OUT_L, INPUT);
    pinMode(POW_L, OUTPUT);pinMode(GND_R, OUTPUT);
    pinMode(OUT_R, INPUT);pinMode(POW_R, OUTPUT);
  }

  public:
  ARENA_SENSORS()
  {GND_L=0;POW_L=0;OUT_L=0;
  GND_R=0;POW_R=0;OUT_R=0;}
  ARENA_SENSORS(int _GNDL,int _POWL,int _OUTL,
  int _GNDR,int _POWR,int _OUTR)
  {GND_L=_GNDL;POW_L=_POWL;OUT_L=_OUTL;
   GND_R=_GNDR;POW_R=_POWR;OUT_R=_OUTR;
  setPINS();}

  bool getData(){
    analogWrite(POW_L,POWER_VAL);
    analogWrite(POW_R,POWER_VAL);

    delay(100);
    bool L= !digitalRead(OUT_L);
    bool R= !digitalRead(OUT_R);
    delay(200);
    
    analogWrite(POW_L,0);
    analogWrite(POW_R,0);

/*
    if((L&&R))
      {
        ACTION_MOTORS(MOVE_FRONT);
        return 1;
      }
    
    {

      if(!R)
      BACKOFF(MOVE_LEFT);
      else BACKOFF(MOVE_RIGHT);

    }
*/    
    }
  
};
//////////////////////////////////////////////////////

/////////////////////////////////////////////
class CLASS_DSENSORS
{
  int R_OUT ;int L_OUT,F_OUT;
  int POWER; int P_VALUE=255;
  void setPins()
  {
    pinMode(F_OUT, INPUT);pinMode(L_OUT, INPUT);
    pinMode(R_OUT, INPUT);pinMode(POWER,OUTPUT);
  }

  public:
  CLASS_DSENSORS()
  {
    R_OUT=0;L_OUT=0;F_OUT=0;
    POWER=0;
  }

  CLASS_DSENSORS(int _POW,int _F_OUT,
                int _L_OUT,int _R_OUT)
  { 
    R_OUT=_R_OUT;L_OUT=_L_OUT;
    F_OUT=_F_OUT;POWER=_POW;
    analogWrite(POWER, P_VALUE);
    setPins();
  }
  

};

//////PREDEFINE_EVERYTHING//////
/////MOTORS/////

/////SENSORS/////
ARENA_SENSORS COLOR;
//////PREDEFINE_EVERYTHING//////

//portocaliu rosu mov - gnd vcc out
//////////LoopDEFINE//////////
int RECV_PIN = 12;
int CODE_START = 0xFF18E7;
int CODE_STOP = 0XFF4AB5;
bool RUNNING = 0;
IRrecv irrecv(RECV_PIN);
decode_results results;
//////////LoopDEFINE//////////





void KILLSWITCH() {
  Motor1_1.Set_MOTOR(0);
  Motor1_2.Set_MOTOR(0);
  Motor2_3.Set_MOTOR(0);
  Motor2_4.Set_MOTOR(0);
}

bool Receiver() {
  //Give power to Receiver
  digitalWrite(13, HIGH);

  //Read Data
  if (irrecv.decode(&results)) {
    int readResults = results.value;

    if (readResults == CODE_START) {
      RUNNING = 1;
      Serial.print("start");
    }

    else if (readResults == CODE_STOP) {
      //Kill the motors
      RUNNING = 0;
      KILLSWITCH();
      Serial.print("stop");
    }
    /*Serial.println(" ");
        Serial.print("Code: ");
        Serial.println(results.value, HEX);
        Serial.println(" ");*/
    irrecv.resume();
  }

  return RUNNING;
}

void Search_ENEMY() {

  delay(250);
  KILLSWITCH();
}

void ATTACK() {

  /* bool F = !analogRead(OP_FRONT_OUT);
  bool L = !analogRead(OP_LEFT_OUT);
  bool R = !analogRead(OP_RIGHT_OUT);

  Serial.print(F);
  Serial.print(L);
  Serial.print(R);
  Serial.println(" ");

  if (F && R && L)
    ACTION_MOTORS(MOVE_FRONT);
  else if (R)
    ACTION_MOTORS(MOVE_RIGHT);
  else if (L)
    ACTION_MOTORS(MOVE_LEFT);
  else if (F)
    ACTION_MOTORS(MOVE_FRONT);
  else
    Search_ENEMY();*/
}

void BACKOFF(int Command) {
  KILLSWITCH();
  delay(50);
  ACTION_MOTORS(MOVE_BACK);
  delay(200);
  KILLSWITCH();
  ACTION_MOTORS(Command);
  delay(200);
  ACTION_MOTORS(MOVE_FRONT);
  delay(300);
  KILLSWITCH();
}

bool Search_ARENA() {
  
  COLOR.getData();
  delay(100);
  return 0;
}



void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(13, OUTPUT);

  /////////////INIT_MOTORS/////////////
  ////CLASS_MOTORS(PIN,POWER,INDEX)////
  Motor1_1 = CLASS_MOTORS( 11, 255, 3); 
  Motor1_2 = CLASS_MOTORS( 10, 255, 2);
  Motor2_3 = CLASS_MOTORS(  9, 255, 1);
  Motor2_4 = CLASS_MOTORS(  6, 255, 0);
  /////////////INIT_MOTORS/////////////

  /////////////SENSORS_ARENA_SETUP//////////////
  ////ARENA_SENSORS(GND_PIN,POW_PIM,OUT_PIN)////
  COLOR = ARENA_SENSORS(5,A4,8,  //LEFT
                          2,A5,3); //RIGHT

  /////////////SENSORS_ARENA_SETUP//////////////

  /////SENSORS_ENEMY_SETUP///////
  CLASS_DSENSORS(A3,A0,A2,A1);
  /////SENSORS_ENEMY_SETUP///////

}

void loop() {
  if (Receiver()) {
    if (Search_ARENA()) {
      //ATTACK();
    }
  }
}