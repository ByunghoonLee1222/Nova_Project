/***************************************
	NOVA Co.,Ltd.
	Motorized Cart Controller
	TMS320F28335

	2015 / 06 / 08
***************************************/
#include "MCart_main.h"
#include "math.h"
#include "DataProcessing.h"


extern char RXABUFFER[10], RXBBUFFER[100], RXCBUFFER[16], TXABUFFER[20], TXBBUFFER[8], FreeMotion[10],TXSENSOR[8];
extern int FreeMotionInterrupt,flagDebug;
extern unsigned int SpeedIncTimer, SpeedDecTimer;
extern unsigned int Timer_LED, Timer_Motion, Timer_Monitor, Timer_Run, Timer_zero;

void ForkLiftControl(unsigned int uiCon);


int  desiredAngle=0,presentAngle=0,presentError=0,lastError=0,FLAG_CCW_IVQuad=-1,FLAG_CW_IVQuad=-1,FLAG_CMD_CTRL=-1,FLAG_INIT=-1;
double speedVal=0;

unsigned char RcvComplete, ucKey;

float fUDLength, Coe[N];
unsigned int uiData, uiM1Speed, uiM2Speed,uiM1Acc, uiM2Acc, uiMotorCon, TEMP[2] = {0x34, 0x35}, uiGyroData[8], uiI2CError;

//**************************************
// Motorized Global Valuables
//**************************************
unsigned long ulKey;

unsigned int uiRoll, uiYaw, uiPitch, uiRollTemp, uiYawTemp, uiPitchTemp, GyroLength;
float fRoll, fYaw, fPitch;
char GyroRcv;
int iCartMotionFlag, LeftSpeed,RightSpeed;

// Kalman Filter

double kmf_q; //process noise covariance
double kmf_r; //measurement noise covariance
double kmf_x; //value
double kmf_p; //estimation error covariance
double kmf_k; //kalman gain
int flag=1;
int i = 0;


EALLOW;
void kalman_init(double q, double r, double p, double intial_value)
 {

	kmf_q = q;
	kmf_r = r;
	kmf_p = p;
    kmf_x = intial_value;


}
void kalman_update( double measurement)
{
                                  //prediction update
                                  //omit x = x
	 kmf_p = kmf_p + kmf_q;
     //measurement update
	 kmf_k = kmf_p / (kmf_p + kmf_r);
	 kmf_x = kmf_x + kmf_k * (measurement - kmf_x);
	 kmf_p = (1 - kmf_k) * kmf_p;
}


void Init_Param_V(void){


	INIT_SYSTEM();   //Initialize all SYSTEM
    //int i = 0;
	GpioDataRegs.GPADAT.bit.GPIO19 = 1;// DAC CS : High
	GpioDataRegs.GPADAT.bit.GPIO20 = 0; // WIFI FUNCTION1 : Low
	GpioDataRegs.GPADAT.bit.GPIO21 = 0; // WIFI FUNCTION2 : Low

	GpioDataRegs.GPBSET.bit.GPIO35 = 1;
	GpioDataRegs.GPBSET.bit.GPIO36 = 1;

	GpioDataRegs.GPADAT.bit.GPIO0 = 0;
	GpioDataRegs.GPADAT.bit.GPIO1 = 0;
	GpioDataRegs.GPADAT.bit.GPIO2 = 0;
	GpioDataRegs.GPADAT.bit.GPIO3 = 0;
	////////////////////////////////////
	GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 0;
	GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 0;
	GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 0;
	GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0;
	GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 0;
	GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 0;
	// GPIO 48~63 GPIO 기능으로 설정
	GpioCtrlRegs.GPBDIR.all = 0x00000000;  // GPI048~ 63 , 입력으로 설정
	GpioDataRegs.GPBDAT.all = 1;  //  GPIO 48~63 = high

	ForkLiftControl(0);

	delay_ms(1000);  // system stable time

	Timer_LED = 1;
	Timer_Motion = 0;
	Timer_Monitor = 1;
	Timer_Run = 1;

	delay_us(100);
	EPR_WRITE_STATUS_REGISTER(ONE_QUARTER);
	uiData = EPR_READ_STATUS_REGISTER();

	ENABLE_INT();

	delay_us(100);
	SPI_VOL_SEND_16(0xD002);	// REF 2.048V
//	SPI_CUR_SEND_EW(0x553005);

	delay_us(100);
//	SPI_CUR_SEND_NS(0x553005);

//	t1 = EPR_READ(400);
//	t2 = EPR_READ(401);
	fUDLength = (float)(EPR_READ(400) + (EPR_READ(401) << 8));

	TXBBUFFER[0] = 'N';
	TXBBUFFER[1] = '0';
	TXBBUFFER[2] = '0';
	TXBBUFFER[3] = '0';
	TXBBUFFER[4] = 'D';

	FIR_LPF_Coef(100, 5, Coe); // 40 2

	//-------------------------------------------------------------
	// Motorized CART INIT
	uiMotorCon = 0;
	uiM1Speed = 0;
	uiM2Speed = 0;
	uiM1Acc = 100;
	uiM2Acc = 100;

	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
/*
	for(i=0;i<10;i++){

		FreeMotion[i] = 0x00;
	}*/

	iCartMotionFlag = CART_READY;
}

