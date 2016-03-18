#include "DHT.h"

#define DHTPIN 2// digital pin for DHT-22 
#define SOUND_DETECT 3//Sound detection module digital pin
#define PIR1 4//Motion detection module digital pin
#define PIR2 5//Motion detection module ditgital pin
#define LDR 0//Light intensiti detector analog pin
#define LIGHT 13//Control light pin

#define SOUND_INTERVAL_FLAG 500
#define MOTION_INTERVAL_FLAG 500
#define LIGHT_THRESHOLD 850 //Light is on if LDR is below this value 

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

char command[10]; //the command received by serial
char aux_command[10]; // hold incomplete command
unsigned cmd_count; //count the received chars

unsigned long lastSoundDetectTime; 
bool sound_flag;

unsigned long LastMotionDetectTime; 
bool motion_flag;

unsigned long light_on_time;

void setup() {
  Serial.begin(115200);

  dht.begin();
  pinMode(SOUND_DETECT, INPUT);
  pinMode(PIR1, INPUT);
  pinMode(PIR2, INPUT);
  pinMode(LIGHT, OUTPUT);

  strncpy(command,"",10);
  strncpy(aux_command,"",10);
  cmd_count = 0;
  sound_flag = false;
}

void LightControl(){
  int ldr = analogRead(LDR);
  if(ldr > LIGHT_THRESHOLD and motion_flag){
    digitalWrite(LIGHT, HIGH);
    light_on_time = millis(); // record the time of the sound alarm
  }else if((millis() - light_on_time) > 20000 or ldr < LIGHT_THRESHOLD){
    //keep the light on for some time or until we detect high luminosity 
    digitalWrite(LIGHT, LOW);
  }
}

void ReadCommand(){
  if(Serial.available()){
    char symbol = char(Serial.read());
    if(symbol != '\n'){
      aux_command[cmd_count]=symbol;
      ++cmd_count;
    }else{
      command[cmd_count]='\0';
      strncpy(command, aux_command, 10);
      strncpy(aux_command,"",10);
      cmd_count = 0;
    }
  }
}

void MotionDetection(){
  int MotionDetectedVal = digitalRead(PIR1) or digitalRead(PIR2); // read the sound alarm time
  if(!MotionDetectedVal == LOW){ //If we hear a sound
    LastMotionDetectTime = millis(); // record the time of the sound alarm
      if (!motion_flag){
        motion_flag = true;
      }
  }
  else if( (millis()-LastMotionDetectTime) > MOTION_INTERVAL_FLAG && motion_flag){
      motion_flag = false;
  }
}

void SoundDetection(){
  int soundDetectedVal = digitalRead(SOUND_DETECT) ; // read the sound alarm time
  if(soundDetectedVal == LOW){ //If we hear a sound
    lastSoundDetectTime = millis(); // record the time of the sound alarm
      if (!sound_flag){
        sound_flag = true;
      }
  }
  else if( (millis()-lastSoundDetectTime) > SOUND_INTERVAL_FLAG &&  sound_flag){
      sound_flag = false;
  }
}

void loop() {
  delay(10);
  LightControl();
  SoundDetection();
  MotionDetection();
  ReadCommand();
  if(strcmp(command, "") != 0){
    if(strcmp(command, "temp") == 0){
      float t = dht.readTemperature();
      Serial.print(t);
      Serial.print('\n');
    } else if(strcmp(command, "humidity") == 0){
      float h = dht.readHumidity();
      Serial.print(h);
      Serial.print('\n');
    }else if(strcmp(command, "sound") == 0){
      Serial.print(sound_flag);
      Serial.print('\n');
    }else if(strcmp(command, "ldr") == 0){
      int ldr = analogRead(LDR);
      Serial.print(ldr);
      Serial.print('\n');
    }else if(strcmp(command, "motion") == 0){
      Serial.print(motion_flag);
      Serial.print('\n');
    }
    strncpy(command,"",10);
  }
}
