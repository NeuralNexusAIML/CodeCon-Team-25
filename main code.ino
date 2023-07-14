//headdders
#include<ESP8266WiFi.h>
#include<stdio.h>
#include <WiFiClientSecure.h>
#include<ESP8266HTTPClient.h>
#include "DHT.h"
//wifi password and id
char wifi_id[]="dengerous";
char password[]="vaheed143"; 

// web connections
WiFiClientSecure client;
HTTPClient http;
String url="https://iot-based-waste-monitering-system2.onrender.com/updatedb/"; //web ulr have to be provided FOR UBDATING DATABASE
//https://iot-based-waste-monitering-system2.onrender.com/updatedb/
 //function declaretion
void ConnectTOWifi(void);

//pins for declaration
const int trigPin = 5;  //D1
const int echoPin = 4;//D2
//DTH 11 //D3
const int mq135 = A0; // pin A0

// defines variables
int temp=0;
int hum=0;
int stepvalue = 0;
int percentage;
int percentage1;
long duration;
int distance;
int SMOKE_VALUE;
int td=200; // total length of dustbin

DHT dht2(0,DHT11);  //d
void setup() {
  
ConnectTOWifi();  //fuction to connect wifi
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
Serial.begin(9600); // Starts the serial communication jest to chek the values in serial port

}

void loop() { 
  //DTH11
 temp=dht2.readTemperature( );
 hum=dht2.readHumidity( );
  Serial.println("Temperature in C:");  
   Serial.println((dht2.readTemperature( )));  
   Serial.println("Humidity in C:");  
   Serial.println((dht2.readHumidity()));  
   delay(1000);  
   
//SMOKE DETECTION
SMOKE_VALUE = analogRead(mq135);
Serial.print("SMOKE_VALUE:");
Serial.println(SMOKE_VALUE);

//ultrasonic    
   // Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;
 
//percentage calculation (this is empty percentege)
percentage1=100*distance/td;
delay(1000); 
if(percentage1>100)
percentage1=100;
       
//dustbin fill percentage      
percentage=100-percentage1; 
Serial.println(distance);
Serial.println(percentage1);
Serial.println(percentage);

//clear the value of step 
stepvalue=0;
//fire check
if(SMOKE_VALUE>50)
stepvalue=1;
else{
stepvalue=0;}

  //function to write in web server
SendGETRequest();

}

void SendGETRequest(){
  //http.begin(client,url2); Specify request destination
  //http.GET();     calling the url by get request
client.setInsecure();
String myStr,tempsr,humsr;
String url1=url+"a"+"1"+"/";
String url2=url+"a"+"2"+"/";
String url3=url+"a"+"3"+"/";
String url0=url+"a"+"0"+"/";
if(percentage<10){ myStr = String(percentage);
http.begin(client,url2+"0"+myStr); 
http.GET();
}
else if (percentage>=100){
http.begin(client,url2+"99");
   http.GET();
}
else{  myStr = String(percentage);
http.begin(client,url2+myStr);
http.GET();
}
if(stepvalue==1){
http.begin(client,url1+"1");  
http.GET(); }
else{
http.begin(client,url1+"0");  
http.GET(); 
}
tempsr=String(temp);
humsr=String(hum);
http.begin(client,url3+tempsr);  
http.GET(); 
http.begin(client,url0+humsr);  
http.GET(); 
}
void ConnectTOWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_id,password);
  Serial.print("connecting to wifi");
  while(WiFi.status()!=WL_CONNECTED){
 delay(100);
  } 
}
