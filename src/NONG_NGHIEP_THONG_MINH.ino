#define BLYNK_TEMPLATE_ID "TMPLHVnojgTf"
#define BLYNK_DEVICE_NAME "Smart Agriculture 2"
#define BLYNK_AUTH_TOKEN "UoLuoburmY0aNNJweCPtG2uBDkQAKC3l"

//DHT Library
#include <DHT.h> // Gọi thư viện DHT22
#define DHTPIN 2          
#define DHTTYPE DHT22  
DHT dht(DHTPIN, DHTTYPE);

//BLynk Library
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

//LCD library
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

// setup Wifi
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Đỗ Đại Học";
char pass[] = "123@5689";

// khai bao chan LCD trong BLynk
WidgetLCD LCD(V0);
BlynkTimer timer;


void sendSensor()
{
  float sh = analogRead(34);
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Khai bao chart cua DHT va CAM BIEN DAT
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
  Blynk.virtualWrite(V7, sh);
}

String array1="CAY DA DUOC TUOI";
String array2="CAY CHUA DUOC TUOI";
#define PIN_BUTTON4 32   
#define VPIN_BUTTON4    V4
int relay = 27;
int state_relay = 0;


//Khai báo nhiệm vụ của button cho blynk


BLYNK_CONNECTED() {
  Blynk.syncVirtual(VPIN_BUTTON4);
}

BLYNK_WRITE(VPIN_BUTTON4) {
state_relay = param.asInt();
// Tùy vào lệnh nhận được ta thay đổi trạng thái bật tắt của ổ cắm
digitalWrite(relay, state_relay);
Serial.println("BLYNK_WRITE: Relay State = "+String(state_relay));
}

void setup()
{
  pinMode (35, INPUT);
  pinMode(34, INPUT);
  Serial.begin(115200);
  pinMode(PIN_BUTTON4,INPUT_PULLUP);
  pinMode(relay,OUTPUT);
  dht.begin(); // Khởi động LCD
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
 

  dht.begin();
  lcd.begin();
  lcd.backlight(); //open the backlight 
  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
  LCD.clear(); //Use it to clear the LCD Widget
  LCD.print(0, 0, "CAY CHUA DUOC TUOI"); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
  hienthichuoi(0,0,array2);

}

void loop()
{
  Serial.begin(115200);
  int buttonvalue = digitalRead(PIN_BUTTON4);
  Serial.println(buttonvalue);
  int soilvalue = analogRead(34);
  Serial.println(soilvalue);
  delay(1000);  
   float h = dht.readHumidity(); //Đọc độ ẩm
   float t = dht.readTemperature(); //Đọc nhiệt độ 
  Serial.println("Arduino.vn");
  Serial.print("Nhiet do: ");
  Serial.println(t); //Xuất nhiệt độ
  Serial.print("Do am: ");
  Serial.println(h); //Xuất độ ẩm 
  Serial.println(); //Xuống hàng
   Serial.println(soilvalue);//Xuất ra serial Monitor       
  delay(1000);
  Blynk.run();
  timer.run();
  button_pushing();
  LCD_display();
}

// co che hoat dong nut
void button_pushing(){
    //--------------------------------------------------------------------------
    if(digitalRead(PIN_BUTTON4) == LOW){
      delay(200);
      control_relay(1);
      Blynk.virtualWrite(VPIN_BUTTON4, state_relay); //update button state
    }
}

// dieu khien relay
void control_relay(int concak){
  //------------------------------------------------
  if(concak == 1){
    state_relay = !state_relay;
    digitalWrite(relay, state_relay);
    Serial.println("control_relay: Relay1 State = "+String(state_relay));
    delay(50);
  }
}


void hienthichuoi(int cot, int dong, String chuoi)
{
  lcd.setCursor(cot,dong);
  for(int j=0;j<chuoi.length();j++)
    {
      lcd.print(chuoi[j]);
    } 
}

// hien thi len LCD
void LCD_display(){
if(digitalRead(relay) == HIGH){
LCD.clear(); //Use it to clear the LCD Widget
hienthichuoi(0,0,array2);
LCD.print(0, 0, "CAY CHUA DUOC TUOI"); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")

}
else if(digitalRead(relay)== LOW){
LCD.clear(); //Use it to clear the LCD Widget
hienthichuoi(0,0,array1);
LCD.print(0, 0, "CAY DA DUOC TUOI"); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
}
}
