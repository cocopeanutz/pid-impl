#include "qPIDs.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

float setPoint = 0.0;
float sensor = 0.0;
float output = 0.0;


// Example usage for the qPID library for controlling the speed of a simle motor
// The idea is to control the speed of the motor by varing the voltage applied via PWM


struct EMBEDDED_DEVICE{
	float current;
	float target;
	float set;
};
float readSensor(EMBEDDED_DEVICE * s){
	return s->current;
};			// A function that reads the motor speed from a sensor
float readSetPoint(EMBEDDED_DEVICE * s){
	return s->target;
};		// A function that reads the desired speed from a potentiometer
void setActuator(EMBEDDED_DEVICE * s){
	s->current += s->set;
};				// A function that sets the PWM output for controlling the motor

void example(void){

	// Declare de new object
	qPID controller;
	qPID_Init(&controller);
	// Configure settings
	controller.AntiWindup = ENABLED;
	controller.Bumpless = ENABLED;

	// Set mode to auotmatic (otherwise it will be in manual mode)
	controller.Mode = AUTOMATIC;
	// controller.Mode = RELAY;
	// Configure de output limits for clamping
	controller.OutputMax = 150.0;
	controller.OutputMin = -150.0;

	// Set the rate at the PID will run in seconds
	controller.Ts = 0.005;

	// More settings
	controller.b = 1.0;
	controller.c = 1.0;

	// Init de controller

	controller.Mode = AUTOMATIC;
	// Set the tunning constants
	controller.K = 0.5;// P
	controller.Ti = 1/0.02;// I
	controller.Td = 0.0;// D
	controller.Nd = 4.0;// Nd
	EMBEDDED_DEVICE s;
	s.current = 100;
	s.target = 500;
	s.set = 0;
	while (1){
		sensor = readSensor(&s);				// update the process variable
		setPoint = readSetPoint(&s); 			// update the user desired value

		// Update the PID and get the new output
		output = qPID_Process(&controller, setPoint, sensor);
		s.set = output;
		printf("output: %f ,", s.set);
		sleep(1);
		setActuator(&s);				// update the actuator input
		printf("current:%f , target:%f , set:%f \n", s.current, s.target, s.set);
		float diff = s.target > s.current ? s.target -s.current : s.current - s.target;
		if(diff<0.1) break;
	}
}

int main(){
	example();
}
