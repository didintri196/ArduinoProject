#include <Arduino.h>
#include <Wire.h>
#include <sim800.h>
#include <WiFi.h>
#include <gprs.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#define T_CALL

#if defined(T_CALL)
#define UART_TX                     27
#define UART_RX                     26
#define SIMCARD_RST                 5
#define SIMCARD_PWKEY               4
#define SIM800_POWER_ON             23
#else
#define UART_TX                     33
#define UART_RX                     34
#define SIMCARD_RST                 14
#define SIMCARD_PWKEY               15
#define SIM800_POWER_ON             4
#endif

#define UART_BANUD_RATE             9600

#define I2C_SDA                     21
#define I2C_SCL                     22

#define IP5306_ADDR                 0X75
#define IP5306_REG_SYS_CTL0         0x00
String RETURN="";

HardwareSerial hwSerial(1);
GPRS gprs(hwSerial, SIMCARD_PWKEY, SIMCARD_RST, SIM800_POWER_ON);

bool setPowerBoostKeepOn(int en)
{
    Wire.beginTransmission(IP5306_ADDR);
    Wire.write(IP5306_REG_SYS_CTL0);
    if (en)
        Wire.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
    else
        Wire.write(0x35); // 0x37 is default reg value
    return Wire.endTransmission() == 0;
}

void wifi_connect()
{
    WiFi.disconnect();
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);
    WiFi.setHostname("INDOSAT-01");
    WiFi.begin("ss", "11111111");
    WiFi.mode(WIFI_STA);
    WiFi.waitForConnectResult();
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void setup()
{
    Serial.begin(115200);

#if defined(T_CALL)
    Wire.begin(I2C_SDA, I2C_SCL);
    bool   isOk = setPowerBoostKeepOn(1);
    String info = "IP5306 KeepOn " + String((isOk ? "PASS" : "FAIL"));
    Serial.println(info);
#endif

    hwSerial.begin(UART_BANUD_RATE, SERIAL_8N1, UART_RX, UART_TX);
    Serial.println("Setup Complete!");

    wifi_connect();

    if (gprs.preInit()) {
        Serial.println("SIM800 Begin PASS");
    } else {
        Serial.println("SIM800 Begin FAIL");
    }

    if (0 != gprs.init()) {
        Serial.println("Not checked out to SIM card...");
        while (1);
    }
    Serial.println("Init success");
    hwSerial.print("AT+CMGF=1;\r\n");
}
int i =0;
bool isCallIn = false;
bool cekperintah=true;
void loop()
{
  if(WiFi.waitForConnectResult() != WL_CONNECTED) { 
    Serial.println("Connection Failed! Reconnect..."); 
    WiFi.reconnect();
    delay(5000);
    }
    hwSerial.print("AT+CMGL=\"ALL\";\r\n");
    delay(1000);
    String a="";
    while (hwSerial.available() > 0) {
      a = hwSerial.readString();
      Serial.println(a);
//      if(a.indexOf("+CMTI") > 0){
//          bacapesanindex(a);
//      }else if(a.indexOf("+CMGR") > 0){
//          kirimsmsindex(a);
//      }else 
      if(a.indexOf("+CMGL:") > 0){
          kirimsmsall(a);
      }else if(a.indexOf("+CUSD:") > 0){
          kirimlaporan(a);
      }
      
     }
    
    if (Serial.available()) {
        String r = Serial.readString();
        hwSerial.write(r.c_str());
    }
    if(cekperintah==true){
      getperintah();
      }
    
}

void getperintah(){
 HTTPClient http;
    http.begin("http://webhook.bitrans.co.id/GET_COMMAND.php");
//  http.begin("http://192.168.43.235/SERVER_PULSA/GETPERINTAH.php"); //Specify the URL
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.addHeader("Host", "192.168.43.235");
    int httpCode = http.GET();  
    if (httpCode > 0) { //Check for the returning code
        String payload = http.getString();
        //Serial.println(httpCode);
        if(payload!=""){
          Serial.println(payload);
          hwSerial.print(payload+"\r\n");
          cekperintah=false;
          }
          
      }
 
    else {
      Serial.println("Error on HTTP request");
      cekperintah=true;
    }
 
    http.end(); //Free the resources
  }

String kirimlaporan(String a){
 HTTPClient http;
    //Serial.println(a);
    http.begin("http://webhook.bitrans.co.id/POST_COMMAND.php");
//    http.begin("http://192.168.43.235/SERVER_PULSA/APIV1.php"); //Specify the URL
    http.addHeader("Content-Type", "text/plain"); //Specify content-type header
    int httpCode = http.POST(a);  
    if (httpCode > 0) { //Check for the returning code
        String payload = http.getString();
        //Serial.println(httpCode);
        if(payload!=""){
          Serial.println(payload);
          }
          cekperintah=true;
      }
 
    else {
      Serial.println("Error on HTTP request");
      cekperintah=true;
    }
 
    http.end(); //Free the resources
  }

String kirimsmsindex(String a){
 HTTPClient http;
    Serial.println("UPLOAD SMS MASUK");
    http.begin("http://webhook.bitrans.co.id/POST_COMMAND.php");
//    http.begin("http://192.168.43.235/SERVER_PULSA/APIV1.php"); //Specify the URL
    http.addHeader("Content-Type", "text/plain"); //Specify content-type header
    int httpCode = http.POST(a);  
    if (httpCode > 0) { //Check for the returning code
        String payload = http.getString();
        //Serial.println(httpCode);
        if(payload=="OK"){
          Serial.println(payload);
          Serial.println("DELETING SMS MASUK");
          hwSerial.print("AT+CMGD="+a+";\r\n");
          }
          cekperintah=true;
      }
 
    else {
      Serial.println("Error on HTTP request");
      cekperintah=true;
    }
 
    http.end(); //Free the resources
  }

  String kirimsmsall(String a){
  HTTPClient http;
    Serial.println("UPLOAD SMS MASUK ALL");
    http.begin("https://webhook.bitrans.co.id/POST_COMMAND.php");
//    http.begin("http://192.168.43.235/SERVER_PULSA/APIV1.php"); //Specify the URL
    http.addHeader("Content-Type", "text/plain"); //Specify content-type header
    int httpCode = http.POST(a);  
    if (httpCode > 0) { //Check for the returning code
        String payload = http.getString();
        //Serial.println(httpCode);
        if(payload=="OK"){
          Serial.println(payload);
          Serial.println("DELETING SMS MASUK ALL");
          hwSerial.print("AT+CMGD=1,4;\r\n");
          }
          cekperintah=true;
      }
 
    else {
      Serial.println("Error on HTTP request");
      cekperintah=true;
    }
 
    http.end(); //Free the resources
  }
 
void bacapesanindex(String a){
     String pecahindex = getValue(a,',',1);
     pecahindex.replace("\n", "");
     Serial.println(pecahindex);
     hwSerial.print("AT+CMGR="+pecahindex+";\r\n");
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
