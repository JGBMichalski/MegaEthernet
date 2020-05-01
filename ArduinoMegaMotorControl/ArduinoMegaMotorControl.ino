/*
 DHCP Motor Control
 
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
boolean gotAMessage = false; 

void setup() {
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  
  pinMode(in5Pin, OUTPUT);
  pinMode(in6Pin, OUTPUT);
  pinMode(in7Pin, OUTPUT);
  pinMode(in8Pin, OUTPUT);
  
  Serial.begin(9600);
  Serial.write("Starting...");
  motor1.setSpeed(15);
  motor2.setSpeed(15);

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

    Ethernet.maintain();
    
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
      
      Ethernet.maintain();
    }
    
    if (data.indexOf("x") >= 0) { 
      String val = data.substring(1, data.indexOf(";"));
      int steps = val.toInt(); //timing out??????
      motor2.step(steps);
      Ethernet.maintain();
      
    } else if (data.indexOf("y") >= 0) {
      String val = data.substring(1, data.indexOf(";"));
      int steps = val.toInt(); //timing out??????
      motor1.step(steps);
      Ethernet.maintain();
      
    } else if (data.indexOf("s") >= 0) {
      String val = data.substring(1, data.indexOf(";"));
      int speed = val.toInt(); //timing out??????
      motor1.setSpeed(speed);
      motor2.setSpeed(speed);
      Ethernet.maintain();
    }else {
      server.write("No Motor Chosen");
    }
    data = "";
    Ethernet.maintain();
  }
}
