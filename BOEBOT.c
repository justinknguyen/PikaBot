#include "simpletools.h"                      // Include simple tools
#include "adcDCpropab.h"
#include "servo.h"
#include "boebot.h"

int main()
{
  pause(1000);
  adc_init(21, 20, 19, 18);                   // CS=21, SCL=20, DO=19, DI=18 
  while(1)
  {
     HappyState();
     SadState();
     ScaredState();
     AngerState();
  }
  return 0;
}

///////////***EMOTION***///////////
void HappyState()
{
  pause(1000);
  while(1)
  {
    ///////SWITCH////////
    int button = input(11);
    print("button = %d\n", button);
    if (button == 1)
    {
      return;
    }
    ///////SWITCH////////  
    
    servo_speed(14,0);
    servo_speed(15,0);
    
    adc_init(21, 20, 19, 18);
    float v1;
  
    v1 = adc_volts(2);                  // Check A/D 1
    print("Sound Happy %f V\n", v1);                // Display volts
  
    if(v1 > 0.05)
    {
      InteractHappy();
    }      
    pause(100);                        // Wait 1/10 s
  }
  return;    
}  

void InteractHappy()
{
  servo_speed(14,200);
  servo_speed(15,200);
  high(26);
  high(27);
  pause(2000);
  low(26);
  low(27);
  return;
}

void SadState()
{
  pause(1000);
  while(1)
  {
    ///////SWITCH////////
    int button = input(11);
    print("button = %d\n", button);
    if (button == 1)
    {
      return;
    }
    ///////SWITCH//////// 
    
    high(3);
    pause(200);
    low(3);
    
    ////////PING/////////
    float i = -1;
    i = Ping(12,13);           
    i = i / 148;                      //divide the ping by 148 to get inches, or by 54 to get centimeters
    print("Ping Sad %f\n", i);
    if(i < 10)
    {
      //Turning the Robot 180 degrees
      servo_speed(14, 100);
      servo_speed(15, 100);
      pause(1200);
    
      servo_speed(14, 0);
      servo_speed(15, 0);
      pause(100);
  
      servo_speed(14, -50);
      servo_speed(15, 50);
    }
    else
    {
      servo_speed(14, -50);
      servo_speed(15, 50);
    }
    ////////PING/////////
    
    adc_init(21, 20, 19, 18);
    float v1;
  
    v1 = adc_volts(2);                  // Check A/D 1
    print("Sound Sad %f V\n", v1);                // Display volts
  
    if(v1 > 0.05)
    {
      InteractSad();
    }      
    pause(100); 
  } 
}  

void InteractSad()
{
  int *sadsens_cog = cog_run(sadsens, 128);
  int count = 0;
  
  while(count < 100)
  {
    ////////PING/////////
    float i = -1;
    i = Ping(12,13);           
    i = i / 148;                      //divide the ping by 148 to get inches, or by 54 to get centimeters
    print("Ping Sad %f\n", i);
    if(i < 10)
    {
      GameOver();
    }  
    ////////PING/////////
    count++;
  }   
  cog_end(sadsens);
}

void ScaredState()
{
  pause(1000);
  while(1)
  {
    ///////SWITCH////////
    int button = input(11); 
    print("button = %d\n", button);                   
    if (button == 1)
    {
      return;
    }
    ///////SWITCH////////
    
    float LDRVoltageValue;      
                   
    LDRVoltageValue = adc_volts(1);            // Check A/D 0
    print("LDR Value = %f \n", LDRVoltageValue); // Display LDR voltage value
    Jitter();                                 // Calling Jitter Function
    
    if (LDRVoltageValue < 3.2)                  // Checking to see LDR Value
    {
      InteractScared();
    }
  }
  return;
}

void InteractScared()
{
  LetsGetIt();
  
  //Turning the Robot 180 degrees
  servo_speed(14, 200);
  pause(3000);
  
  servo_speed(14, 0);                     // Move pin 14 motor by speed 0
  servo_speed(15, 0);                     // Move pin 15 motor by speed 0
  pause(1000);                            // Pause for 4 seconds
  return;
}

void AngerState()
{
  servo_speed(14,0);
  servo_speed(15,0);
  pause(2000);
  int *lightshow_cog = cog_run(lightshow, 128);
  while(1)
  {
    ///////SWITCH////////
    int button = input(11);
    if (button == 1)
    {
      print("button = %d\n", button);
      return;
    }   
    ///////SWITCH////////
    float flex = adc_volts(0);
    print("Flex = %f V\n", flex);
    if(flex>1.87 || flex<1.47)
    {
      cog_end(lightshow_cog);
      InteractAnger();
      lightshow_cog = cog_run(lightshow, 128);
      pause(2000);
    }
  }
  cog_end(lightshow_cog);
  return;
}

void InteractAnger()
{
int counter = 0;
  int *lightshowf_cog = cog_run(lightshowf, 128);
  int *backup_cog = cog_run(backup, 128);
  while(counter<100){
    float distance = distance_cm();
    if (distance < 10){
      counter=0;
      cog_end(lightshowf_cog);
      cog_end(backup_cog);
      attack();
      lightshowf_cog = cog_run(lightshowf, 128);
      backup_cog = cog_run(backup, 128);
      pause(2000);
    }
    counter++;
  }
  cog_end(lightshowf_cog);
  cog_end(backup_cog);
  servo_speed(14,0);
  servo_speed(15,0);
  return;
}
///////////***EMOTION***///////////

