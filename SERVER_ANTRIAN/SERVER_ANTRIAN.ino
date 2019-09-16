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
#include <Ethernet.h>
#include "RS485_protocol.h"
#include <SoftwareSerial.h>
#include <MD_YX5300.h>
SoftwareSerial rs485 (2, 3);
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// Connections for serial interface to the YX5300 module
const uint8_t ARDUINO_RX = 18;    // connect to TX of MP3 Player module
const uint8_t ARDUINO_TX = 19;    // connect to RX of MP3 Player module

// Define global variables
MD_YX5300 mp3(ARDUINO_RX, ARDUINO_TX);

const int buzzer = 31; //buzzer to arduino pin 31
const byte ENABLE_PIN = 4;

// callback routines
  
void fWrite (const byte what)
  {
  rs485.write (what);  
  }
  
int fAvailable ()
  {
  return rs485.available ();  
  }

int fRead ()
  {
  return rs485.read ();  
  }

  
//WIRELESS
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "0001";
const byte address_client[6] = "0002";

//ETHERNET
#define STATUS_CONNECTED 1
#define STATUS_DISCONNECTED 0
char namaServer[] = "10.10.10.10";
byte IP_eth[] = {10,42,0,2};
byte MAC_eth[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
int counter = 0;
boolean startRead = false; 
char inString[32];
char charFromWeb[9];
bool stgetcommand=true;
EthernetClient myEthernet;



void setup() {
  //lcd.init();                      // initialize the lcd 
  //lcd.init();
  //lcd.backlight();
  rs485.begin(28800);
  pinMode(ENABLE_PIN, OUTPUT);  // driver output enable
//  pinMode(LED_PIN, OUTPUT);
  pinMode(buzzer, OUTPUT); // Set buzzer
  Serial.begin(9600);
  Serial.println("Starting Engine...");
//  lcd.setCursor(0,0);
//  lcd.print("Starting Engine");
  // ETHERNET
  Serial.println("--------------------------------------------------"); 
  Serial.println("Setting Perangkat");
  Serial.println("Mohon menunggu . . . ");
  Serial.println("Setting Ethernet MAC Address dan IP Address");
  Serial.println("Mohon menunggu . . . ");
  if (Ethernet.begin(MAC_eth) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    beep("ERROR");
    //Ethernet.begin(MAC_eth,IP_eth);
    
  }
  // print your local IP address:
  Serial.print("My IP address: ");
    
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }

  
  
  // WIRELESS
  Serial.println();
  Serial.print("Setting Module Wireless");
  radio.begin();
  radio.openWritingPipe(address_client);
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  Serial.println("....OK");
  delay(1000);
  Serial.println("Setting Perangkat selesai!");
  Serial.println("--------------------------------------------------");
  mp3.begin();
  mp3.volume(30);
  beep("OK");
//  suaraOKE("A",1,19);//ABJAT INDEX FILE MENUJU KE


}

void loop() {
  func_wireless();
 // func_ethernet();
}
//FUNGSI ETHERNET
void func_ethernet(){
    
  if(stgetcommand==true){
    String leddata="";
    leddata=getcommand();
    //Serial.print(leddata+"SSS");
    String command=getValue(leddata,'|',0);
   
    if(command=="LED"){
      int addrs_led=getValue(leddata,'|',1).toInt();
      String tipeantrian=getValue(leddata,'|',2);
      int digit_1=getValue(leddata,'|',3).toInt();
      int digit_2=getValue(leddata,'|',4).toInt();
      int digit_3=getValue(leddata,'|',5).toInt();
      // assemble message
      char as='-';
      Serial.println(tipeantrian);
      if(tipeantrian=="A"){
        as='A';
      }else if(tipeantrian=="B"){
        as='B';
      }
//      Serial.println(noantrian.c_str());
//      Serial.println(testss);
        byte msg [] = { 
           addrs_led,    // device 1
           as,    // HURUF
           digit_1,
           digit_2,
           digit_3
        };
        // send to slave  
        digitalWrite (ENABLE_PIN, HIGH);  // enable sending
        sendMsg (fWrite, msg, sizeof msg);
        digitalWrite (ENABLE_PIN, LOW);  // disable sending
      }else if(command=="SUARA"){
      String Abjat=getValue(leddata,'|',1);
      int digit_1=getValue(leddata,'|',2).toInt();
      int digit_2=getValue(leddata,'|',3).toInt();
      // assemble message
      suaraOKE(Abjat,digit_1,digit_2);
      }
    }
}


