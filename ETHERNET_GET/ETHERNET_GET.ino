
#include <Ethernet.h>
#include <SPI.h>

#define DHT_SENSOR_PIN 2
#define STATUS_CONNECTED 1
#define STATUS_DISCONNECTED 0

char namaServer[] = "10.42.0.1";


byte IP_eth[] = {192,168,0,110};
byte MAC_eth[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };

int counter = 0;

boolean startRead = false; 

char inString[32];
char charFromWeb[9];

EthernetClient myEthernet;

int iterasi = 0;

void setup(){
  Serial.begin(9600);
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
  Serial.println();
  delay(1000);
  Serial.println("Setting Perangkat selesai!");
  Serial.println("--------------------------------------------------");
}

void loop() {
  sendcommand("CREATE|B");
  delay(1000); 
  Serial.println("--------------------------------------------------");
}

void sendcommand(String a){
    int resultBukaKoneksi = bukaKoneksi();
    if(resultBukaKoneksi==1){
      kirimData(a);
      Serial.println();
    }
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
  delay(1000);
}

void kirimData(String a){
  Serial.println("Menjalankan perintah kirim data");
  String data = " Arduino";
  int ln = data.length();
  String uri_segment;
  uri_segment = "/puskemas-kandangan/DDSC/postcommand?cmd=" + a; 
  myEthernet.print("GET ");
  myEthernet.print(uri_segment); 
  delay(1000);
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
    Serial.println("Data suhu dan kelembaban tersimpan.");
    Serial.print("Jumlah rows database ada : ");
    Serial.println(res);
  }
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
      //Serial.print(c);
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
