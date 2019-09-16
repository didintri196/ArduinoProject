const int buzzer = 31; //buzzer to arduino pin 9


void setup(){
 
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
  beep("OK");
}

void loop(){
 
  
  
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
