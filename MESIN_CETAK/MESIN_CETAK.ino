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
#include "thermalprinter.h"

int buttonPinA = 2;//A
int buttonPinB = 3;//B
bool stprint = true;

//WIRELESS
#define RX 8
#define TX 9
String AP = "WIFI_LOKET";
String PASS = "11001100";
String PORT = "80";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
char inString[32];
boolean startRead = false; 
String HOST = "10.10.10.10";


//SETTING PRINTER
#define rxPin 5
#define txPin 6

int printStatus = 0;

bool cek = false;
//delay(3000);
Epson TM88 = Epson(rxPin, txPin); // init the Printer with Output-Pin
SoftwareSerial ESP8266(RX,TX); 

void setup() {
  //delay(3000);
  TM88.start();
  Serial.begin(9600);
  ESP8266.begin(9600);
  Serial.println("Starting engine");
  pinMode(buttonPinB, INPUT_PULLUP);
  pinMode(buttonPinA, INPUT_PULLUP);
  sendCommand("AT+CWMODE=1",5,"OK");
  String resp = sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
  if(resp == "OK"){
    cek = true;
      printtikettext("SUKSES KONEK WIFI");
    }else{
      printtikettext("GAGAL KONEK WIFI");
    }
  //SETTING PRINTER
  //TM88.start();
//  TM88.feed(5);
//  TM88.cut();
}

void loop() {
  if(cek == false){
    Serial.println("Mencoba konek wifi");
    String resp = sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
    if(resp == "OK"){
      cek = true;
      printtikettext("SUKSES KONEK WIFI");
      }
    }
  //A PRESSED
  int buttonValueA = digitalRead(buttonPinA);
   if (buttonValueA == LOW){
      // If button pushed, turn LED on
      //if(stprint==true){
      //Serial.println(stprint);
      //stprint=false;
      buatkarcis("A");
      
      //}
      
   } else {
      // Otherwise, turn the LED off
      //Serial.println("OFF");
      //stprint=true;
   }

   //B PRESSED
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

void printtiket(String nomor,String tanggal){
    
    TM88.justifyCenter();
    TM88.doubleHeightOn();
    TM88.println("UPTD PUSKESMAS KANDANGAN"); 
    TM88.doubleHeightOff();
    TM88.println("Jl. Malang No.109, Kandangan, Kec. Kandangan, Kediri, Jawa Timur 64294"); 
    TM88.feed(1);
    TM88.setSize(60);
    TM88.println(nomor);    
    TM88.reset();  
    TM88.println(tanggal);
    TM88.feed(1);  
    
    TM88.justifyRight();
    TM88.println("*Tiket dibawa ke loket");  
    TM88.feed(1);
    TM88.cut(); 
//    TM88.end(); 
  }

void printtikettext(String message){
    //TM88.start();
    TM88.feed(4);
    TM88.justifyCenter();
    TM88.setSize(60);
    TM88.println(message);  
    TM88.feed(5);
    TM88.cut();  
  }

void printResponse() {
  while (ESP8266.available()) {
    Serial.println(ESP8266.readStringUntil('\n')); 
  }
}


 
String bacaWebText(){
  unsigned int time;
  Serial.println("Baca respon dari server . . . "); 
  Serial.println("Mohon menunggu . . . ");
  //time = millis();
  //Serial.print("Timer Millis () : ");
  //Serial.println(time);
  int stringPos = 0;
  memset( &inString, 0, 32 );
  int unvailable_ctr = 0;
  while(true){
    if (ESP8266.available()) {
      char c = ESP8266.read();
      Serial.print(c);
      if (c == '#' ) { 
        Serial.print("Menemukan start key # dengan isi : ");
        startRead = true;  
      }
      else if(startRead){
        if(c != '^'){ 
          inString[stringPos] = c;
          stringPos ++;
        }else{
          startRead = false;
          Serial.println();
          Serial.println("Baca respon dari server selesai!");
          Serial.println("Sambungan diputuskan . . . ");
          return inString;
        }
      }
    }
    else{
      //Serial.println("ethernet unavailable");
      delay(50);
      unvailable_ctr++;
      if(unvailable_ctr == 25){
        Serial.println("Koneksi mengalami time out");
        Serial.println("Sambungan diputuskan . . . ");
        Serial.println("Reset...");
        return inString;
      }
    }
  }
  //delay(1000);
}


void buatkarcis(String tipe) {
  sendCommand("AT+CIPMUX=1",5,"OK");
    //ESP8266.println("AT+CIPMUX=1");
    //delay(500);
    //printResponse();

    ESP8266.println("AT+CIPSTART=4,\"TCP\",\""+HOST+"\",80");
    delay(500);
    //printResponse();
    String uri_segment = "/html/DDSC/postcommand?cmd=CREATE|" + tipe; 
    String cmd = "GET "+uri_segment+" HTTP/1.1\r\nHost: "+HOST+"\r\nConnection: close\r\n\r\n";
    ESP8266.println("AT+CIPSEND=4," + String(cmd.length() + 4));
    delay(100);

    ESP8266.println(cmd);
    ESP8266.println(""); 
 // }

//  if (ESP8266.available()) {
//    Serial.write(ESP8266.read());
//  }
  String recv_string = bacaWebText();
//  Serial.println(hasil);
//  String recv_string="";
  if(recv_string!=""){
      
      Serial.println("CETAK KARCIS");
    
      //PARSING TIKET
      String nomor=getValue(recv_string,'|',1);
      String tanggal=getValue(recv_string,'|',2);
      
      printtiket(nomor,tanggal);
     // delay(3000);
    }

}

String sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    ESP8266.println(command);//at+cipsend
    if(ESP8266.find(readReplay))//ok
    {
      found = true;
      break;
    }

    countTimeCommand++;
  }
  String Message="";
  if(found == true)
  {
    Serial.println("OYI");
    Message="OK";
    countTrueCommand++;
    countTimeCommand = 0;
  }

  if(found == false)
  {
    Serial.println("Fail");
   Message="ERR";
    countTrueCommand = 0;
    countTimeCommand = 0;
  }

  found = false;
  return Message;
 }
