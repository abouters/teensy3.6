#include <Arduino.h>
#include <MsTimer2.h>
#include <interrupt.h>

void setupTimerInt(int ms){
    MsTimer2::set(ms, timerInt);
}
void startTimerInt(){
    MsTimer2::start();
}
void timerInt(){
	//ここにタイマー割り込みの処理
}
void serialEvent1(){
    if(Serial1.available() > 0){
        //ここに通信割り込みの処理(Serial1)
    }
}
void serialEvent2(){
    if(Serial2.available() > 0){
        //ここに通信割り込みの処理(Serial2)
    }
}