//FUNGSI WIRELESS
void func_wireless(){
  Serial.println("ASA");
      if (radio.available()) {
      char text_recv[32] = "";
      String text_send = "PRINT|B-001|12-02-2019";
      while (radio.available()) {
           radio.read(&text_recv, sizeof(text_recv));
      }
      //Serial.println(text_recv);
      radio.stopListening();
      //POST KE SERVER WEB
      text_send=sendcommand(text_recv);
      radio.write(text_send.c_str(), text_send.length());
      // Now, resume listening so we catch the next packets.
      radio.startListening();
  
      // Tell the user what we sent back (the random numer + 1)
      Serial.print("received: ");
      Serial.print(text_recv);
      Serial.print(". Sent response ");
      Serial.println(text_send);
      
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

//ETHERNET

String sendcommand(String a){
  String response;
    int resultBukaKoneksi = bukaKoneksi();
    if(resultBukaKoneksi==1){
      response=kirimData(a);
      Serial.println();
    }
    return response;
  }

String getcommand(){
  String response;
    int resultBukaKoneksi = bukaKoneksi();
    if(resultBukaKoneksi==1){
      response=kirimData("GET");
      Serial.println();
    }
    return response;
  }

int bukaKoneksi(){
  Serial.print("Mencoba sambungan ke server http://"); 
  Serial.println(namaServer);  
  Serial.println("Mohon menunggu . . . ");
  if(myEthernet.connect(namaServer,80)){
    Serial.println("Sambungan ke server berhasil!");
    return STATUS_CONNECTED; 
  }
  else{
    Serial.print("Sambungan ke server gagal!");
    Serial.println();
    return STATUS_DISCONNECTED;
  }
 // delay(1000);
}

String kirimData(String a){
  Serial.println("Menjalankan perintah kirim data");
  String data = " Arduino";
  int ln = data.length();
  String uri_segment;
  uri_segment = "/html/DDSC/postcommand?cmd=" + a; 
  myEthernet.print("GET ");
  myEthernet.print(uri_segment); 
  //delay(1000);
  Serial.println(uri_segment);
  Serial.print("Data yang dikirim di ke server : ");
  Serial.println(a);
  myEthernet.println(" HTTP/1.0");
  myEthernet.print( "Host: " );
  myEthernet.println(" 10.42.0.1 \r\n");
  Serial.println("Host OK");
  myEthernet.println( "Content-Type: application/x-www-form-urlencoded \r\n" );
  Serial.println("Content type OK");
  myEthernet.print( "Content-Length: " );
  myEthernet.print(ln);
  myEthernet.print(" \r\n");
  myEthernet.println( "Connection: close" );
  myEthernet.println();
  String res;
  res = bacaWebText();
  if(!res.equals("")){
    Serial.println("PERINTAH DARI SERVER.");
    Serial.println(res);
  }
  return res;
}

String bacaWebText(){
  unsigned int time;
  Serial.println("Baca respon dari server . . . "); 
  Serial.println("Mohon menunggu . . . ");
  time = millis();
  Serial.print("Timer Millis () : ");
  Serial.println(time);
  int stringPos = 0;
  memset( &inString, 0, 32 );
  int unvailable_ctr = 0;
  while(true){
    if (myEthernet.available()) {
      char c = myEthernet.read();
      Serial.print(c);
      if (c == '#' ) { 
        Serial.print("Menemukan start key # dengan isi : ");
        startRead = true;  
      }
      else if(startRead){
        if(c != '^'){ 
          inString[stringPos] = c;
          stringPos ++;
        }
        else{
          startRead = false;
          Serial.println();
          Serial.println("Baca respon dari server selesai!");
          myEthernet.stop();
          myEthernet.flush();
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
        myEthernet.stop();
        myEthernet.flush();
        Serial.println("Koneksi mengalami time out");
        Serial.println("Sambungan diputuskan . . . ");
        Serial.println("Reset...");
        return inString;
      }
    }
  }
  delay(1000);
}

///END ETHERNET

//SUARA

void suaraOKE(String AbjatString,uint32_t Bilangan,uint32_t Menujuke)
{
  uint32_t Abjat=0;
  if(AbjatString=="A"){
    Abjat=18;
    }else if(AbjatString=="B"){
    Abjat=19;
    }
  mp3.playTrack(23);
  delay(2000);
  mp3.playTrack(16);
  delay(1000);
  mp3.playTrack(Abjat);
  delay(200);

  if(Bilangan < 100)
  {
     suaraPuluhan(Bilangan);
   
  }
  else if(Bilangan < 1000)
  {
     suaraRatusan(Bilangan);
  }
   mp3.playTrack(17);//MENUJU KE
   delay(900);
   mp3.playTrack(Menujuke);
   delay(1000);
  
}


void suaraPuluhan(uint8_t Bilangan)
{
  if(Bilangan < 12)
  {
    mp3.playTrack(Bilangan);
    delay(300);
  }
  else if(Bilangan < 20)
  {
    mp3.playTrack(Bilangan-10);
    delay(300);
    mp3.playTrack(12);//BELAS
    delay(300);
  //Serial.print("BELAS");
  }
  else
  {
    uint8_t puluhan = Bilangan/10;
    mp3.playTrack(puluhan);
    delay(300);
    mp3.playTrack(14);//PULUH
    delay(300);
    puluhan *= 10;
    if(Bilangan - puluhan != 0)
    {
      mp3.playTrack((Bilangan - puluhan));
      delay(300);
    }
  }
}

void suaraRatusan(uint16_t Bilangan)
{
  uint8_t ratusan = (uint8_t)(Bilangan/100);
  if(ratusan == 1)
  {
    mp3.playTrack(13);//SERATUS
    delay(300);
  }
  else
  {
    mp3.playTrack(ratusan);
    delay(300);
    mp3.playTrack(15);//RATUS
    delay(300);
  }
  if(Bilangan % 100)
  {
    suaraPuluhan(Bilangan - (ratusan*100));
  }
}

void beep(String status){
  if(status=="OK"){
    tone(buzzer, 4000); // Send 1KHz sound signal...
    delay(100);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    tone(buzzer, 4000); // Send 1KHz sound signal...
    delay(200);        // ...for 1 sec
    noTone(buzzer);        // ...for 1sec
    }else if(status=="ERROR"){
    tone(buzzer, 4000); // Send 1KHz sound signal...
    }
  
  }