void Processing_StringCMD_V(void){

	if(RcvComplete){
		if(RXABUFFER[0] == 'A'){
			uiM1Acc = ((RXABUFFER[1]-'0') * 100) + ((RXABUFFER[2]-'0') * 10) + (RXABUFFER[3]-'0');
			uiM2Acc = ((RXABUFFER[4]-'0') * 100) + ((RXABUFFER[5]-'0') * 10) + (RXABUFFER[6]-'0');
			RXABUFFER[0]='N';
			RXABUFFER[7]='N';
		}else if(RXABUFFER[0] == 'M' && RXABUFFER[7]=='E'){
			uiMotorCon = RXABUFFER[6]-'0';
			MotorControl(uiMotorCon);
			uiMotorCon=0;
			//RXABUFFER[0]='N';
			RXABUFFER[0]='N';
			RXABUFFER[7]='N';
		}else if(RXABUFFER[0] == 'S') {
			uiM1Speed = ((RXABUFFER[1]-'0') * 100) + ((RXABUFFER[2]-'0') * 10) + (RXABUFFER[3]-'0');
			uiM2Speed = ((RXABUFFER[4]-'0') * 100) + ((RXABUFFER[5]-'0') * 10) + (RXABUFFER[6]-'0');
			RXABUFFER[0]='N';
			RXABUFFER[7]='N';
		}else if(RXABUFFER[0] == 'G') {
			// Gyro Data Recall
			GYRO_Process();
			RXABUFFER[0]='N';
			RXABUFFER[7]='N';
		}

		RcvComplete = TRUE;
	}

}

