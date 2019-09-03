#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
char server[] = "10.42.0.1";
void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // this check is only needed on the Leonardo:


  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();
}

void loop() {
getcommand();
}

void getcommand(){
   if (client.connect(server, 80)) {
    Serial.println("connected");
    client.println("GET /puskemas-kandangan/DDSC/getcommand HTTP/1.1\r\n");
    client.println("Host: 10.42.0.1\r\n");
    client.println("User-Agent: arduino-ethernet\r\n");
    //client.println("User-Agent: Mozilla/5.0"); 
    //This supposedly fixed 403 error for another user
    client.println("Connection: close\r\n\r\n");
    client.println();
   }else {
      Serial.println("connection failed");
   }

    if (client.connected()) { 
      char c = client.read();    
      Serial.print(c);
      client.stop();  // DISCONNECT FROM THE SERVER
    }
  
    delay(2000); 
   
  }
