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
#include "thermalprinter.h"

int buttonPinB = 3;//B
bool stprint = true;
//WIRELESS
RF24 radio(7, 8); // CE, CSN

const byte address[6] = "0002";
const byte address_server[6] = "0001";

//SETTING PRINTER
#define rxPin 5
#define txPin 6

int printStatus = 0;

Epson TM88 = Epson(rxPin, txPin); // init the Printer with Output-Pin


void setup() {
  Serial.begin(9600);
  pinMode(buttonPinB, INPUT_PULLUP);
  //SETTING WIRELLESS
  radio.begin();
  radio.openWritingPipe(address_server);
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  //SETTING PRINTER
  TM88.start();
  //TM88.feed(20);
  //TM88.cut();
}

void loop() {
  int buttonValueB = digitalRead(buttonPinB);
   if (buttonValueB == LOW){
      // If button pushed, turn LED on
      //if(stprint==true){
      //Serial.println(stprint);
      //stprint=false;
      buatkarcis("B");
      
      //}
      
   } else {
      // Otherwise, turn the LED off
      //Serial.println("OFF");
      //stprint=true;
   }
}

void buatkarcis(String tipe) {
  radio.stopListening();
  String query = "CREATE|"+tipe;
  //radio.write(&text, sizeof(text));
  if (!radio.write(query.c_str(), query.length())) {
    Serial.println("No acknowledgement of transmission - receiving radio device connected?");    
  }
   // Now listen for a response
  radio.startListening();
   // But we won't listen for long, 200 milliseconds is enough
  unsigned long started_waiting_at = millis();

  // Loop here until we get indication that some data is ready for us to read (or we time out)
  while ( ! radio.available() ) {

    
  }

   // Now read the data that is waiting for us in the nRF24L01's buffer
  char text_recv[32] = "";
  radio.read( &text_recv, sizeof(text_recv) );

  // Show user what we sent and what we got back
  Serial.print("Sent: ");
  Serial.print(query);
  Serial.print(", received: ");
  Serial.println(text_recv);
  String recv_string=text_recv;
  if(recv_string!=""){
      
      Serial.println("CETAK KARCIS");
    
      //PARSING TIKET
      String nomor=getValue(recv_string,'|',1);
      String tanggal=getValue(recv_string,'|',2);
      
      printtiket(nomor,tanggal);
      delay(3000);
    }

}


void printtiket(String nomor,String tanggal){
    TM88.start();
    TM88.justifyCenter();
    TM88.doubleHeightOn();
    TM88.println("UPTD PUSKESMAS KANDANGAN"); 
    TM88.doubleHeightOff();
    TM88.println("Jl. Malang No.109, Kacangan, Kandangan, Kec. Kandangan, Kediri, Jawa Timur 64294"); 
    TM88.feed(1);
    TM88.setSize(60);
    TM88.println(nomor);    
    TM88.reset();  
    TM88.println(tanggal);
    TM88.feed(1);  
    
    TM88.justifyRight();
    TM88.println("*Tiket dibawa ke kasir");  
    TM88.feed(1);
    TM88.cut();  
  }

//FUNGSI EXPLODE
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
