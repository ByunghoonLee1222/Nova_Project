//################################################################
// 	NiCC_MC_main.h
//################################################################

#define ANGLE_STEP       10

#include "DSP28335_User_Define.h"
#include "math.h"
#include "stdio.h"

#pragma CODE_SECTION(TIMER0_SW_ISR, "ramfuncs");
//#pragma CODE_SECTION(RX_STACK, "ramfuncs");
//#pragma CODE_SECTION(FIR_LPF_Coef, "ramfuncs");

//--------------------------------------------------------------------------------------------
//                       Initialize System Function Definition 
//--------------------------------------------------------------------------------------------
void INIT_SYSTEM(void);
void ENABLE_INT(void);

//--------------------------------------------------------------------------------------------
//                       System Function Definition
//--------------------------------------------------------------------------------------------
void INIT_USER_GPIO(void);
void INIT_USER_XINTF(void);
void INIT_ADC(void);
void INIT_SCI_UNIT(unsigned int port, unsigned int baud_rate, unsigned int stop, 
              unsigned int parity,  unsigned int odd_even);
void INIT_SCI_ALL(void);
void SCIA_PUTCH(char value);
void SCIB_PUTCH(char value);
void SCIC_PUTCH(unsigned int value);
void SCIA_PUT_STRING(char *string, int len);
void SCIB_PUT_STRING(char *string, int len);
void SCIC_PUT_STRING(char *string, int len);
unsigned int SPI_SEND_8(unsigned int value);
unsigned int EPR_READ_STATUS_REGISTER(void);
unsigned int EPR_WRITE_STATUS_REGISTER(unsigned int val);
unsigned int EPR_SET_WRITE_ENABLE(void);
unsigned int EPR_READ(unsigned int addr);
unsigned int EPR_WRITE(unsigned int addr, unsigned int val);
unsigned int SPI_SEND_16(unsigned int value);
unsigned int SPI_VOL_SEND_16(unsigned int value);
unsigned int MOTOR_SPEED(unsigned int uiM1, unsigned int uiM2);
unsigned long SPI_SEND_24(unsigned long value);
unsigned long SPI_CUR_SEND_EW(unsigned long value);
void INIT_SPI(void);
void INIT_I2C(void);
void EEPROM_WRITEDATA(unsigned int slave_address,unsigned int eeprom_address, unsigned int data);
unsigned int EEPROM_READDATA(unsigned int slave_address,unsigned int eeprom_address);
void INIT_TIMER01(void);
void INIT_EPWM5A(void);
void PWM_OUT(unsigned int fre, unsigned int ctr);


//--------------------------------------------------------------------------------------------
//                       ADC Sub Function Definition
//--------------------------------------------------------------------------------------------
void READ_ADC_ALL(unsigned int *ADCH);
unsigned int READ_ADC_ONE(unsigned int ch);


//--------------------------------------------------------------------------------------------
//                       Interrupt Sub Function Definition 
//--------------------------------------------------------------------------------------------
extern interrupt void TIMER0_SW_ISR(void);
extern interrupt void TIMER1_SW_ISR(void);
interrupt void SCIA_GETCH_ISR(void);
interrupt void SCIB_GETCH_ISR(void);
interrupt void SCIC_GETCH_ISR(void);


//--------------------------------------------------------------------------------------------
//                       NiCC Control Function Definition 
//--------------------------------------------------------------------------------------------
int STR_TO_INT(char *str, int start, int finish);
void RX_STACK1(int num, int dat, int *buf);
void RX_STACK2(int num, int dat, int *buf);
void FIR_LPF_Coef(float fs, float fc, float *h);
float Convolute1(int num, int *dat, float *coe);
float Convolute2(int num, int *dat, float *coe);
float STDE_EW(int num, float dat);
float STDE_NS(int num, float dat);
float EW_AV_STDE(int num, float dat);
float NS_AV_STDE(int num, float dat);
extern void UPDOWN(unsigned int dir, unsigned int step);


//--------------------------------------------------------------------------------------------
//                       Motorized CART Function Definition 
//--------------------------------------------------------------------------------------------
void MotorControl(unsigned int uiCon);
unsigned int I2C_WRITEBYTES(unsigned int slave_address, unsigned int *data, unsigned int num);
unsigned int I2C_READBYTES(unsigned int slave_address, unsigned int *data, unsigned int num);
unsigned int I2C_WRITE_READBYTES(unsigned int slave_address, unsigned int *wr_data, unsigned int wr_num
                                                           , unsigned int *rd_data, unsigned int rd_num);
void GYRO_Process(void);
void QRProcess(void);
void CART_SPEED_INC(unsigned int TimeInterval, unsigned int Acc, unsigned int Speed);
void CART_SPEED_DEC(unsigned int TimeInterval, unsigned int Acc, unsigned int Speed);