void Processing_KalmanCtl_V(void){

	switch(iCartMotionFlag)
	{
		case CART_READY:
			break;
		case CART_ACC:
			//CART_SPEED_INC(SpeedIncTimer, uiM1Acc, uiM1Speed);
			//MOTOR_SPEED(uiM1Speed, uiM1Speed);
			break;
		case CART_DEC:
			MOTOR_SPEED(0, 0);
			GpioDataRegs.GPADAT.bit.GPIO0 = 0;
			GpioDataRegs.GPADAT.bit.GPIO1 = 0;
			GpioDataRegs.GPADAT.bit.GPIO2 = 0;
			GpioDataRegs.GPADAT.bit.GPIO3 = 0;
			iCartMotionFlag = CART_READY;
			break;
		case CART_CCW_READY:
			if(fYaw>=0 && fYaw <=90)
			{
				desiredAngle=anglePos(fYaw)-ANGLE_ROT;
				FLAG_CCW_IVQuad=1;
			}
			else
			{
				desiredAngle=fYaw-ANGLE_ROT;
			}
			iCartMotionFlag=CART_CCW_START;
			break;
		case CART_CCW_START:

			if(FLAG_CCW_IVQuad==1)
			{
				speedVal=0;
				speedVal=(double)PidSpeed_ctrl(anglePos(fYaw));
				speedVal=speedVal*1;
			}
			else
			{
				speedVal=0;
				speedVal=(double)PidSpeed_ctrl(fYaw);
				speedVal=speedVal*1;
			}
			if(speedVal==0.0)
			{
				MOTOR_SPEED(0,0);
				iCartMotionFlag=CART_STOP;
				FLAG_CCW_IVQuad=-1;
			}
			else
			{
				MOTOR_SPEED((int)speedVal,(int)speedVal);
			}

			break;
		case CART_CCW_END:
			break;
		case CART_CW_READY:
			if(fYaw>270 && fYaw <=360)
			{
				desiredAngle=anglePos(fYaw)+ANGLE_ROT;
				FLAG_CW_IVQuad=1;
			}
			else
			{
				desiredAngle=fYaw+ANGLE_ROT;
			}
				iCartMotionFlag=CART_CW_START;
			break;
		case CART_CW_START:
			if(FLAG_CW_IVQuad==1)
			{
				speedVal=0;
				speedVal=(double)PidSpeed_ctrl(anglePos(fYaw));
				speedVal=speedVal*1;
			}
			else
			{
				speedVal=0;
				speedVal=(double)PidSpeed_ctrl(fYaw);
				speedVal=speedVal*1;
			}
			if(speedVal==0.0)
			{
				MOTOR_SPEED(0,0);
				iCartMotionFlag=CART_STOP;
				FLAG_CW_IVQuad=-1;
			}
			else
			{
				MOTOR_SPEED((int)speedVal,(int)speedVal);
			}

			break;
		case CART_CW_END:
			break;
		case CART_STOP:
			MOTOR_SPEED(0, 0);
			GpioDataRegs.GPADAT.bit.GPIO0 = 0;
			GpioDataRegs.GPADAT.bit.GPIO1 = 0;
			GpioDataRegs.GPADAT.bit.GPIO2 = 0;
			GpioDataRegs.GPADAT.bit.GPIO3 = 0;
			iCartMotionFlag = CART_READY;
			break;
	}
}

void Processing_AllKalman_V(void){

	if(1){
		if(Timer_LED > 10){
			GpioDataRegs.GPBTOGGLE.bit.GPIO36 = 1;
			Timer_LED = 1;

			QRProcess();
			GYRO_Process();
		}

	/*	if(Timer_Monitor > 5000) {
			GpioDataRegs.GPADAT.bit.GPIO4 = 0;
			Timer_Monitor = 0;
		}*/
		if(FLAG_INIT<0)
		{
			kalman_init(0.00005,4,0.95,fYaw);
			FLAG_INIT=1;
		}
		else
		{
			kalman_update(fYaw);
		}

		Processing_KalmanCtl_V();
	}
}

int DeltaVel_sl=0,DesiredLifter=0,DesiredForkLift=0,l_debugvalue=0;

