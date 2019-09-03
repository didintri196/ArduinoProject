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


//WIRELESS
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "0001";
const byte address_client[6] = "0002";

//ETHERNET
#define STATUS_CONNECTED 1
#define STATUS_DISCONNECTED 0
char namaServer[] = "10.42.0.1";
byte IP_eth[] = {10,42,0,2};
byte MAC_eth[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
int counter = 0;
boolean startRead = false; 
char inString[32];
char charFromWeb[9];
bool stgetcommand=true;
EthernetClient myEthernet;



void setup() {
  Serial.begin(9600);
  Serial.println("Starting Engine...");
  // ETHERNET
  Serial.println("--------------------------------------------------"); 
  Serial.println("Setting Perangkat");
  Serial.println("Mohon menunggu . . . ");
  Serial.println("Setting Ethernet MAC Address dan IP Address");
  Serial.println("Mohon menunggu . . . ");
  if (Ethernet.begin(MAC_eth) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(MAC_eth,IP_eth);
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
  
}

void loop() {
  func_wireless();
  
  if(stgetcommand==true){
    getcommand();
    }
}

//FUNGSI WIRELESS
void func_wireless(){
      if (radio.available()) {
      char text_recv[32] = "";
      String text_send = "PRINT|B-001|12-02-2019";
      while (radio.available()) {
           radio.read(&text_recv, sizeof(text_recv));
      }
      //Serial.println(text_recv);
      radio.stopListening();
      //POST KE SERVER WEB
//      String data_recv;
//      data_recv=text_recv;
      //delay(2000);
      //Serial.println(resp+"SDSDSD");
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
  uri_segment = "/PUSKESMAS_FIX/DDSC/postcommand?cmd=" + a; 
  myEthernet.print("GET ");
  myEthernet.print(uri_segment); 
  //delay(1000);
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
