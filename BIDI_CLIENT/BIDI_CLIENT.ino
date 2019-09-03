/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "0002";
const byte address_server[6] = "0001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address_server);
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  radio.stopListening();
  const char text[] = "CREATE|B";
  //radio.write(&text, sizeof(text));
  if (!radio.write(&text, sizeof(text) )) {
    Serial.println("No acknowledgement of transmission - receiving radio device connected?");    
  }
   // Now listen for a response
  radio.startListening();
   // But we won't listen for long, 200 milliseconds is enough
  unsigned long started_waiting_at = millis();

  // Loop here until we get indication that some data is ready for us to read (or we time out)
  while ( ! radio.available() ) {

    // Oh dear, no response received within our timescale
    if (millis() - started_waiting_at > 200 ) {
      Serial.println("No response received - timeout!");
      return;
    }
  }

   // Now read the data that is waiting for us in the nRF24L01's buffer
  char text_recv[32] = "";
  radio.read( &text_recv, sizeof(text_recv) );

  // Show user what we sent and what we got back
  Serial.print("Sent: ");
  Serial.print(text);
  Serial.print(", received: ");
  Serial.println(text_recv);
  delay(5000);
}