void Processing_MotorCtl_V(void)
{
	//Receive interrupt from USART --> start processing data
	if(FreeMotionInterrupt==1)
	{

		//int dayday=GpioDataRegs.GPBDAT.bit.GPIO57;
		unsigned int time_plus1, time_plus2, time_plus3;
		//If received datas are correct
		if(FreeMotion[0] == 0xB7 && FreeMotion[1] == 0xB8 && FreeMotion[9] == 0xFA)
		{

			int t_forkliftmove=0;

			//DesiredLifter = FreeMotion[2];//desired position of lifter
			//DesiredForkLift = FreeMotion[3];//desired poistion of fork-lift

			if(FreeMotion[4] < 5)
			{
				t_forkliftmove = (unsigned int)(FreeMotion[4]);//controlling fork-type lifter
				ForkLiftControl(t_forkliftmove);
			}

			else if(FreeMotion[4] == 5)// speed motion
			{

				LeftSpeed = (signed int) ((FreeMotion[6] << 8) | FreeMotion[5]);
				RightSpeed = (signed int) ((FreeMotion[8] << 8) | FreeMotion[7]);

				//forward processing
				if(LeftSpeed > 0 && RightSpeed > 0)
				{

					if(LeftSpeed==1)
						LeftSpeed = 0;
					if(RightSpeed==1)
						RightSpeed = 0;

					MotorControl(2);//Forward
				}

				//backward processing
				if(LeftSpeed < 0 && RightSpeed < 0)
				{

					if(LeftSpeed==-1)
						LeftSpeed = 0;

					if(RightSpeed==-1)
						RightSpeed = 0;

					LeftSpeed *= (-1);
					RightSpeed *= (-1);
					MotorControl(3);//Backward
				}

				//turn left processing
				if(LeftSpeed < 0 && RightSpeed > 0)
				{

					LeftSpeed *= (-1);
					MotorControl(1);//Turn left
				}

				//turn right processing
				if(LeftSpeed > 0 && RightSpeed < 0)
				{

					RightSpeed *= (-1);
					MotorControl(4);//Turn right
				}



				//brake urgent processing
				if(LeftSpeed == 0 && RightSpeed == 0)
				{

					LeftSpeed = 5;
					RightSpeed = 5;
					MotorControl(3);//Brake Urgent
				}

				uiM1Speed = (unsigned int)LeftSpeed;
				uiM2Speed = (unsigned int)RightSpeed;
			}

			else if(FreeMotion[4] == 6)// Acc motion
			{
				Timer_Monitor = 0;
				LeftSpeed = (signed int) ((FreeMotion[6] << 8) | FreeMotion[5]);
				RightSpeed = (signed int) ((FreeMotion[8] << 8) | FreeMotion[7]);

				Timer_zero = (((FreeMotion[3]<<8)| FreeMotion[2])*100);
				//forward processing
				if(LeftSpeed > 0 && RightSpeed > 0)
				{

					if(LeftSpeed==1)
					LeftSpeed = 0;
					if(RightSpeed==1)
					RightSpeed = 0;

					MotorControl(2);//Forward

					uiM1Speed = 0;
					uiM2Speed = 0;

					time_plus1 = (unsigned int)(Timer_zero * 0.25);
					time_plus2 = (unsigned int)(Timer_zero * 0.5);
					time_plus3 = (unsigned int)(Timer_zero * 0.75);

					do
					{
					if(Timer_Monitor < time_plus1 && Timer_Monitor > 0)
					{
						uiM1Speed = (unsigned int)(LeftSpeed*0.25);
						uiM2Speed = (unsigned int)(RightSpeed*0.25);

					}
					else if(Timer_Monitor < time_plus2 && Timer_Monitor > time_plus1)
					{
						FreeMotionInterrupt = 1;
						uiM1Speed = (unsigned int)(LeftSpeed*0.5);
						uiM2Speed = (unsigned int)(RightSpeed*0.5);
					}
					else if(Timer_Monitor < time_plus3 && Timer_Monitor > time_plus2 )
					{
						FreeMotionInterrupt = 1;
						uiM1Speed = (unsigned int)(LeftSpeed*0.75);
						uiM2Speed = (unsigned int)(RightSpeed*0.75);
					}
					else if(Timer_Monitor > time_plus3 )
					{
						FreeMotionInterrupt = 1;
						uiM1Speed = (unsigned int)LeftSpeed;
						uiM2Speed = (unsigned int)RightSpeed;
					}
					FreeMotionInterrupt = 0;
					} while(Timer_Monitor < Timer_zero);
				}
									//backward processing
				if(LeftSpeed < 0 && RightSpeed < 0)
				{

					if(LeftSpeed==-1)
					LeftSpeed = 0;
					if(RightSpeed==-1)
					RightSpeed = 0;
					LeftSpeed *= (-1);
					RightSpeed *= (-1);
					MotorControl(3);//Backward

				}

					//turn left processing
				if(LeftSpeed < 0 && RightSpeed > 0)
				{

					LeftSpeed *= (-1);
					MotorControl(1);//Turn left

				}

									//turn right processing
				if(LeftSpeed > 0 && RightSpeed < 0)
				{

					RightSpeed *= (-1);
					MotorControl(4);//Turn right

				}


				//brake urgent processing
				if(LeftSpeed == 0 && RightSpeed == 0)
				{

					LeftSpeed = 5;
					RightSpeed = 5;
					MotorControl(3);//Brake Urgent
				}

					//uiM1Speed = (unsigned int)LeftSpeed;
					//uiM2Speed = (unsigned int)RightSpeed;
				}
			}


		//processing with sensor

			for(i=0;i<10;i++)
			{

				FreeMotion[i] = 0x00;
			}

			FreeMotionInterrupt = 0;

	}

	if(FreeMotion[4]==3 && SENSOR_LIFT_LOW_ONE == 0)
	{//lowest sensor//GpioDataRegs.GPBDAT.bit.GPIO58
		ForkLiftControl(0);
	}

	/*if(FreeMotion[4]==9)
	{
		ForkLiftControl(4);
		l_debugvalue = 1;

		if(SENSOR_LIFT_MIDDLE_ONE == 0)
		{
			ForkLiftControl(0);
			l_debugvalue = 2;

			ForkLiftControl(1);
		}
			if(SENSOR_LIFT_MIDDLE_ONE ==0 && SENSOR_FORK_LIMIT_OUT==0)
			{
				ForkLiftControl(0);
				l_debugvalue =3;
			}

		}*/
	   if(FreeMotion[4]==3 && SENSOR_LIFT_LOW_ONE == 0) // Lift down LIMIT
	   {//lowest sensor//GpioDataRegs.GPBDAT.bit.GPIO59
	      ForkLiftControl(0);


	   }

	   if(FreeMotion[4]==1 && SENSOR_FORK_LIMIT_OUT == 0) //Fork OUT LIMIT
	      {
	         ForkLiftControl(0);



	      }
	   if(FreeMotion[4]==2 && SENSOR_FORK_LIMIT_IN == 0) //Fork IN LIMIT
	   {
	      ForkLiftControl(0);
	      FreeMotion[4]=0;

	   }

	   if(SENSOR_LIFT_HIGH_TWO == 0) //Lift up LIMIT
	      {
	         ForkLiftControl(0);
	      }

	///////////////////////////////////
	   if(FreeMotion[4]==9)
	      {
	         ForkLiftControl(4);
	         FreeMotion[4]=4;
	         l_debugvalue = 1;
	      }
	   if(l_debugvalue == 1 && (FreeMotion[4]==4 ||FreeMotion[4]==3) && SENSOR_LIFT_LOW_TWO == 0)
	            {
	               ForkLiftControl(0);
	               l_debugvalue = 3;
	               ForkLiftControl(1);
	               FreeMotion[4]=6;
	            }
	   if(l_debugvalue == 1 && FreeMotion[4]==4 &&(SENSOR_LIFT_MIDDLE_TWO==0 || SENSOR_LIFT_HIGH_ONE==0))
	            {
	               l_debugvalue =1;
	               ForkLiftControl(3);
	               FreeMotion[4]=3;
	            }

	   /////////////////////////
	   if(l_debugvalue == 3 && FreeMotion[4]==6 && SENSOR_FORK_LIMIT_OUT == 0 )
	   {
	      ForkLiftControl(0);
	      l_debugvalue = 4;
	      ForkLiftControl(4);
	      FreeMotion[4]=4;
	   }
	   if(FreeMotion[4]==4 &&l_debugvalue == 4 && SENSOR_LIFT_MIDDLE_ONE == 0 )
	   {
	      ForkLiftControl(0);
	      l_debugvalue = 5;
	      ForkLiftControl(2);
	      FreeMotion[4]=2;

	   }



		/*if(SENSOR_FORK_LIMIT_IN == 0)
			{
				ForkLiftControl(0);
				l_debugvalue = 4;

			}*/

	/*
	if(SENSOR_FORK_LIMIT_OUT == 0)
		{
			ForkLiftControl(0);
			l_debugvalue = 3;
			ForkLiftControl(4);
			l_debugvalue = 4;
			delay_us(1000);
			ForkLiftControl(2);
			l_debugvalue = 5;
if(SENSOR_FORK_LIMIT_IN == 0)
{
	ForkLiftControl(0);
	l_debugvalue = 6;
}

		}*/
}
			/*else if(FreeMotion[4]==3 && GpioDataRegs.GPBDAT.bit.GPIO58 == 0)
			{
							ForkLiftControl(0);
							FreeMotion[4]=0;
			}
*/



