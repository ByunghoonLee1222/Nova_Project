#include <stdio.h>
#include "PIDControl.h"

void PIDInit_V(PIDProfile *PID_st, double kp,double ki,double kd){

	PID_st->KP = kp;
	PID_st->KI = ki;
	PID_st->KD = kd;
	PID_st->dt = 0;
	PID_st->Derivative = 0;
	PID_st->Integral = 0;
	PID_st->Output = 0;
	PID_st->Pre_error = 0;
	PID_st->error = 0;
	PID_st->Value_act = 0;
	PID_st->Value_des = 0;
}

unsigned char PIDCtrl_UB(PIDProfile *PID_st, double dt){
	unsigned char Continue=0;

	PID_st->error = PID_st->Value_des - PID_st->Value_act;
	PID_st->Integral = PID_st->Integral + (PID_st->error * dt);
	PID_st->Derivative = (PID_st->error - PID_st->Pre_error) / dt;
	PID_st->Output = (PID_st->KP * PID_st->error) + (PID_st->KI * PID_st->Integral) + (PID_st->KD * PID_st->Derivative);
	PID_st->Pre_error = PID_st->error;
		
	if(PID_st->error == 0)
		Continue = 1;

	return Continue;
}
