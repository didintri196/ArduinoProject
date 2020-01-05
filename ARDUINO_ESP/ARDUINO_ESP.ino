#include <SoftwareSerial.h>


#define RX 8
#define TX 9
String AP = "ss";
String PASS = "uuuuuuuu";
String PORT = "80";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
///
char inString[32];
boolean startRead = false; 
SoftwareSerial ESP8266(RX,TX); 

void setup() {
  Serial.begin(9600);
  ESP8266.begin(9600);
  //sendCommand("AT+CIOBAUD=9600",5,"OK");
  String resp = sendCommand("AT+CWMODE=1",5,"OK");
  Serial.println(resp);
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
}

unsigned long lastTimeMillis = 0;

void printResponse() {
  while (ESP8266.available()) {
    Serial.println(ESP8266.readStringUntil('\n')); 
  }
}

void loop() {

  //if (millis() - lastTimeMillis > 30000) {
    //lastTimeMillis = millis();

    ESP8266.println("AT+CIPMUX=1");
    delay(500);
    printResponse();

    ESP8266.println("AT+CIPSTART=4,\"TCP\",\"192.168.43.235\",80");
    delay(2000);
    printResponse();

    String cmd = "GET /coba.txt HTTP/1.1\r\nHost: 192.168.43.235\r\nConnection: close\r\n\r\n";
    ESP8266.println("AT+CIPSEND=4," + String(cmd.length() + 4));
    delay(100);

    ESP8266.println(cmd);
    ESP8266.println(""); 
 // }

//  if (ESP8266.available()) {
//    Serial.write(ESP8266.read());
//  }
String hasil = bacaWebText();
Serial.println(hasil);
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
  delay(1000);
}

///END ETHERNET


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