// FORK LIFT Control
// uiCon 0: Stop, 1->Fork FW, 2->Fork BW, 3->Lift UP, 4->Lift DN
//--------------------------------------------
void ForkLiftControl(unsigned int uiCon)
{
	// K4 : FORK FW
	// K5 : FORK BW
	// K6 : LIFT UP
	// K7 : LIFT DN
	switch(uiCon)
	{
		case 0: // STOP
			GpioDataRegs.GPADAT.bit.GPIO4 = 0;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO5 = 0;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO6 = 0;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO7 = 0;
			delay_us(1);
			break;
		case 1: // FORK FORWARD
			GpioDataRegs.GPADAT.bit.GPIO4 = 1;
			delay_us(5);
			GpioDataRegs.GPADAT.bit.GPIO5 = 0;
			delay_us(5);
			GpioDataRegs.GPADAT.bit.GPIO6 = 0;
			delay_us(5);
			GpioDataRegs.GPADAT.bit.GPIO7 = 0;
			delay_us(5);

			break;
		case 2: // FORK BACKWARD
			GpioDataRegs.GPADAT.bit.GPIO4 = 0;
			delay_us(5);
			GpioDataRegs.GPADAT.bit.GPIO5 = 1;
			delay_us(5);
			GpioDataRegs.GPADAT.bit.GPIO6 = 0;
			delay_us(5);
			GpioDataRegs.GPADAT.bit.GPIO7 = 0;
			delay_us(5);
			break;
		case 3: // LIFT down
			GpioDataRegs.GPADAT.bit.GPIO4 = 0;
			delay_us(5);
			GpioDataRegs.GPADAT.bit.GPIO5 = 0;
			delay_us(5);
			GpioDataRegs.GPADAT.bit.GPIO6 = 1;
			delay_us(5);
			GpioDataRegs.GPADAT.bit.GPIO7 = 0;
			delay_us(5);
			break;
		case 4: // LIFT up
			GpioDataRegs.GPADAT.bit.GPIO4 = 0;
			delay_us(5);
			GpioDataRegs.GPADAT.bit.GPIO5 = 0;
			delay_us(5);
			GpioDataRegs.GPADAT.bit.GPIO6 = 0;
			delay_us(5);
			GpioDataRegs.GPADAT.bit.GPIO7 = 1;
			delay_us(5);
			break;
		default: // STOP
			GpioDataRegs.GPADAT.bit.GPIO4 = 0;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO5 = 0;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO6 = 0;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO7 = 0;
			delay_us(1);
			break;
	}

}


