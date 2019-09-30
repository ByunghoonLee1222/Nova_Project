//###############################################################################################################
// file name: DSP28335_User_Define.h
//###############################################################################################################

#ifndef DSP28335_USER_DEFINE_H
#define DSP28335_USER_DEFINE_H

#ifdef __cplusplus
extern "C" {
#endif

#define BOOL			char
#define BYTE			unsigned char
#define WORD			unsigned int
#define LONG			unsigned long
#define LLONG			unsigned long long

#define PBYTE(X)		*(volatile BYTE *)(X)
#define PWORD(X)		*(volatile WORD *)(X)
#define PLONG(X)		*(volatile LONG *)(X)
#define PLLONG(X)		*(volatile LLONG *)(X)

#define NOP				asm("	NOP")

// Bit define
#define BIT_MASK(bit)				(1 << (bit))
#define GETBIT(val, bit)			(((val) & BIT_MASK(bit)) >> (bit))
#define SETBIT(val, bit)			(val |= BIT_MASK(bit))
#define CLRBIT(val, bit)			(val &= ~BIT_MASK(bit))
#define TGLBIT(val, bit)			(val ^= BIT_MASK(bit))
#define BIT_IS_SET(val, bit)		(val & BIT_MASK(bit))
#define BIT_IS_CLR(val, bit)		(~val & BIT_MASK(bit))

// bit define
#define BIT0_POS    	0
#define BIT1_POS    	1
#define BIT2_POS    	2
#define BIT3_POS    	3
#define BIT4_POS    	4
#define BIT5_POS    	5
#define BIT6_POS    	6
#define BIT7_POS    	7
#define BIT8_POS    	8
#define BIT9_POS    	9
#define BIT10_POS   	10
#define BIT11_POS   	11
#define BIT12_POS   	12
#define BIT13_POS   	13
#define BIT14_POS   	14
#define BIT15_POS   	15

// Bit Mask define
#define BIT0_MASK    	0x0001
#define BIT1_MASK    	0x0002
#define BIT2_MASK    	0x0004
#define BIT3_MASK    	0x0008
#define BIT4_MASK    	0x0010
#define BIT5_MASK    	0x0020
#define BIT6_MASK    	0x0040
#define BIT7_MASK    	0x0080
#define BIT8_MASK    	0x0100
#define BIT9_MASK    	0x0200
#define BIT10_MASK   	0x0400
#define BIT11_MASK   	0x0800
#define BIT12_MASK   	0x1000
#define BIT13_MASK   	0x2000
#define BIT14_MASK   	0x4000
#define BIT15_MASK   	0x8000


/* micro-seconds delay function */
#define delay_us(us)		DELAY_US(us)	 

/* milli-seconds delay function */
#define delay_ms(ms)		DELAY_US(ms*1000)


#define EEPROM_SLAVE_ADD        0x0050

#define SCB_RX_NUM              80

//#define ON                      1
//#define OFF                     0
#define PULL_UP                 1
#define PULL_DOWN               0
#define EN                      0
#define DI                      1
#define HZ                      1
#define Y0                      0
#define Y1                      1


//----------------------------------------------------------------
//                  Main Board definition
//----------------------------------------------------------------
#define TEST_LED0				*(volatile unsigned int *)0x270800
#define TEST_LED1				*(volatile unsigned int *)0x270A00
#define REF_CTRL            	*(volatile unsigned int *)0x200200
#define MUX1_EN                 *(volatile unsigned int *)0x200600
#define MUX1_SW                 *(volatile unsigned int *)0x200000
#define MUX2_EN                 *(volatile unsigned int *)0x200800
#define MUX2_SW                 *(volatile unsigned int *)0x200400
#define REF_RELAY               *(volatile unsigned int *)0x210400
#define DISCRETE_OUT            *(volatile unsigned int *)0x210000  //SOL(1~13),OK_LAMP(14),NG_LAMP(15),SOL(16)
#define DO_RELAY                *(volatile unsigned int *)0x210200  //RELAY K1/K2/K3/K4
#define BUZZER_SD               *(volatile unsigned int *)0x210600  //BUZZER 
#define LED_CTRL                *(volatile unsigned int *)0x200A00  //ILED_A1~A4 

#define LCD_WR_REGISTER			*(volatile unsigned int *)0x270E00
#define LCD_RD_REGISTER		    *(volatile unsigned int *)0x270E02
#define LCD_WR_DATA			    *(volatile unsigned int *)0x270E01
#define LCD_RD_DATA				*(volatile unsigned int *)0x270E03
//LCD_CS1(0x270C00),XRD,XWE is no use
//LCD_CS2(E), XA0(READ(H)/WRITE(L)), XA1(RS(DATA(H)/REGISTER(L))




//----------------------------------------------------------------
//                  EEPROM Block Write Protection
//----------------------------------------------------------------
#define NONE_OF_THE_MEMORY            0x00 // level : 0 (None)
#define ONE_QUARTER                   0x04 // level : 1 (0600~07FF are protected)
#define ONE_HALF                      0x08 // level : 2 (0400~07FF are protected)
#define ALL_OF_THE_MEMORY             0x0C // level : 3 (0000~07FF are protected)


//----------------------------------------------------------------
//                  NiCC Definitions
//----------------------------------------------------------------
#define FALSE                   0
#define TRUE                    1


//----------------------------------------------------------------
//                  NiCC Input Pins
//----------------------------------------------------------------
#define IN_EMG                  GpioDataRegs.GPBDAT.bit.GPIO53
#define IN_SC                   GpioDataRegs.GPBDAT.bit.GPIO54
#define IN_SF                   GpioDataRegs.GPBDAT.bit.GPIO55
#define IN_MU                   GpioDataRegs.GPBDAT.bit.GPIO56
#define IN_MD                   GpioDataRegs.GPBDAT.bit.GPIO57
#define IN_M2                   GpioDataRegs.GPBDAT.bit.GPIO58
#define IN_M3                   GpioDataRegs.GPBDAT.bit.GPIO59
#define IN_M4                   GpioDataRegs.GPBDAT.bit.GPIO60
#define IN_SP1                  GpioDataRegs.GPBDAT.bit.GPIO61
#define IN_SP2                  GpioDataRegs.GPBDAT.bit.GPIO62
#define IN_EN                   GpioDataRegs.GPBDAT.bit.GPIO63
#define IN_E                    GpioDataRegs.GPADAT.bit.GPIO25
#define IN_W                    GpioDataRegs.GPADAT.bit.GPIO26
#define IN_EW2                  GpioDataRegs.GPADAT.bit.GPIO27
#define IN_EW3                  GpioDataRegs.GPBDAT.bit.GPIO32
#define IN_EW4                  GpioDataRegs.GPBDAT.bit.GPIO33
#define IN_N                    GpioDataRegs.GPBDAT.bit.GPIO48
#define IN_S                    GpioDataRegs.GPBDAT.bit.GPIO49
#define IN_NS2                  GpioDataRegs.GPBDAT.bit.GPIO50
#define IN_NS3                  GpioDataRegs.GPBDAT.bit.GPIO51
#define IN_NS4                  GpioDataRegs.GPBDAT.bit.GPIO52
#define IN_BT                   GpioDataRegs.GPADAT.bit.GPIO8
#define IN_DIP1                 GpioDataRegs.GPADAT.bit.GPIO31
#define IN_DIP2                 GpioDataRegs.GPADAT.bit.GPIO30
#define IN_DIP3                 GpioDataRegs.GPADAT.bit.GPIO14
#define IN_DIP4                 GpioDataRegs.GPADAT.bit.GPIO15


//----------------------------------------------------------------
//                  NiCC Input Control
//----------------------------------------------------------------
#define IN_E_W                  (((~IN_W & 0x1) << 1) | (~IN_E & 0x1))
#define IN_EW_STEP              (((~IN_EW4 & 0x1) << 2) | ((~IN_EW3 & 0x1) << 1) | (~IN_EW2 & 0x1))
#define IN_N_S                  (((~IN_S & 0x1) << 1) | (~IN_N & 0x01))
#define IN_NS_STEP              (((~IN_NS4 & 0x1) << 2) | ((~IN_NS3 & 0x1) << 1) | (~IN_NS2 & 0x1))
#define IN_U_D                  (((~IN_MD & 0x1) << 1) | (~IN_MU & 0x1))
#define IN_UD_STEP              (((~IN_M4 & 0x1) << 2) | ((~IN_M3 & 0x1) << 1) | (~IN_M2 & 0x1))
#define BT_CONNECT              (~IN_BT & 0x1)


//----------------------------------------------------------------
//                  NiCC Output Status
//----------------------------------------------------------------
#define OUT_E                   GpioDataRegs.GPADAT.bit.GPIO0
#define OUT_W                   GpioDataRegs.GPADAT.bit.GPIO1
#define OUT_N                   GpioDataRegs.GPADAT.bit.GPIO2
#define OUT_S                   GpioDataRegs.GPADAT.bit.GPIO3
#define OUT_SP1                 GpioDataRegs.GPADAT.bit.GPIO4
#define OUT_SP2                 GpioDataRegs.GPADAT.bit.GPIO5
#define OUT_OUTAB               GpioDataRegs.GPADAT.bit.GPIO6


//----------------------------------------------------------------
//                  NiCC Output Control
//----------------------------------------------------------------
#define EAST_RUN                {GpioDataRegs.GPASET.bit.GPIO0 = 1;GpioDataRegs.GPACLEAR.bit.GPIO1 = 1;}
#define WEST_RUN                {GpioDataRegs.GPASET.bit.GPIO1 = 1;GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;}
#define EAST_WEST_STOP          {GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;GpioDataRegs.GPACLEAR.bit.GPIO1 = 1;}
#define NORTH_RUN               {GpioDataRegs.GPASET.bit.GPIO2 = 1;GpioDataRegs.GPACLEAR.bit.GPIO3 = 1;}
#define SOUTH_RUN               {GpioDataRegs.GPASET.bit.GPIO3 = 1;GpioDataRegs.GPACLEAR.bit.GPIO2 = 1;}
#define NORTH_SOUTH_STOP        {GpioDataRegs.GPACLEAR.bit.GPIO2 = 1;GpioDataRegs.GPACLEAR.bit.GPIO3 = 1;}
#define SP1_RUN                 {GpioDataRegs.GPASET.bit.GPIO4 = 1;}
#define SP1_STOP                {GpioDataRegs.GPACLEAR.bit.GPIO4 = 1;}
#define SP2_RUN                 {GpioDataRegs.GPASET.bit.GPIO5 = 1;}
#define SP2_STOP                {GpioDataRegs.GPACLEAR.bit.GPIO5 = 1;}
#define OUT_RUN                 {GpioDataRegs.GPASET.bit.GPIO6 = 1;}
#define OUT_STOP                {GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;}


//----------------------------------------------------------------
//                  NiCC Data Definitions from Sensor Module
//----------------------------------------------------------------
#define ACC_X                   0
#define ACC_Y                   1
#define ACC_Z                   2
#define GYR_X                   3
#define GYR_Y                   4
#define TEMPE                   5
#define ANG_X                   6
#define ANG_Y                   7

//
#define INC                     1
#define DEC                     2

//
#define N 31
#define M_PI 3.14159

//
#define X_PLUS                  0
#define X_MINUS                 1
#define Y_PLUS                  2
#define Y_MINUS                 3

//Select AIO0~AIO64 Channel
#define X0_Y0                   0
#define X1_Y1                   1
#define X2_Y2                   2
#define X3_Y3                   3

#define CH_1                    1
#define CH_2                    2
#define CH_3                    3
#define CH_4                    4
#define CH_5                    5
#define CH_6                    6
#define CH_7                    7
#define CH_8                    8
#define CH_9                    9
#define CH_10                   10
#define CH_11                   11
#define CH_12                   12
#define CH_13                   13
#define CH_14                   14
#define CH_15                   15
#define CH_16                   16
#define CH_ALL                  0

#define MODE_SW                 0xF37F
#define LEFT_SW                 0xF3FE
#define UP_SW                   0xF3FD
#define RIGHT_SW                0xF3FB
#define SET_SW                  0xF3F7
#define FN_SW                   0xF3EF
#define RUN_SW                  0xF3DF
#define DOWN_SW                 0xF3BF
#define BACK_SW                 0xF2FF
#define ENTER_SW                0xF1FF
#define FN_SET_SW               0xF3E7
#define RUN_UP_SW               0xF3DD
#define RUN_DOWN_SW             0xF39F
#define FN_UP_SW                0xF3ED
#define FN_DOWN_SW              0xF3AF
#define BACK_UP_SW              0xF2FD
#define BACK_DOWN_SW            0xF2BF
#define FN_RUN_SW               0xF3CF
#define FN_RUN_UP_SW            0xF3CD
#define FN_RUN_DOWN_SW          0xF38F
#define DEFAULT_SW              0xF3FF
//----------------------------------------------------------------
//                  Interface Board definition
//----------------------------------------------------------------
#define INTERFACE_RELAY          *(volatile unsigned int *)0x220200  // RELAY K1/K2/K3/K4/K5/K6/K7/K8
#define INTERFACE_MIC            *(volatile unsigned int *)0x220600  // MIC1CON, MIC2CON, VAI1CON
#define INTERFACE_DOME           *(volatile unsigned int *)0x220000
#define DOME_EN0                0x10 
#define DOME_EN1                0x08
#define DOME_EN_ALL             0x00

#define X0                      0 
#define X1                      1 
#define X2                      2 
#define X3                      3 
#define X4                      4 
#define X5                      5 
#define X6                      6 
#define X7                      7 

#define START_SW                0x73FF
#define VACCUM_SW               0xB3FF
#define RESET_SW                0xD3FF
#define STARTL0_SW              0xE3FF
#define STARTALL_SW             0x63FF

//AD Converter
#define ADC_usDELAY  5000L

//SCI Environment
#define SCIDA           1
#define SCIDB           2
#define SCIDC           3
#define NO_PARITY       0
#define EN_PARITY       1
#define ODD_PARITY      0
#define EVEN_PARITY     1
#define ONE_STOP_BIT    0
#define TWO_STOP_BIT    1
#define BPS_9600        0x1E7
#define BPS_19200       0xF3
#define BPS_38400       0x79
#define BPS_115200      0x28





#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif // end of DSP28335_MD_DEFINE_H definition
