#ifndef _MPU6050_H_
#define _MPU6050_H_

#define MPU_DEVICE_ADDR 0x68
#define LIMIT(a,b) (a * (fabs(a)>=b))
class Mpu6050{
public:
	double compensate[7] = {};
	double data[7] = {};
	double last_millis;
	double angle;
	Mpu6050(){
		init();
		setCompensate(1000);
		this->last_millis = 0;
		this->angle = 0;
	}
	Mpu6050(int count){
		init();
		setCompensate(count);
		this->last_millis = 0;
		this->angle = 0;
	}
	void init();
	void update();
	void readRawMpu6050();
	void setCompensate(int count);
	void findAngle();
};

#endif