void ulkeyInit_V(void){

	ulKey = GpioDataRegs.GPBDAT.all;
	ulKey = (ulKey & 0x3FC00000) >> 22;
	ucKey = ulKey ^ 0xff;
}

int PidSpeed_ctrl(int _fYaw)
{
	presentError=abs(desiredAngle-(int)_fYaw);
	int deltaError=lastError-presentError;
	double Temp_KP=presentError*PARAM_KP;
	double Temp_KD=deltaError*PARAM_KD/0.1;
	lastError=presentError;
	double OutPD=Temp_KP+Temp_KD;
	if(OutPD>5.0)
	{
		OutPD=5.0;
	}
	else if(OutPD<=0)
	{
		OutPD=0.0;
	}
	return (int)OutPD;
}
int anglePos(int angle)
{
		if(angle<90 && angle>=0)
		{
			return angle+180;
		}
		else if(angle>270 && angle<=360)
		{
			return angle-180;
		}
		else
		{
			return angle;
		}
}

//Function Name: MotorControl
//Some changes: Survey again with truth table of 4 relays: K0,K1,K2,K3 and then change with new protocol
void MotorControl(unsigned int uiCon)
{
	switch(uiCon)
	{
		case 0: // STOP
			MOTOR_SPEED(0, 0);
			GpioDataRegs.GPADAT.bit.GPIO0 = 0;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO1 = 0;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO2 = 0;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO3 = 0;
			delay_us(1);
			SpeedDecTimer = 0;
			iCartMotionFlag = CART_DEC;
			break;
		case 1: // TURN LEFT
			MOTOR_SPEED(uiM1Speed, uiM2Speed);
			GpioDataRegs.GPADAT.bit.GPIO0 = 1;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO1 = 1;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO2 = 1;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO3 = 1;
			delay_us(1);
			SpeedIncTimer = 0;
			iCartMotionFlag = CART_ACC;
			break;
		case 2: // FORWARD
			MOTOR_SPEED(uiM1Speed, uiM2Speed);
			GpioDataRegs.GPADAT.bit.GPIO0 = 0;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO1 = 1;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO2 = 1;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO3 = 1;
			delay_us(1);
			SpeedIncTimer = 0;
			iCartMotionFlag = CART_ACC;
			break;
		case 3: // BACKWARD
			MOTOR_SPEED(uiM1Speed, uiM2Speed);
			GpioDataRegs.GPADAT.bit.GPIO0 = 1;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO1 = 1;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO2 = 0;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO3 = 1;
			delay_us(1);
			SpeedIncTimer = 0;
			iCartMotionFlag = CART_ACC;
			break;
		case 4: // TURN RIGHT
			MOTOR_SPEED(uiM1Speed, uiM2Speed);
			GpioDataRegs.GPADAT.bit.GPIO0 = 0;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO1 = 1;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO2 = 0;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO3 = 1;
			delay_us(1);
			SpeedIncTimer = 0;
			iCartMotionFlag = CART_ACC;
			break;
		case 5: // ONLY LEFT MOTOR FW
			MOTOR_SPEED(uiM1Speed, uiM2Speed);
			GpioDataRegs.GPADAT.bit.GPIO0 = 1;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO1 = 0;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO2 = 0;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO3 = 1;
			delay_us(1);
			SpeedIncTimer = 0;
			iCartMotionFlag = CART_ACC;
			break;

		case 6: // ONLY LEFT MOTOR BW
			MOTOR_SPEED(uiM1Speed, uiM2Speed);
			GpioDataRegs.GPADAT.bit.GPIO0 = 0;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO1 = 0;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO2 = 1;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO3 = 1;
			delay_us(1);
			SpeedIncTimer = 0;
			iCartMotionFlag = CART_ACC;
			break;

		case 7: // ONLY RIGHT MOTOR FW
			MOTOR_SPEED(uiM1Speed, uiM2Speed);
			GpioDataRegs.GPADAT.bit.GPIO0 = 1;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO1 = 1;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO2 = 0;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO3 = 0;
			delay_us(1);
			SpeedIncTimer = 0;
			iCartMotionFlag = CART_ACC;
			break;

		case 8: // ONLY RIGHT MOTOR BW
			MOTOR_SPEED(uiM1Speed, uiM2Speed);
			GpioDataRegs.GPADAT.bit.GPIO0 = 0;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO1 = 1;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO2 = 0;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO3 = 0;
			delay_us(1);
			SpeedIncTimer = 0;
			iCartMotionFlag = CART_ACC;
			break;

		case 9: // ONLY MOTOR FREE MOTION
			MOTOR_SPEED(uiM1Speed, uiM2Speed);
			GpioDataRegs.GPADAT.bit.GPIO0 = 1;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO1 = 1;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO2 = 0;
			delay_us(1);
			GpioDataRegs.GPADAT.bit.GPIO3 = 1;
			delay_us(1);
			SpeedIncTimer = 0;
			iCartMotionFlag = CART_ACC;

		default: // STOP
			MOTOR_SPEED(0, 0);
			GpioDataRegs.GPADAT.bit.GPIO0 = 0;
			GpioDataRegs.GPADAT.bit.GPIO1 = 0;
			GpioDataRegs.GPADAT.bit.GPIO2 = 0;
			GpioDataRegs.GPADAT.bit.GPIO3 = 0;

			break;
	}

}


