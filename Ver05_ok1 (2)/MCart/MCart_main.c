/***************************************
	NOVA Co.,Ltd.
	Motorized Cart Controller
	TMS320F28335

	2015 / 06 / 08
***************************************/
#include "PIDControl.h"
#include "MCart_main.h"
#include "math.h"
#include "DataProcessing.h"

extern void Init_Param_V(void);
extern void Processing_StringCMD_V(void);
extern void Processing_MotorCtl_V(void);
extern void ulkeyInit_V(void);
extern void Processing_AllKalman_V(void);

PIDProfile WheelsPID_st;

void main(void)
{

	Init_Param_V();

	PIDInit_V(&WheelsPID_st,1,0,0);

	while(1)
	{

		Processing_StringCMD_V();

		//Processing_KalmanCtl_V();

		Processing_MotorCtl_V();

		ulkeyInit_V();

		Processing_AllKalman_V();


	}
}





