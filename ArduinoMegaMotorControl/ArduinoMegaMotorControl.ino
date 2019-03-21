/*
 DHCP Motor Control
 
 To use, telnet to your device's IP address and type.
 You can see the client's input in the serial monitor as well.
 Using an Arduino Wiznet Ethernet shield.

 This version attempts to get an IP address using DHCP


 Created using the DhcpChatServer Example from the Arduino IDE.

 */

#include <SPI.h>
#include <Ethernet.h>
#include <Stepper.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
};
int in1Pin = 8;
int in2Pin = 9;
int in3Pin = 10;
int in4Pin = 11;

int in5Pin = 2;
int in6Pin = 3;
int in7Pin = 4;
int in8Pin = 5;

Stepper motor1(512, in1Pin, in3Pin, in2Pin, in4Pin); 
Stepper motor2 (512, in5Pin, in7Pin, in6Pin, in8Pin); 
IPAddress ip(192, 168, 1, 177);
IPAddress myDns(192,168,1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);

// telnet defaults to port 23
EthernetServer server(23);
boolean gotAMessage = false; // whether or not you got a message from the client yet

void setup() {
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  
  pinMode(in5Pin, OUTPUT);
  pinMode(in6Pin, OUTPUT);
  pinMode(in7Pin, OUTPUT);
  pinMode(in8Pin, OUTPUT);

  // this line is for Leonardo's, it delays the serial interface
  // until the terminal window is opened
  //while (!Serial);
  
  Serial.begin(9600);
  Serial.write("Starting...");
  motor1.setSpeed(30);
  motor2.setSpeed(30);
  // Open serial communications and wait for port to open:
  // this check is only needed on the Leonardo:
  //while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  //}


  // start the Ethernet connection:
  Serial.println("Trying to get an IP address using DHCP");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // initialize the Ethernet device not using DHCP:
    Ethernet.begin(mac, ip, myDns, gateway, subnet);
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  ip = Ethernet.localIP();
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(ip[thisByte], DEC);
    Serial.print(".");
  }
  Serial.println();
  // start listening for clients
  server.begin();

}

void loop() {
  EthernetClient client;
  if (!client.available()){
    // wait for a new client:
    client = server.available();
  }
  String data = "";
  bool finished = false;
  // when the client sends the first byte, say hello:
  if (client) {
    if (!gotAMessage) {
      Serial.println("We have a new client");
      client.println("Hello, client!");
      gotAMessage = true;
    }
    
    // read the bytes incoming from the client:
    while (!finished && client.available()){
      char thisChar = client.read();
      //Serial.println(thisChar);
      data = data + thisChar;
      
    
      if (data.indexOf(";") >= 0) {
        finished = true;
        data = data.substring(0, data.indexOf(";"));
        Serial.print(data);
      }
    }
    
    if (data.indexOf("x") >= 0) { 
      String val = data.substring(1, data.indexOf(";"));
      // echo the bytes back to the client:
      //int time = 1000;
      //Serial.setTimeout(time);
      int steps = val.toInt(); //timing out??????
      motor2.step(steps);
      //Serial.println(steps);
      // echo the bytes to the server as well:
      //Serial.println(val);
      Ethernet.maintain();
      
    } else if (data.indexOf("y") >= 0) {
      String val = data.substring(1, data.indexOf(";"));
      // echo the bytes back to the client:
      //int time = 1000;
      //Serial.setTimeout(time);
      int steps = val.toInt(); //timing out??????
      motor1.step(steps);
      //Serial.println(steps);
      // echo the bytes to the server as well:
      //Serial.println(val);
      Ethernet.maintain();
      
    } else if (data.indexOf("s") >= 0) {
      String val = data.substring(1, data.indexOf(";"));
      // echo the bytes back to the client:
      //int time = 1000;
      //Serial.setTimeout(time);
      int speed = val.toInt(); //timing out??????
      motor1.setSpeed(speed);
      motor2.setSpeed(speed);
      //Serial.println(steps);
      // echo the bytes to the server as well:
      //Serial.println(val);
      Ethernet.maintain();
    }else {
      //Serial.print("No Motor Chosen");
      server.write("No Motor Chosen");
    }
    data = "";
  }
}