void GYRO_Process(void)
{
	uiI2CError = 0;

	uiI2CError =  I2C_WRITE_READBYTES(GYRO_SLAVE_ADDRESS,TEMP, 1, uiGyroData, 8);

	uiRoll = (uiGyroData[3] << 8 ) | (uiGyroData[2] & 0xff);
	uiPitch = (uiGyroData[5] << 8 ) | (uiGyroData[4] & 0xff);
	uiYaw = (uiGyroData[7] << 8 ) | (uiGyroData[6] & 0xff);

	fRoll = (float)uiRoll * 180 / 32767;
	fPitch = (float)uiPitch * 180 / 32767;
	fYaw = (float)uiYaw * 180 / 32767;

	uiRoll = fRoll * 10;
	uiPitch = fPitch * 10;
	uiYaw = fYaw * 10;


	TXABUFFER[0] = 'G';
	uiYawTemp = uiYaw / 1000;
	TXABUFFER[1] = uiYawTemp + '0';
	uiYaw = uiYaw % 1000;
				
	uiYawTemp = uiYaw / 100;
	TXABUFFER[2] = uiYawTemp + '0';
	uiYaw = uiYaw % 100;

	uiYawTemp = uiYaw / 10;
	TXABUFFER[3] = uiYawTemp + '0';

	uiYawTemp = uiYaw % 10;
	TXABUFFER[4] = uiYawTemp + '0';
	TXABUFFER[5] = '0';
	TXABUFFER[6] = '0';
	TXABUFFER[7] = 'E';
	SCIA_PUT_STRING(TXABUFFER, 8);


}

