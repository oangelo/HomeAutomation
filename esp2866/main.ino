#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char *ssid = "StelarNetwork";
const char *password = "star1966trek";

ESP8266WebServer server ( 80 );

const int led = 13;

void ReadCommand(char* command, unsigned lenght){
  if(Serial.available()){
    char symbol = (char)Serial.read();
    unsigned count(0);
    while(symbol != '\n' and count < lenght){
      delay(100);
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

void handleRoot() {
	digitalWrite ( led, 1 );
	char temp[1000];
	int sec = millis() / 1000;
	int min = sec / 60;
	int hr = min / 60;
  char temperature[10] = "";
  char humidity[10] = "";
  Serial.print("temp\n");
  delay(100);
  ReadCommand(temperature, 10);
  Serial.print("humidity\n");
  delay(100);
  ReadCommand(humidity, 10);

	snprintf ( temp, 600, 
  "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>Home Automation</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
  <center>\
    <h1>System Painel</h1>\
  </center>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p>Temperature: %s C</p>\
    <p>Humidity: %s </p>\
    <button type=\"button\" onclick=\"loadDoc()\">Light on</button>\
    <script>\
    function loadDoc() {\
    var xhttp = new XMLHttpRequest();\
    xhttp.open(\"GET\", \"/light_on\", true);\
    xhttp.send();\
    }\
    </script>\
  <!--<img src=\"/test.svg\" />-->\
  </body>\
</html>",hr, min % 60, sec % 60, temperature, humidity);
	server.send ( 200, "text/html", temp);
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
	server.on("/inline",[]() {
		server.send (200,"text/plain","this works as well");
	} );
	server.onNotFound ( handleNotFound );
	server.begin();
	Serial.println ( "HTTP server started" );
}

void loop ( void ) {
	server.handleClient();
  delay(100);
}
