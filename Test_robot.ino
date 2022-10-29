#include <IRremote.h>

//LoopDEFINE
int  RECV_PIN   = 12;
int  CODE_START = 0xFF18E7;
int  CODE_STOP  = 0XFF4AB5;
bool RUNNING    = 0;
IRrecv irrecv(RECV_PIN);
decode_results results;

//MotorDEFINE
#define Motor1_1 11
#define Motor1_2 10
#define Motor2_3 9
#define Motor2_4 6

//Power given to the motors
int MOTOR_POWER = 255; 
int MOVE_BACK   = 9 ;  // 1001
int MOVE_FRONT  = 6 ;  // 0110
int MOVE_LEFT   = 10;  // 1010
int MOVE_RIGHT  = 5 ;  // 0101

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
            analogWrite(Motor1_1,0);
            analogWrite(Motor1_2,0);
            analogWrite(Motor2_3,0);
            analogWrite(Motor2_4,0);
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


void Search_ATTACK()
{
    
}

bool Search_ARENA()
{
    return 1;
}

void setup()
{
    Serial.begin(9600);
    irrecv.enableIRIn();
    pinMode(13,OUTPUT);
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
    if(Receiver())
    {
       ACTION_MOTORS(MOVE_FRONT);
    }
    
    
}
