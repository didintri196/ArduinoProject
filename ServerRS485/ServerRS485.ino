//Master code (Arduino UNO)
//Serial Communication Between Arduino using RS-485

int enablePin = 2;  
int pushval = A0;
int potval =0 ;

void setup()
{
  Serial.begin(9600);            // initialize serial at baudrate 9600:
  pinMode(enablePin, OUTPUT);
  pinMode(pushval,INPUT); 
  delay(10); 
  digitalWrite(enablePin, HIGH);  //  (always high as Master Writes data to Slave)
}
void loop()
{
    int potval = analogRead(pushval);
    Serial.println("asd");          //Serial Write POTval to RS-485 Bus
    delay(100);
}
