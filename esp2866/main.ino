#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "html.h"

const char *ssid = "StelarNetwork";
const char *password = "star1966trek";

ESP8266WebServer server ( 80 );

const int led = 13;

String temperature = "";
String humidity = "";
String motion;
String sound;
String luminosity;

void ReadCommand(char* command, unsigned lenght){
  long unsigned time = millis();
  while(!Serial.available() and (millis()-time < 1000)){}
  if(Serial.available()){
    char symbol = (char)Serial.read();
    unsigned count(0);
    while(symbol != '\n' and count < lenght){
      delay(10);
      if(Serial.available()){
        command[count]=symbol;
        symbol = char(Serial.read());
        ++count;
      }
    }
    command[count]='\0';
  }else{
    strncpy(command,"", lenght);
  }
}

int ReadInt(){
  unsigned char buf[sizeof(int)];
  long unsigned time = millis();
  while(!Serial.available() and (millis()-time < 10000)){}
  if(Serial.available()){
    byte symbol = (unsigned char)Serial.read();
    unsigned count(0);
    while(((char)symbol) != '\n' and count < sizeof(int)){
      delay(10);
      if(Serial.available()){
        buf[count] = (unsigned char)Serial.read();
        ++count;
      }
    }
    return(*(int *)buf);
  }else{
    return(-1);
  }
}

void drawGraph() {
	String out = "";
	char temp[600];
	out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
 	out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
 	out += "<g stroke=\"black\">\n";
 	int y = rand() % 130;
 	for (int x = 10; x < 390; x+= 10) {
 		int y2 = rand() % 130;
 		sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
 		out += temp;
 		y = y2;
 	}
	out += "</g>\n</svg>\n";

	server.send ( 200, "image/svg+xml", out);
}

void LightOn(){
  Serial.print("ligth_on\n");
	server.send ( 200, "text/plain", "Light On");
}

void LightOff(){
  Serial.print("ligth_off\n");
	server.send ( 200, "text/plain", "Light Off");
}

void LightSleep(){
  Serial.print("ligth_off\n");
	server.send ( 200, "text/plain", "sleep");
}

void UpTime(){
  int sec = millis() / 1000;
	int min = sec / 60;
	int hr = min / 60;
	char temp[20];
  char str[] = "%02d:%02d:%02d";
	snprintf(temp,20,str,hr, min % 60, sec % 60);
	server.send( 200, "text/plain", temp);
}


void ReadTemperature(){
	server.send( 200, "text/plain", temperature);
}

void ReadHumidity(){
	server.send( 200, "text/plain", humidity);
}

void ReadMotion(){
	server.send( 200, "text/plain", motion);
}

void ReadSound(){
	server.send ( 200, "text/plain", sound);
}

void ReadLuminosity(){
	server.send ( 200, "text/plain", luminosity);
}

void GetSensorsData(){
  Serial.print("temp\n");
  if(Serial.find("t")){
    temperature = String(Serial.parseFloat());
  }
  Serial.print("humidity\n");
  if(Serial.find("h")){
    humidity = String(Serial.parseFloat());
  }
  Serial.print("motion\n");
  if(Serial.find("m")){
    motion = String(Serial.parseInt());
  }
  Serial.print("sound\n");
  if(Serial.find("s")){
    sound = String(Serial.parseInt());
  }
  Serial.print("ldr\n");
  if(Serial.find("l")){
    luminosity = String(Serial.parseInt());
  }
}

void handleRoot() {
	digitalWrite ( led, 1 );
  server.send ( 200, "text/html", main_html);
	digitalWrite (led, 0);
}

void handleNotFound() {
	digitalWrite ( led, 1 );
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";

	for ( uint8_t i = 0; i < server.args(); i++ ) {
		message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
	}

	server.send ( 404, "text/plain", message );
	digitalWrite ( led, 0 );
}



void setup ( void ) {
	pinMode ( led, OUTPUT );
	digitalWrite ( led, 0 );
	Serial.begin ( 115200);
	WiFi.begin ( ssid, password );
	Serial.println ( "" );

	// Wait for connection
	while ( WiFi.status() != WL_CONNECTED ) {
		delay ( 500 );
		Serial.print ( "." );
	}

	Serial.println ( "" );
	Serial.print ( "Connected to " );
	Serial.println ( ssid );
	Serial.print ( "IP address: " );
	Serial.println ( WiFi.localIP() );

	if ( MDNS.begin ( "esp8266" ) ) {
		Serial.println ( "MDNS responder started" );
	}

	server.on("/",handleRoot);
	server.on("/test.svg",drawGraph);
	server.on("/light_on",LightOn);
	server.on("/light_off",LightOff);
	server.on("/light_sleep",LightSleep);
	server.on("/temperature",ReadTemperature);
	server.on("/humidity",ReadHumidity);
	server.on("/motion",ReadMotion);
	server.on("/sound",ReadSound);
	server.on("/luminosity",ReadLuminosity);
	server.on("/up_time",UpTime);
	server.onNotFound ( handleNotFound );
	server.begin();
	Serial.println ( "HTTP server started" );
}

void loop ( void ) {
	server.handleClient();
  if(millis() % 300 == 0){
    GetSensorsData();
  }
}
