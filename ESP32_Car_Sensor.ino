#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

#define IN1A 26
#define IN2A 25

#define IN1B 32
#define IN2B 33

#define TRIG 5
#define ECHO 17


void setup() {
  // put your setup code here, to run once:
  pinMode(IN1A, OUTPUT);
  pinMode(IN2A, OUTPUT);
  pinMode(IN1B, OUTPUT);
  pinMode(IN2B, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  
  Serial.begin(115200);
  Serial.println("Start!");
  SerialBT.begin("#ESP32_Car");
  SerialBT.println("Start!");
  randomSeed(analogRead(0));
}
char data;
bool autoMode = false;

double duration = 0;
double distance = 0;
double speed_of_sound = 331.5 + 0.6 * 25; // 25℃の気温の想定

int waiting = 0;
void loop() {
for(;;){
      ///////////////////////////////////////////↓autoMode
     if(autoMode==true)
      {
            digitalWrite(TRIG, LOW); 
            delayMicroseconds(2); 
            digitalWrite( TRIG, HIGH );
            delayMicroseconds( 10 ); 
            digitalWrite( TRIG, LOW );
            duration = pulseIn( ECHO, HIGH ); // 往復にかかった時間が返却される[マイクロ秒
            if (duration > 0) {
              duration = duration / 2; // 往路にかかった時間
              distance = duration * speed_of_sound * 100 / 1000000;
              Serial.print(distance);
              Serial.println("cm");
                 if(distance>100){
                    //run forward
                    digitalWrite(IN1A, HIGH);
                    digitalWrite(IN2A, LOW);
                    digitalWrite(IN1B, LOW);
                    digitalWrite(IN2B, HIGH);
                   }
                   else if(distance<40){
                     //back
                    digitalWrite(IN1A, LOW);
                    digitalWrite(IN2A, HIGH);
                    digitalWrite(IN1B, HIGH);
                    digitalWrite(IN2B, LOW);
                   }

                   else if(distance>50 && distance <60){
                   // waiting++;
                    digitalWrite(IN1A, LOW);
                    digitalWrite(IN2A, LOW);
                    digitalWrite(IN1B, LOW);
                    digitalWrite(IN2B, LOW);
                    /*if(waiting>100){
                      int randomNumber = random(2);
                      if(randomNumber==0){
                    digitalWrite(IN1A, HIGH);
                    digitalWrite(IN2A, LOW);
                    digitalWrite(IN1B, HIGH);
                    digitalWrite(IN2B, LOW);
                      }
                      if(randomNumber==1){
                    digitalWrite(IN1A, LOW);
                    digitalWrite(IN2A, HIGH);
                    digitalWrite(IN1B, LOW);
                    digitalWrite(IN2B, HIGH);
                      }
                      delay(200);
                    digitalWrite(IN1A, LOW);
                    digitalWrite(IN2A, LOW);
                    digitalWrite(IN1B, LOW);
                    digitalWrite(IN2B, LOW);
                    waiting=0;
                    }*/
                 }
             }
               if (SerialBT.available()) {
                    data = SerialBT.read();
                     if(data=='a'){autoMode=false;Serial.println("Changed MANUAL!");}
                     }
       }

    /////////////////////////////////////////////↓handleMode
   else if(autoMode==false){
      if (SerialBT.available()) {
         data = SerialBT.read();
         if (data == '0') {
          digitalWrite(IN1A, LOW);
          digitalWrite(IN2A, LOW);
          digitalWrite(IN1B, LOW);
          digitalWrite(IN2B, LOW);
         }
         else if (data == 's') {
          digitalWrite(IN1A, HIGH);
          digitalWrite(IN2A, LOW);
          digitalWrite(IN1B, LOW);
          digitalWrite(IN2B, HIGH);
         }
         else if (data == 'b') {
          digitalWrite(IN1A, LOW);
          digitalWrite(IN2A, HIGH);
          digitalWrite(IN1B, HIGH);
          digitalWrite(IN2B, LOW);
         }
         else if (data == 'r') {
          digitalWrite(IN1A, HIGH);
          digitalWrite(IN2A, LOW);
          digitalWrite(IN1B, HIGH);
          digitalWrite(IN2B, LOW);
         }
         else if (data == 'l') {
          digitalWrite(IN1A, LOW);
          digitalWrite(IN2A, HIGH);
          digitalWrite(IN1B, LOW);
          digitalWrite(IN2B, HIGH);
         }
         else if (data=='A'){
          autoMode=true;
          Serial.println("Changed AUTO!");
         }
      }
    }
    
    }
   }