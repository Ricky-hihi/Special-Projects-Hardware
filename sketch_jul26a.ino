//電池宣告

int analogPin = A1 ;       // 感應電池容量
int MyArray[11] = {0} ;    // 排大小所用陣列
int val ;                  // 電池容量轉成百分比後
int blank ;                // 排大小所需暫存器

//感應器宣告

byte j ;                   // 記錄次數
byte k ;                   // 符合條件才加
byte led = digitalRead(7) ;
byte g ;

// 螢幕

#include <SPI.h>  
#include <Wire.h>  
#include <Adafruit_GFX.h>  
#include <Adafruit_SSD1306.h>  
#define OLED_RESET 4  
Adafruit_SSD1306 display(OLED_RESET);  
  
#define NUMFLAKES 10  
#define XPOS 0  
#define YPOS 1  
#define DELTAY 2  
  
  
#define LOGO16_GLCD_HEIGHT 16  
#define LOGO16_GLCD_WIDTH  16  
  
#define imageWidth 96  
#define imageHeight 64  
  
static const unsigned char PROGMEM logo16_glcd_bmp[] =  
{ B00000000, B11000000,  
  B00000001, B11000000,  
  B00000001, B11000000,  
  B00000011, B11100000,  
  B11110011, B11100000,  
  B11111110, B11111000,  
  B01111110, B11111111,  
  B00110011, B10011111,  
  B00011111, B11111100,  
  B00001101, B01110000,  
  B00011011, B10100000,  
  B00111111, B11100000,  
  B00111111, B11110000,  
  B01111100, B11110000,  
  B01110000, B01110000,  
  B00000000, B00110000 };  
  
  
#if (SSD1306_LCDHEIGHT != 64)  
#endif  


void setup() {

  //電池跟感應器都有
  Serial.begin(9600) ;

  
  //感應器
  pinMode(3, INPUT_PULLUP) ;  // 開關
  pinMode(5, INPUT) ;         // 感應器
  pinMode(7, OUTPUT) ;        // LED
  pinMode(8, OUTPUT) ;        // 蜂鳴器

  // 螢幕
  
  // put your setup code here, to run once:  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)  
  // init done  
  display.setTextColor(WHITE);  
  // Show image buffer on the display hardware.  
  // Since the buffer is intialized with an Adafruit splashscreen  
  // internally, this will display the splashscreen.  
  display.display();  
  delay(2000);  
  
  // Clear the buffer.  
  display.clearDisplay();  
  
}

void loop() {
  
  //電池

  // 把電量轉成百分比
  val = map(analogRead(analogPin),720,850,0,100) ;
  if (val < 0 or val > 100){
    if (val < 0){
      val = 0 ;
      }
    if (val > 100){
      val = 100 ;
      }
    }
  
  // 填入陣列 MyArray 中
  for (byte i = 0 ; i<11 ; i++){
    MyArray[i] = {val} ;
    }
    
  // 陣列內排大小
  for (byte m = 0 ; m<11 ; m++ ){
    for (byte n = m+1 ; n<11 ; n++){
      if (MyArray[m] < MyArray[n]){
        blank = MyArray[m] ;
        MyArray[m] = MyArray[n] ;
        MyArray[n] = blank ;
        }
      }
    }

  // 感應器
  
  // 顯示感應器是否有檢測到液面
  Serial.print("Liquid Level: ") ;Serial.println(digitalRead(5)) ;
  
  // 判斷功能為尿袋或點滴
  switch (digitalRead(3)){
    
    // 尿袋功能
    case HIGH :
    if (j<30) {
      j++ ;
      // 有感應到 k 才加
      if (digitalRead(5) == HIGH){
        k++ ;
        }
        delay(200) ;
      // 感應完30次
      }else{
        if(k>15){
          g = 1 ;
          digitalWrite(7,HIGH) ;
          digitalWrite(8,HIGH) ;
          }else{
            g = 0 ;
            digitalWrite(7,LOW) ;
            digitalWrite(8,LOW) ;
            }
        Serial.print("模式(尿袋 1 、 點滴 2): ") ;Serial.println(digitalRead(3)) ;
        Serial.print("k: ") ;Serial.println(k) ;
        Serial.print("Induction: ") ;Serial.println(digitalRead(7)) ;
        j = 0 ; k = 0 ;
        Serial.print("battery capacity : ") ;Serial.print(MyArray[5]) ;Serial.println("%") ;
        }
    break ; 

    // 點滴功能
    case LOW :
    if (j<30) {
      j++ ;
      // 沒感應到 k 才加
      if (digitalRead(5) == LOW){
        k++ ;
        }
        delay(200) ;
      // 感應完30次
      }else{
        if(k>15){
          g = 1 ;
          digitalWrite(7,HIGH) ;
          digitalWrite(8,HIGH) ;
          }else{
            g = 0 ;
            digitalWrite(7,LOW) ;
            digitalWrite(8,LOW) ;
            }
        Serial.print("模式: ") ;Serial.println(digitalRead(3)) ;
        Serial.print("k: ") ;Serial.println(k) ;
        Serial.print("Induction: ") ;Serial.println(digitalRead(7)) ;
        j = 0 ; k = 0 ;
        Serial.print("battery capacity : ") ;Serial.print(MyArray[5]) ;Serial.println("%") ;
        }
    break ; 
    }
    // 螢幕   

  // 顯示達標值
  display.setTextSize(1);               
  display.setCursor(0,0);               //設置文字位置  
  display.print("Standard:16");          

  // 顯示感應次數
  display.setTextSize(1);               
  display.setCursor(0,15);               //設置文字位置  
  display.print("Times: ");     
    
  display.setTextSize(1);               
  display.setTextColor(WHITE);          
  display.setCursor(40,15);   
  display.println(k);     

  // 顯示是否達標
  display.setTextSize(1);               
  display.setCursor(70,0);               //設置文字位置  
  display.print("Status: ");     
    
  display.setTextSize(1);               
  display.setTextColor(WHITE);          
  display.setCursor(115,0);   
  display.println(g);     

  // 顯示電量
  display.setTextSize(1);               
  display.setCursor(55,15);               //設置文字位置  
  display.print("Battery: ");     
    
  display.setTextSize(1);               
  display.setTextColor(WHITE);          
  display.setCursor(105,15);   
  display.println(MyArray[5]);     
  display.display(); 
    
  delay(100);  
  display.clearDisplay();  
  
}
