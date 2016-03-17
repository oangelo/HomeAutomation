#include "DHT.h"

#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

char command[10]; //the command received by serial
char aux_command[10]; // hold incomplete command
unsigned cmd_count; //count the received chars

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  //Serial.println("Home Automation System");
  strncpy(command,"",10);
  strncpy(aux_command,"",10);
  cmd_count = 0;
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

void loop() {
  delay(100);
  ReadCommand();
  //Execute a command
  if(strcmp(command, "") != 0){
    if(strcmp(command, "temp") == 0){
      float t = dht.readTemperature();
      Serial.println(t);
    } else if(strcmp(command, "humidity") == 0){
      float h = dht.readHumidity();
      Serial.println(h);
    }
    strncpy(command,"",10);
  }
}
