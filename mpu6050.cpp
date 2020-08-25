#include "mpu6050.h"
#include <Arduino.h>
#include <Wire.h>
#include <math.h>
void Mpu6050::init(){
	Wire.begin();
	Wire.beginTransmission(MPU_DEVICE_ADDR);
	Wire.write(0x6B);
	Wire.write(0x00);
	Wire.endTransmission();	

	//Gyro初期設定
	Wire.beginTransmission(MPU_DEVICE_ADDR);
	Wire.write(0x1B);
	Wire.write(0x00);
	Wire.endTransmission();	

	//加速度センサー初期設定
	Wire.beginTransmission(MPU_DEVICE_ADDR);
	Wire.write(0x1C);
	Wire.write(0x10);
	Wire.endTransmission();	

	//LPF設定
	Wire.beginTransmission(MPU_DEVICE_ADDR);
	Wire.write(0x1A);
	Wire.write(0x03);
	Wire.endTransmission();
}
void Mpu6050::readRawMpu6050(){
	Wire.beginTransmission(MPU_DEVICE_ADDR);
	Wire.write(0x3B);
	Wire.endTransmission();
	Wire.requestFrom(MPU_DEVICE_ADDR, 14);

	while (Wire.available() < 14);
	this->data[0] = (short)((Wire.read()<<8) | (Wire.read()));//ax
	this->data[1] = (short)((Wire.read()<<8) | (Wire.read()));//ay
	this->data[2] = (short)((Wire.read()<<8) | (Wire.read()));//az
	this->data[3] = (short)((Wire.read()<<8) | (Wire.read()));//temperature
	this->data[4] = (short)((Wire.read()<<8) | (Wire.read()));//gx
	this->data[5] = (short)((Wire.read()<<8) | (Wire.read()));//gy
	this->data[6] = (short)((Wire.read()<<8) | (Wire.read()));//gz
}

void Mpu6050::setCompensate(int count){
	for(int i=0; i<count; i++){
		readRawMpu6050();
		for(int j=0; j<7; j++){
			this->compensate[j] += data[j];
		}
	}
	for(int i=0; i<7; i++){
		this->compensate[i] /= count;
	}
}
void Mpu6050::update(){
	double dt = millis() - last_millis;
	readRawMpu6050();
	for(int i=0; i<7; i++){
		this->data[i] -= compensate[i];
	}
	this->data[0] = LIMIT(data[0],100);
	this->data[1] = LIMIT(data[1],100);
	this->data[2] = LIMIT(data[2],100);
	this->data[3] = LIMIT(data[3],10);
	this->data[4] = LIMIT(data[4],50);
	this->data[5] = LIMIT(data[5],50);
	this->data[6] = LIMIT(data[6],50);

	this->data[4] = dt*((data[4])*PI/180)/131;
	this->data[5] = dt*((data[5])*PI/180)/131;
	this->data[6] = dt*((data[6])*PI/180)/131;
	this->last_millis = millis();
	findAngle();
}
void Mpu6050::findAngle(){
	double cosx = cos(data[4]);
	double cosy = cos(data[5]);
	double cosz = cos(data[6]);
	double sign = data[6]/fabs(data[6]);
	double cos_theta = (cosx*cosy+cosy*cosz+cosz*cosx-1)/2;
	if(cos_theta<1) this->angle += acos(cos_theta)*sign;
}