void Interrupt(void)
{

}
void QRProcess(void)
{
	//---------------------------------------------------------------------
	// QRCode Reader Process
	GyroLength = 0x00;

	GyroLength = (ScibRegs.SCIFFRX.all >> 8) & 0x1F;

	if(GyroLength > 0)
	{
		GyroRcv = ScibRegs.SCIRXBUF.all;

		if(GyroRcv == '0' || GyroRcv == '1' || GyroRcv == '2' || GyroRcv == '3' || GyroRcv == '4')
		{
			RXBBUFFER[0] = GyroRcv;

	/*				delay_ms(100);
					GpioDataRegs.GPADAT.bit.GPIO4 = 0;
					delay_ms(500);
					GpioDataRegs.GPADAT.bit.GPIO4 = 1;
					Timer_Monitor = 0;
					delay_ms(100);
					GpioDataRegs.GPADAT.bit.GPIO4 = 0;
					delay_ms(500);
					GpioDataRegs.GPADAT.bit.GPIO4 = 1;*/
		}

		delay_ms(100);
		GpioDataRegs.GPADAT.bit.GPIO4 = 0;
		delay_ms(100);
		GpioDataRegs.GPADAT.bit.GPIO4 = 1;
		Timer_Monitor = 0;

		ScibRegs.SCIFFRX.bit.RXFFOVRCLR=1;  			// Clear Overflow flag
//			ScibRegs.SCIFFRX.bit.RXFFINTCLR=1; 			// Clear Interrupt flag
		PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;		// Must acknowledge the PIE group
	}
}

//---------------------------------------------------------------------------
// CART Speed Control  
//---------------------------------------------------------------------------
void CART_SPEED_INC(unsigned int TimeInterval, unsigned int Acc, unsigned int Speed)
{
	float fCurrSpeed;
	unsigned int currSpeed;
	//int i;
	//Acc = uiM1Acc;
	//Speed = uiM1Speed;

	//for(i=0; i++; i < uiM1Acc)
	//{
	//	TimeInterval = i;
		if(TimeInterval < uiM1Acc)
		{
		// Speed increasing
		fCurrSpeed = (float)Speed*(float)(TimeInterval/Acc);
		currSpeed = (unsigned int)fCurrSpeed;
		MOTOR_SPEED(currSpeed, currSpeed);	
		}
		else
		{
		// currSpeed = Speed
		MOTOR_SPEED(Speed, Speed);	
		iCartMotionFlag = CART_READY;
		}
	//}
	//TimeInterval = 0;
}

void CART_SPEED_DEC(unsigned int TimeInterval, unsigned int Acc, unsigned int Speed)
{
	float fCurrSpeed;
	unsigned int currSpeed;
	//int i;
	//uiM1Acc
	//uiM1Speed
	///for(i=0; i++; i < uiM1Acc)
	//{
		//TimeInterval = i;

		if(TimeInterval < uiM1Acc)
		{
		// Speed increasing
		fCurrSpeed = (float)Speed*(float)(TimeInterval/uiM1Acc);
		currSpeed = Speed - (unsigned int)fCurrSpeed;
		MOTOR_SPEED(currSpeed, currSpeed);	
		}
		else
		{
		// currSpeed = Speed
		MOTOR_SPEED(0, 0);	
		iCartMotionFlag = CART_STOP;
		}
	//}
	//TimeInterval = 0;
}
