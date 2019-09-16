// Test program for the MD_YX5300 library
//
// Menu driven interface using the Serial Monitor to test individual functions.
//

#include <MD_YX5300.h>

// Connections for serial interface to the YX5300 module
const uint8_t ARDUINO_RX = 18;    // connect to TX of MP3 Player module
const uint8_t ARDUINO_TX = 19;    // connect to RX of MP3 Player module

// Define global variables
MD_YX5300 mp3(ARDUINO_RX, ARDUINO_TX);

void setup()
{
  mp3.begin();
  mp3.volume(30);
  suaraOKE("A",149,20);//ABJAT INDEX FILE MENUJU KE
}

void loop()
{

}

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
