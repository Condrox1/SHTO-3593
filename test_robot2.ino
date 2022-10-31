#include <IRremote.h>

//LoopDEFINE
////////////////////////////////
int  RECV_PIN   = 12;
int  CODE_START = 0xFF18E7;
int  CODE_STOP  = 0XFF4AB5;
bool RUNNING    = 0;
IRrecv irrecv(RECV_PIN);
decode_results results;
////////////////////////////////

////////MotorDEFINE/////////
#define Motor1_1 11
#define Motor1_2 10
#define Motor2_3 9
#define Motor2_4 6
////////MotorDEFINE/////////

//Power given to the motors
///////////////////////////////
#define MOTOR_POWER 255 
#define MOVE_BACK   9   // 1001
#define MOVE_FRONT  6   // 0110
#define MOVE_LEFT   10  // 1010
#define MOVE_RIGHT  5   // 0101
///////////////////////////////


//////////SensorsDEFINE//////////
#define OP_FRONT_POW  0 //BROWN
#define OP_FRONT_OUT A0 //BLACK 

#define OP_LEFT_POW   0 //BROWN 
#define OP_LEFT_OUT  A1 //BLACK

#define OP_RIGHT_POW  0 //BROWN
#define OP_RIGHT_OUT A2 //BLACK

#define AR_LEFT_GND   5
#define AR_LEFT_POW   7 
#define AR_LEFT_OUT   8 

#define AR_RIGHT_GND  4 // WHITE
#define AR_RIGHT_POW  3 // GREY
#define AR_RIGHT_OUT  2 // VIOLET
//////////SensorsDEFINE//////////

void KILLSWITCH()
{
    analogWrite(Motor1_1,0);
    analogWrite(Motor1_2,0);
    analogWrite(Motor2_3,0);
    analogWrite(Motor2_4,0);
}

bool Receiver()
{   
    //Give power to Receiver 
    digitalWrite(13,HIGH);
    
    //Read Data
    if(irrecv.decode(&results))
    {
       int readResults = results.value;
        
       if(readResults == CODE_START){
            RUNNING=1;
            Serial.print("start");
        }
        
       else if(readResults==CODE_STOP)
        { 
            //Kill the motors
            RUNNING=0;
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

void Search_ENEMY()
{
delay(250);
KILLSWITCH();    
}

void ATTACK()
{
    bool F = !analogRead(OP_FRONT_OUT);
    bool L = !analogRead(OP_LEFT_OUT);
    bool R = !analogRead(OP_RIGHT_OUT);
    
    /*Serial.print(F);
    Serial.print(L);
    Serial.print(R);
    Serial.println(" ");*/   
    
    if(F)
     ACTION_MOTORS(MOVE_FRONT);
    else if(R)
     ACTION_MOTORS(MOVE_RIGHT);
    else if(L)
     ACTION_MOTORS(MOVE_LEFT);
    else 
        Search_ENEMY();
    
    
}

void BACKOFF(int Command)
{
    KILLSWITCH();
    delay(50);
    ACTION_MOTORS(MOVE_BACK);
    delay(100);
    KILLSWITCH();
    ACTION_MOTORS(Command);
    delay(200);
    KILLSWITCH();
}

bool Search_ARENA()
{
    bool L= !digitalRead(AR_LEFT_OUT);
    bool R= !digitalRead(AR_RIGHT_OUT);
    Serial.print(L);
    Serial.print(R);
    Serial.println(" ");
   /* bool InArena= L||R;    
    if(InArena)
    {
        if(!R)
        {
            Serial.println("RIGHT");
            //BACKOFF(MOVE_LEFT);
        }
         else Serial.println("LEFT"); 
        //BACKOFF(MOVE_RIGHT);        
    }*/
    delay(100);
    return 0 ;
}

void setup()
{
    Serial.begin(9600);
    irrecv.enableIRIn();
    pinMode(13,OUTPUT);

////////////SENSORS_SETUP/////////////
    //LEFT
    pinMode(AR_LEFT_GND,OUTPUT);
    pinMode(AR_LEFT_OUT,INPUT);
    pinMode(AR_LEFT_POW , OUTPUT);
    digitalWrite(AR_LEFT_POW,HIGH);
    //RIGHT
    pinMode(AR_RIGHT_GND,OUTPUT);
    pinMode(AR_RIGHT_OUT,INPUT);
    pinMode(AR_RIGHT_POW , OUTPUT);
    digitalWrite(AR_RIGHT_POW,HIGH);
////////////SENSORS_SETUP/////////////
}

// Read the command and apply the POWER
int SetPower(int Binary,int Position){
    return MOTOR_POWER*((Binary>>Position)&1);
}

void ACTION_MOTORS(int Command)
{
    analogWrite(Motor1_1,SetPower(Command,3));
    analogWrite(Motor1_2,SetPower(Command,2));
    analogWrite(Motor2_3,SetPower(Command,1));
    analogWrite(Motor2_4,SetPower(Command,0));
}


void loop()
{    
    if(1)
    {
       if(Search_ARENA())
        {
           // ATTACK();
        }
    }
    
    
}