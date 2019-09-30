
typedef struct
{
	double error;
	double Pre_error;
	double Integral;
	double Derivative;
	double Output;
	double KP;
	double KI;
	double KD;
	double Value_des;
	double Value_act;
	double dt;
}PIDProfile;

void PIDInit_V(PIDProfile *PID_st, double kp,double ki,double kd);
unsigned char PIDCtrl_UB(PIDProfile *PID_st, double dt);
