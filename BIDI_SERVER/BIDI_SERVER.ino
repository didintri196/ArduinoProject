/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "0001";
const byte address_client[6] = "0002";

void setup() {
  Serial.begin(9600);
  Serial.println("Starting Engine...");
//  WIRELESS
  radio.begin();
  radio.openWritingPipe(address_client);
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  Serial.println("Module Wireless...ok");
}

void loop() {
func_wireless();
}

void func_wireless(){
    if (radio.available()) {
    char text_recv[32] = "";
    char text_send[32] = "OK";
    while (radio.available()) {
         radio.read(&text_recv, sizeof(text_recv));
    }
    //Serial.println(text_recv);
    radio.stopListening();
    radio.write(&text_send, sizeof(text_send));
    // Now, resume listening so we catch the next packets.
    radio.startListening();

    // Tell the user what we sent back (the random numer + 1)
    Serial.print("Sent response ");
    Serial.println(text_send);
  }
  }
