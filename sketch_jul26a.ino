  //電池宣告

int analogPin = A1 ;       // 感應電池容量
int MyArray[11] = {0} ;    // 排大小所用陣列
int val ;                  // 電池容量轉成百分比後
int blank ;                // 排大小所需暫存器

//感應器宣告

byte j ;                   // 記錄次數
byte k ;                   // 符合條件才加
byte led = digitalRead(7) ;

void setup() {

  //電池跟感應器都有
  Serial.begin(9600) ;


  //感應器
  pinMode(3, INPUT_PULLUP) ;  // 開關
  pinMode(5, INPUT) ;         // 感應器
  pinMode(7, OUTPUT) ;        // LED
  pinMode(8, OUTPUT) ;        // 蜂鳴器

}

void loop() {
  
  //電池

  // 把電量轉成百分比
  val = map(analogRead(analogPin),720,850,0,100) ;
  if (val > 100 or val < 0 ){
    if (val > 100){
      val = 100 ;
      }
    if (val < 0){
      val = 0 ;
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
          digitalWrite(7,HIGH) ;
          digitalWrite(8,HIGH) ;
          }else{
            digitalWrite(7,LOW) ;
            digitalWrite(8,LOW) ;
            }
        Serial.print("模式(尿袋 1 、 點滴 0): ") ;Serial.println(digitalRead(3)) ;
        Serial.print("k: ") ;Serial.println(k) ;
        Serial.print("Induction: ") ;Serial.println(digitalRead(7)) ;
        Serial.print("battery capacity : ") ;Serial.print(MyArray[5]) ;Serial.println("%") ;
        j = 0 ; k = 0 ;

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
          digitalWrite(7,HIGH) ;
          digitalWrite(8,HIGH) ;
          }else{
            digitalWrite(7,LOW) ;
            digitalWrite(8,LOW) ;
            }
        Serial.print("模式(尿袋 1 、 點滴 0): ") ;Serial.println(digitalRead(3)) ;
        Serial.print("k: ") ;Serial.println(k) ;
        Serial.print("Induction: ") ;Serial.println(digitalRead(7)) ;
        Serial.print("battery capacity : ") ;Serial.print(MyArray[5]) ;Serial.println("%") ;
        j = 0 ; k = 0 ;  
        }
    break ; 
    }
}
