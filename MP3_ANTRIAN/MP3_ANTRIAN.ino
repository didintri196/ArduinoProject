void setup() {

  uint32_t beratTimbangan = 0;
   
//SUARA NOMOR ANTRIAN

}
 
void loop() {
 
}
 
void suaraBilangan(uint32_t Bilangan)
{
  if(Bilangan < 100)
  {
    suaraPuluhan(Bilangan);
  }
  else if(Bilangan < 1000)
  {
    suaraRatusan(Bilangan);
  }
  else
  {
    suaraRibuan(Bilangan);
  }
}
void suaraPuluhan(uint8_t Bilangan)
{
  if(Bilangan < 12)
  {
//    mp3_play(Bilangan);
  }
  else if(Bilangan < 20)
  {
//    mp3_play(Bilangan - 10);
//    mp3_play(belas);
  Serial.print("BELAS");
  }
  else
  {
    uint8_t puluhan = Bilangan/10;
    mp3_play(puluhan);
    mp3_play(puluh);
 
    puluhan *= 10;
    if(Bilangan - puluhan != 0)
    {
//      mp3_play((Bilangan - puluhan));
    }
  }
}
void suaraRatusan(uint16_t Bilangan)
{
  uint8_t ratusan = (uint8_t)(Bilangan/100);
  if(ratusan == 1)
  {
//mp3_play(seratus);
  }
  else
  {
//    mp3_play(ratusan);
//    mp3_play(ratus);
  }
  if(Bilangan % 100)
  {
    suaraPuluhan(Bilangan - (ratusan*100));
  }
}
void suaraRibuan(uint32_t Bilangan)
{
  uint16_t ribuan= (uint16_t)(Bilangan/1000);
  if(ribuan == 1)
  {
//    mp3_play(seribu);
  }
  else if(ribuan < 100)
  {
    suaraPuluhan(ribuan);
//    mp3_play(ribu);   
  }
  else
  {
    suaraRatusan(ribuan);
//    mp3_play(ribu);   
  }
  if(Bilangan % 1000)
  {
    suaraRatusan(Bilangan - (ribuan*1000));
  }
}