///////////***MISC***//////////////
int sadsens()
{
  while(1)
  {
    ////////PING/////////
    float i = -1;
    i = Ping(12,13);           
    i = i / 148;                      //divide the ping by 148 to get inches, or by 54 to get centimeters
    if(i < 10)
    {
      //Turning the Robot 180 degrees
      servo_speed(14, 100);
      servo_speed(15, 100);
      pause(1200);
    
      servo_speed(14, 0);
      servo_speed(15, 0);
      pause(100);
  
      servo_speed(14, -200);
      servo_speed(15, 200);
      pause(2000);
    }  
    ////////PING///////// 
  }    
}  

void LetsGetIt()
{
  int LightTime = 100;
  int FrequencyTime = 200;
  int FrequencyLevel = 500;
  
  int count = 0;              // Count value          
  int x = 100;
  
  while(x < 2000)
  {
    freqout(0,20,x);
    x = x + 10;
  }
                            
  while(count < 6)
  {
    freqout(0, FrequencyTime, FrequencyLevel);
    high(26);
    low(27);
    pause(LightTime);
    high(27);
    low(26);
    pause(LightTime);
    count++;
    LightTime = 75 + LightTime;
    FrequencyLevel = FrequencyLevel + 100;
    FrequencyTime = FrequencyTime + 100;
  }
  return;
}  
  
void Jitter()
{
  servo_speed(14, 100);                      // Move pin 14 motor by speed 100
  servo_speed(15, -100);                     // Move pin 15 motor by speed -100
  pause(200);                               // Pause for 0.2 seconds
  servo_speed(14, -100);                      // Move pin 14 motor by speed 100
  servo_speed(15, 100);                     // Move pin 15 motor by speed -100
  pause(200);                               // Pause for 0.2 seconds
  return;
}   

int Ping(int trig, int echo)//trig is trigger pin, echo is echo pin
{
  low(trig);//set trig low for start pulse
  low(echo);//set echo low to be safe
  pulse_out(trig, 10);//send the minimum 10 ms pulse on trig to start a ping
  return pulse_in(echo, 1);//get the pulse duration back on echo pin
}

float distance_cm(){
  float i = -1;
  float distancecm;
  i = Ping(12,13);
  distancecm = i/54;
  print("Distance %f\n", distancecm); //divide the ping by 148 to get inches, or by 54 to get centimeters
  return distancecm;
}

void attack(){
  int *lightshowff_cog = cog_run(lightshowff, 128);
  servo_speed(14,-200);
  servo_speed(15,200);
  pause(500);
  servo_speed(14,0);
  servo_speed(15,0);
  cog_end(lightshowff_cog);
  return;
}

void backup(){
  while(1){
    servo_speed(14,25);
    servo_speed(15,-10);
    pause(500);
    servo_speed(14,0);
    servo_speed(15,0);
    pause(500);
    servo_speed(14,10);
    servo_speed(15,-25);
    pause(500);
    servo_speed(14,0);
    servo_speed(15,0);
    pause(500);
  }
  return; 
}

void lightshow(){
 while(1){
   high(26);
   low(27);
   pause(500);  
   low(26);
   high(27);
   pause(500);    
 }
 return;
}

void lightshowf(){
 while(1){
   high(26);
   low(27);
   pause(200);
   low(26);
   high(27);
   pause(200);
 }  
 return;
}

void lightshowff(){
 while(1){
   high(26);
   low(27);
   pause(50);
   low(26);
   high(27);
   pause(50);
 }
 return; 
}
///////////***MISC***//////////////

///////////***MUSIC***/////////////


void GameOver()
{
  int *game_cog1 = cog_run(game1,128);  
  int *game_cog2 = cog_run(game2,128);
  pause(10000);
  cog_end(game_cog1);
  cog_end(game_cog2);
}
int game1()
{
  while(1)
  {
   int pin0 = 0;
   freqout(pin0,EN,C7);
   pause(EN);
   pause(EN);
   freqout(pin0,EN,G6);
   pause(EN);
   freqout(pin0,EN,E6);
   pause(EN);
   freqout(pin0,EN,F6);
   freqout(pin0,EN,B6);
   freqout(pin0,EN,A6);
   pause(EN);
   
   freqout(pin0, QN, F8);/*****//// play this and next line together
   freqout(pin0, EN, Gs8Ab8);/*****////
   freqout(pin0, EN, As8Bb8);
   freqout(pin0, EN, Gs8Ab8);
   freqout(pin0, HN, G8);
   freqout(pin0, EN, D8);
   freqout(pin0, QN, E8);
  }    
} 
int game2() 
{
  while(1)
  {
   int pin6=6;
   freqout(pin6,EN,G8);
   pause(EN);
   pause(EN);
   freqout(pin6,EN,E8);
   pause(EN);
   freqout(pin6,EN,C8);
   pause(EN);
   freqout(pin6,QN,F8);
   pause(QN);
   
   freqout(pin6, HN, Cs7Db7);
   freqout(pin6, HN, C7);
  }    
}
///////////***MUSIC***/////////////
