/***************************************
	NOVA Co.,Ltd.
	Motorized Cart Controller
	TMS320F28335

	2015 / 06 / 08
***************************************/
#include "DSP28335_User_Define.h"
#include "math.h"
#include "stdio.h"
#include "MCart_main.h"

extern unsigned char RcvComplete;

unsigned int SpeedIncTimer, SpeedDecTimer;
unsigned int Timer_LED, Timer_Motion, Timer_Monitor, Timer_Run, Timer_zero;
int FreeMotionInterrupt=0,flagDebug=0;

char RXABUFFER[10], RXBBUFFER[100]={0x00}, RXCBUFFER[16], TXABUFFER[20], TXBBUFFER[8], FreeMotion[10];

// Timer0 Interrupt Service Routine
interrupt void TIMER0_SW_ISR(void) //defaut=0xF3FF
{
	Timer_LED++;
	Timer_Monitor++;
	SpeedIncTimer++;
	SpeedDecTimer++;
	Timer_Motion++;
/*	Timer_UD++;
	Timer_Monitor++;
	Timer_Run++;
	Timer_EW++;
	Timer_NS++;
	Timer_RemLen++;
	Timer_EWOp++;
	Timer_NSOp++;*/

//	UPDOWN(UP_DOWN, UDCurStep);
	// Acknowledge this interrupt to receive more interrupts from group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

// Timer1 Interrupt Service Routine
interrupt void TIMER1_SW_ISR(void) //defaut=0xF3FF
{
	EDIS;
}

//---------------------------------------------------------------------------
// I2C Write datas
//---------------------------------------------------------------------------

unsigned int I2C_WRITEBYTES(unsigned int slave_address, unsigned int *data, unsigned int num)
{
  unsigned int i,j;
  unsigned int i2c_error;

  i2c_error = 0; //init i2c error

  if((I2caRegs.I2CMDR.bit.STP == 1) | (I2caRegs.I2CSTR.bit.BB == 1))
    {
	  i2c_error = ((I2caRegs.I2CMDR.bit.STP << 1) | (I2caRegs.I2CSTR.bit.BB)) & 0x0003;
      return i2c_error;
    }
  else
    {

      I2caRegs.I2CSAR = slave_address & 0x007F;         // 7bit address mode Setup Slave address(0X0000~0X1FFF)
      I2caRegs.I2CCNT = num;                            // number of data
   //   TEST_LED = 0x0001; //led off
      I2caRegs.I2CFFTX.bit.TXFFST == 0;                  //init tx buffer
      I2caRegs.I2CMDR.all = 0x6E20;                     //SEND Slave Module

      i=0; j=0;
      while(j <= 2000) //max = 20ms wait and then error(change 600us -->20ms-2014/02/23
       {
        if(I2caRegs.I2CFFTX.bit.TXFFST == 0)
          {
           I2caRegs.I2CDXR = data[i] & 0x00FF;
           i=i+1;
		   if(i == num)
		     {j = 2001;}
           else
             {j=0;}
          }
        else
		  {
           delay_us(10);
           j=j+1;
	      }
        }

       j=0;
       while(j <= 2000) //max 600us waiting
	   {
         i2c_error = ((I2caRegs.I2CMDR.bit.STP << 1) | (I2caRegs.I2CSTR.bit.BB)) & 0x0003;
         if(i2c_error == 0)
		  j=2001;
         else
		  {
		   delay_us(10);
		   j= j+1;
		  }
	    }
   //   TEST_LED = 0x0000; //led off
      return i2c_error;
    }
}

//---------------------------------------------------------------------------
// I2C read datas
//---------------------------------------------------------------------------
unsigned int I2C_READBYTES(unsigned int slave_address, unsigned int *data, unsigned int num)
{
  unsigned int i,j;
  unsigned int i2c_error;

  i2c_error = 0; //init i2c error

  if((I2caRegs.I2CMDR.bit.STP == 1) | (I2caRegs.I2CSTR.bit.BB == 1))
    {
	  i2c_error = ((I2caRegs.I2CMDR.bit.STP << 1) | (I2caRegs.I2CSTR.bit.BB)) & 0x0003;
      return i2c_error;
    }
   else
    {
      I2caRegs.I2CSAR = slave_address & 0x007F;         // Setup Slave address
      I2caRegs.I2CCNT = num;                            // read byte number
     // TEST_LED = 0x0001; //led off
      I2caRegs.I2CFFRX.bit.RXFFST = 0;                  //init rx buffer
      I2caRegs.I2CMDR.all = 0x6C20;                     // Send slave address
      i=0; j=0;
      while(j <= 2000) //max = 20ms wait and then error change 600us ==>20ms(2012/02/23)
       {
        if(I2caRegs.I2CFFRX.bit.RXFFST >= 1)
          {
           data[i] = I2caRegs.I2CDRR;
           i=i+1;
		   if(i == num)
		     j = 2001;
           else
             j=0;
          }
        else
		  {
           delay_us(10);
           j=j+1;
	      }
        }

	   j=0;
       while(j <= 2000) //max 20ms waiting(change 40us --> 20ms-2014/02/23)
	   {
         i2c_error = ((I2caRegs.I2CMDR.bit.STP << 1) | (I2caRegs.I2CSTR.bit.BB)) & 0x0003;
         if(i2c_error == 0)
		   { j=2001;
		  //   TEST_LED = 0x0000; //led off
           }
         else
		  {
		   delay_us(10);
		   j= j+1;
		  }
	    }
     return i2c_error;
    }
}


//---------------------------------------------------------------------------
// WriteRead
//---------------------------------------------------------------------------
unsigned int I2C_WRITE_READBYTES(unsigned int slave_address,unsigned int *wr_data, unsigned int wr_num
                                                           ,unsigned int *rd_data, unsigned int rd_num)
{
  unsigned int i2c_error;
  unsigned int i,j;
  i2c_error = 0; //init i2c error

  if((I2caRegs.I2CMDR.bit.STP == 1) | (I2caRegs.I2CSTR.bit.BB == 1))
    {
	  i2c_error = ((I2caRegs.I2CMDR.bit.STP << 1) | (I2caRegs.I2CSTR.bit.BB)) & 0x0003;
      return i2c_error;
    }
  else
    {
      I2caRegs.I2CSAR = slave_address & 0x007F;         // Setup Slave address
      I2caRegs.I2CCNT = wr_num;                         // Write byte number
//    TEST_LED = 0x0001;
      I2caRegs.I2CFFTX.bit.TXFFST == 0;                  //init tx buffer
      I2caRegs.I2CMDR.all = 0x6620;			            // Write data(no stop bit)

      i=0; j=0;
      while(j <= 60) //send write byte, max = 600us wait and then error
       {
        if(I2caRegs.I2CFFTX.bit.TXFFST == 0)
          {
           I2caRegs.I2CDXR = wr_data[i] & 0x00FF;
           i=i+1;
		   if(i == wr_num)
		     {j = 61;}
           else
             {j=0;}
          }
        else
		  {
           delay_us(10);
           j=j+1;
	      }
        }

      // TEST_LED = 0x0000;
       j = 0;
       while(j <= 60) //waiting register access - 600us waiting
	   {
         if(I2caRegs.I2CSTR.bit.ARDY == 1)    // Register-Acess-ready
		  j=61;
         else
		  {
		   delay_us(10);
		   j= j+1;
		  }
	    }

      I2caRegs.I2CCNT = rd_num;                         // read one byte data
      I2caRegs.I2CFFRX.bit.RXFFST = 0;                  //init rx buffer
      //TEST_LED = 0x0001;
      I2caRegs.I2CMDR.all = 0x2C20;                     // Send restart as master receiver(stop bit)

      i=0; j=0;
      while(j <= 60) //max = 600us wait and then error
       {
        if(I2caRegs.I2CFFRX.bit.RXFFST >= 1)
          {
           rd_data[i] = I2caRegs.I2CDRR;
           i=i+1;
		   if(i == rd_num)
		     j = 61;
           else
             j=0;
          }
        else
		  {
           delay_us(10);
           j=j+1;
	      }
        }

	   j=0;
       while(j <= 5) //max 40us waiting for error check
	   {
         i2c_error = ((I2caRegs.I2CMDR.bit.STP << 1) | (I2caRegs.I2CSTR.bit.BB)) & 0x0003;
         if(i2c_error == 0)
		   { j=6;
		   //  TEST_LED = 0x0000; //led off
           }
         else
		  {
		   delay_us(10);
		   j= j+1;
		  }
	    }
    // TEST_LED = 0x0000; //led off
     return i2c_error;
   }
}

// Define Gpio, Address Line and Data Line
void INIT_USER_GPIO(void)
{
	EALLOW;						// Enable EALLOW protected register access

//---------------------------------------------------------------------------------------------
//--------------------------------------- Group A pins ----------------------------------------
//---------------------------------------------------------------------------------------------
	GpioCtrlRegs.GPAPUD.all   = 0x00000000;			// All group B pullups enabled
	GpioCtrlRegs.GPACTRL.all  = 0x00000000;			// QUALPRD = SYSCLKOUT for all group A GPIO
	GpioCtrlRegs.GPAQSEL1.all = 0x00000000;			// No qualification for all group A GPIO 0-15
	GpioCtrlRegs.GPAQSEL2.all = 0x00000000;			// No qualification for all group A GPIO 16-31

	GpioCtrlRegs.GPAMUX1.bit.GPIO0  = 0;			// 0=GPIO_0  1=EPWM1A     2=rsvd       3=rsvd
    GpioCtrlRegs.GPADIR.bit.GPIO0   = 1;	        // OUTPUT(K0_CTRL)

	GpioCtrlRegs.GPAMUX1.bit.GPIO1  = 0;			// 0=GPIO_1  1=EPWM1B     2=ECAP6      3=rsvd
	GpioCtrlRegs.GPADIR.bit.GPIO1   = 1;	        // OUTPUT(K1_CTRL)

	GpioCtrlRegs.GPAMUX1.bit.GPIO2  = 0;			// 0=GPIO_2  1=EPWM2A     2=rsvd       3=rsvd
    GpioCtrlRegs.GPADIR.bit.GPIO2   = 1;	        // OUTPUT(K2_CTRL)

	GpioCtrlRegs.GPAMUX1.bit.GPIO3  = 0;			// 0=GPIO_3  1=EPWM2B     2=ECAP5      3=rsvd
    GpioCtrlRegs.GPADIR.bit.GPIO3   = 1;	        // OUTPUT(K3_CTRL)

	GpioCtrlRegs.GPAMUX1.bit.GPIO4  = 0;			// 0=GPIO_4  1=EPWM3A     2=rsvd       3=rsvd
	GpioCtrlRegs.GPADIR.bit.GPIO4   = 1;	        // OUTPUT(K4_CTRL)

	GpioCtrlRegs.GPAMUX1.bit.GPIO5  = 0;			// 0=GPIO_5  1=EPWM3B     2=MFSRA      3=ECAP1
	GpioCtrlRegs.GPADIR.bit.GPIO5   = 1;	        // OUTPUT(K5_CTRL)

	GpioCtrlRegs.GPAMUX1.bit.GPIO6  = 0;			// 0=GPIO_6  1=EPWM4A     2=EPWMSYNCI  3=EPWMSYNCO
	GpioCtrlRegs.GPADIR.bit.GPIO6   = 1;	        // OUTPUT(K6_CTRL)

	GpioCtrlRegs.GPAMUX1.bit.GPIO7  = 0;			// 0=GPIO_7  1=EPWM4B     2=MCLKRA     3=ECAP2
	GpioCtrlRegs.GPADIR.bit.GPIO7   = 1;	        // OUTPUT(K7_CTRL)

	GpioCtrlRegs.GPAMUX1.bit.GPIO8  = 0;			// 0=GPIO_8  1=EPWM5A     2=CANTXB     3=ADCSOCAO
    GpioCtrlRegs.GPADIR.bit.GPIO8   = 0;            // INPUT(NC)

	GpioCtrlRegs.GPAMUX1.bit.GPIO9  = 0;			// 0=GPIO_9  1=EPWM5B     2=SCITXDB    3=ECAP3
	GpioCtrlRegs.GPADIR.bit.GPIO9   = 0;	        // INPUT(NC)

	GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;			// 0=GPIO_10  1=EPWM6A     2=CANRXB     3=ADCSOCBO
	GpioCtrlRegs.GPADIR.bit.GPIO10  = 0;	        // INPUT(NC)

	GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;			// 0=GPIO_11  1=EPWM6B     2=SCIRXDB    3=ECAP4
	GpioCtrlRegs.GPADIR.bit.GPIO11  = 0;	        // INPUT(NC)

	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;			// 0=GPIO_12  1=TZ1        2=CANTXB     3=SPISIMOB
	GpioCtrlRegs.GPADIR.bit.GPIO12  = 0;	        // INPUT(NC)

	GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;			// 0=GPIO_13  1=TZ2        2=CANRXB     3=SPISOMIB
	GpioCtrlRegs.GPADIR.bit.GPIO13  = 0;	        // INPUT(NC)

	GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0;			// 0=GPIO_14  1=TZ3/XHOLD  2=SCITXDB    3=SPICLKB
	GpioCtrlRegs.GPADIR.bit.GPIO14  = 0;	        // INPUT(SW-3)

	GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0;			// 0=GPIO_15  1=TZ4/XHOLDA 2=SCIRXDB    3=SPISTEB
	GpioCtrlRegs.GPADIR.bit.GPIO15  = 0;	        // OUTPUT(SW-4)

	GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3;           // asynch input
    GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3;           // asynch input
    GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3;           // asynch input

	GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1;			// 0=GPIO  1=SPISIMOA   2=CANTXB     3=TZ5
	GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1;			// 0=GPIO  1=SPISOMIA   2=CANRXB     3=TZ6
	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1;			// 0=GPIO  1=SPICLKA    2=SCITXDB    3=CANRXA

	GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 0;			// 0=GPIO  1=SPISTEA    2=SCIRXDB    3=CANTXA
	GpioCtrlRegs.GPADIR.bit.GPIO19  = 1;	        // OUTPUT(DAC CS)


	GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;			// 0=GPIO_20  1=EQEP1A     2=MDXA       3=CANTXB
	GpioCtrlRegs.GPADIR.bit.GPIO20  = 1;	        // OUTPUT(WIFI FUNCTION1, WALE UP)

	GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0;			// 0=GPIO_21  1=EQEP1B     2=MDRA       3=CANRXB
	GpioCtrlRegs.GPADIR.bit.GPIO21  = 1;	        // OUTPUT(WIFI FUNCTION2, FACTORY RESET)

	GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 3;			// 0=GPIO  1=EQEP1S     2=MCLKXA     3=SCITXDB
    GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;           // asynch input
	GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 3;			// 0=GPIO  1=EQEP1I     2=MFSXA      3=SCIRXDB

	GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;			// 0=GPIO_24  1=ECAP1      2=EQEP2A     3=MDXB
	GpioCtrlRegs.GPADIR.bit.GPIO24  = 1;	        // OUTPUT(EEPROM_CS)

	GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0;			// 0=GPIO_25 1=ECAP2      2=EQEP2B     3=MDRB
	GpioCtrlRegs.GPADIR.bit.GPIO25  = 0;	        // INPUT(LOW BATTERY)

	GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;			// 0=GPIO_26  1=ECAP3      2=EQEP2I     3=MCLKXB
	GpioCtrlRegs.GPADIR.bit.GPIO26  = 0;	        // INPUT(NC)

	GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0;			// 0=GPIO_27  1=ECAP4      2=EQEP2S     3=MFSXB
	GpioCtrlRegs.GPADIR.bit.GPIO27  = 0;	        // INPUT(NC)

    GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;           // Asynch input
	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;			// 0=GPIO  1=SCIRXDA    2=XZCS6      3=XZCS6
	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;			// 0=GPIO  1=SCITXDA    2=XA19       3=XA19

	GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 0;			// 0=GPIO  1=CANRXA     2=XA18       3=XA18
	GpioCtrlRegs.GPADIR.bit.GPIO30  = 0;	        // INPUT(SW-2)

	GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0;			// 0=GPIO  1=CANTXA     2=XA17       3=XA17
	GpioCtrlRegs.GPADIR.bit.GPIO31  = 0;	        // INPUT(SW-1)

//---------------------------------------------------------------------------------------------
//--------------------------------------- Group B pins ----------------------------------------
//---------------------------------------------------------------------------------------------
	GpioCtrlRegs.GPBPUD.all   = 0x00000000;			// All group B pullups enabled
	GpioCtrlRegs.GPBCTRL.all  = 0x00000000;			// QUALPRD = SYSCLKOUT for all group B GPIO
	GpioCtrlRegs.GPBQSEL1.all = 0x00000000;			// No qualification for all group B GPIO 32-47
	GpioCtrlRegs.GPBQSEL2.all = 0x00000000;			// No qualification for all group B GPIO 48-63

    GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;           // asynch input(GPIO32)
    GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;           // asynch input(GPIO33)
	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;			// 0=GPIO  1=SDAA      2=EPWMSYNCI  3=ADCSOCAO
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;			// 0=GPIO  1=SCLA      2=EPWMSYNCO  3=ADCSOCBO

	GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;			// 0=GPIO  1=ECAP1     2=XREADY     3=XREADY
    GpioCtrlRegs.GPBDIR.bit.GPIO34 =  1;            // OUTPUT(TP7)

	GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 0;			// 0=GPIO  1=SCITXDA   2=XR/W       3=XR/W
    GpioCtrlRegs.GPBDIR.bit.GPIO35 =  1;            // OUTPUT(LED-D82)

	GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 0;			// 0=GPIO  1=SCIRXDA   2=XZCS0      3=XZCS0
    GpioCtrlRegs.GPBDIR.bit.GPIO36 =  1;            // OUTPUT(LED-D81)

	GpioCtrlRegs.GPBMUX1.bit.GPIO37 = 0;			// 0=GPIO  1=ECAP2     2=XZCS7      3=XZCS7

	GpioCtrlRegs.GPBMUX1.bit.GPIO38 = 0;			// 0=GPIO  1=rsvd      2=XWE0       3=XWE0

	GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 0;			// 0=GPIO  1=rsvd      2=XA16       3=XA16

	GpioCtrlRegs.GPBMUX1.bit.GPIO40 = 0;			// 0=GPIO  1=rsvd      2=XA0/XWE1   3=XA0/XWE1

	GpioCtrlRegs.GPBMUX1.bit.GPIO41 = 0;			// 0=GPIO  1=rsvd      2=XA1        3=XA1

	GpioCtrlRegs.GPBMUX1.bit.GPIO42 = 0;			// 0=GPIO  1=rsvd      2=XA2        3=XA2

	GpioCtrlRegs.GPBMUX1.bit.GPIO43 = 0;			// 0=GPIO  1=rsvd      2=XA3        3=XA3

	GpioCtrlRegs.GPBMUX1.bit.GPIO44 = 0;			// 0=GPIO  1=rsvd      2=XA4        3=XA4

	GpioCtrlRegs.GPBMUX1.bit.GPIO45 = 0;			// 0=GPIO  1=rsvd      2=XA5        3=XA5

	GpioCtrlRegs.GPBMUX1.bit.GPIO46 = 0;			// 0=GPIO  1=rsvd      2=XA6        3=XA6

	GpioCtrlRegs.GPBMUX1.bit.GPIO47 = 0;			// 0=GPIO  1=rsvd      2=XA7        3=XA7

	GpioCtrlRegs.GPBMUX2.bit.GPIO48 = 0;			// 0=GPIO_48  1=ECAP5     2=XD31       3=XD31
	GpioCtrlRegs.GPBDIR.bit.GPIO48  = 0;	        // INPUT(INTA)

	GpioCtrlRegs.GPBMUX2.bit.GPIO49 = 0;			// 0=GPIO_49  1=ECAP6     2=XD30       3=XD30
	GpioCtrlRegs.GPBDIR.bit.GPIO49  = 0;	        // INPUT(NC)

	GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;			// 0=GPIO_50  1=EQEP1A    2=XD29       3=XD29
	GpioCtrlRegs.GPBDIR.bit.GPIO50  = 0;	        // INPUT(NC)

	GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 0;			// 0=GPIO_51  1=EQEP1B    2=XD28       3=XD28
	GpioCtrlRegs.GPBDIR.bit.GPIO51  = 0;	        // INPUT(NC)

	GpioCtrlRegs.GPBMUX2.bit.GPIO52 = 0;			// 0=GPIO_52  1=EQEP1S    2=XD27       3=XD27
	GpioCtrlRegs.GPBDIR.bit.GPIO52  = 0;	        // INPUT(NC)

	GpioCtrlRegs.GPBMUX2.bit.GPIO53 = 0;			// 0=GPIO_53  1=EQEP1I    2=XD26       3=XD26
	GpioCtrlRegs.GPBDIR.bit.GPIO53  = 0;	        // INPUT(NC)

	GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 0;			// 0=GPIO_54  1=SPISIMOA  2=XD25       3=XD25
	GpioCtrlRegs.GPBDIR.bit.GPIO54  = 0;	        // INPUT(DC_IN0)

	GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 0;			// 0=GPIO_55  1=SPISOMIA  2=XD24       3=XD24
	GpioCtrlRegs.GPBDIR.bit.GPIO55  = 0;	        // INPUT(DC_IN1)

	GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 0;			// 0=GPIO_56  1=SPICLKA   2=XD23       3=XD23
	GpioCtrlRegs.GPBDIR.bit.GPIO56  = 0;	        // INPUT(DC_IN2)

	GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0;			// 0=GPIO_57  1=SPISTEA   2=XD22       3=XD22
	GpioCtrlRegs.GPBDIR.bit.GPIO57  = 0;	        // INPUT(DC_IN3)

	GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 0;			// 0=GPIO_58  1=MCLKRA    2=XD21       3=XD21
	GpioCtrlRegs.GPBDIR.bit.GPIO58  = 0;	        // INPUT(DC_IN4)

	GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 0;			// 0=GPIO_59  1=MFSRA     2=XD20       3=XD20
	GpioCtrlRegs.GPBDIR.bit.GPIO59  = 0;	        // INPUT(DC_IN5)

	GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0;			// 0=GPIO_60  1=MCLKRB    2=XD19       3=XD19
	GpioCtrlRegs.GPBDIR.bit.GPIO60  = 0;	        // INPUT(DC_IN6)

	GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 0;			// 0=GPIO_61  1=MFSRB     2=XD18       3=XD18
	GpioCtrlRegs.GPBDIR.bit.GPIO61  = 0;	        // INPUT(DC_IN7)


	GpioCtrlRegs.GPBQSEL2.bit.GPIO62 = 3;			// Asynch input
	GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 1;			// 0=GPIO  1=SCIRXDC   2=XD17       3=XD17
	GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 1;			// 0=GPIO  1=SCITXDC   2=XD16       3=XD16

//---------------------------------------------------------------------------------------------
//--------------------------------------- Group C pins ----------------------------------------
//---------------------------------------------------------------------------------------------
	GpioCtrlRegs.GPCPUD.all = 0x00000000;			// All group C pullups enabled

	GpioCtrlRegs.GPCMUX1.bit.GPIO64 = 3;			// 0=GPIO  1=GPIO      2=XD15       3=XD15
	GpioCtrlRegs.GPCMUX1.bit.GPIO65 = 3;			// 0=GPIO  1=GPIO      2=XD14       3=XD14
	GpioCtrlRegs.GPCMUX1.bit.GPIO66 = 3;			// 0=GPIO  1=GPIO      2=XD13       3=XD13
	GpioCtrlRegs.GPCMUX1.bit.GPIO67 = 3;			// 0=GPIO  1=GPIO      2=XD12       3=XD12
	GpioCtrlRegs.GPCMUX1.bit.GPIO68 = 3;			// 0=GPIO  1=GPIO      2=XD11       3=XD11
	GpioCtrlRegs.GPCMUX1.bit.GPIO69 = 3;			// 0=GPIO  1=GPIO      2=XD10       3=XD10
	GpioCtrlRegs.GPCMUX1.bit.GPIO70 = 3;			// 0=GPIO  1=GPIO      2=XD9        3=XD9
	GpioCtrlRegs.GPCMUX1.bit.GPIO71 = 3;			// 0=GPIO  1=GPIO      2=XD8        3=XD8
	GpioCtrlRegs.GPCMUX1.bit.GPIO72 = 3;			// 0=GPIO  1=GPIO      2=XD7        3=XD7
	GpioCtrlRegs.GPCMUX1.bit.GPIO73 = 3;			// 0=GPIO  1=GPIO      2=XD6        3=XD6
	GpioCtrlRegs.GPCMUX1.bit.GPIO74 = 3;			// 0=GPIO  1=GPIO      2=XD5        3=XD5
	GpioCtrlRegs.GPCMUX1.bit.GPIO75 = 3;			// 0=GPIO  1=GPIO      2=XD4        3=XD4
	GpioCtrlRegs.GPCMUX1.bit.GPIO76 = 3;			// 0=GPIO  1=GPIO      2=XD3        3=XD3
	GpioCtrlRegs.GPCMUX1.bit.GPIO77 = 3;			// 0=GPIO  1=GPIO      2=XD2        3=XD2
	GpioCtrlRegs.GPCMUX1.bit.GPIO78 = 3;			// 0=GPIO  1=GPIO      2=XD1        3=XD1
	GpioCtrlRegs.GPCMUX1.bit.GPIO79 = 3;			// 0=GPIO  1=GPIO      2=XD0        3=XD0

	GpioCtrlRegs.GPCMUX2.bit.GPIO80 = 3;			// 0=GPIO  1=GPIO      2=XA8        3=XA8
	GpioCtrlRegs.GPCMUX2.bit.GPIO81 = 3;			// 0=GPIO  1=GPIO      2=XA9        3=XA9
	GpioCtrlRegs.GPCMUX2.bit.GPIO82 = 3;			// 0=GPIO  1=GPIO      2=XA10       3=XA10
	GpioCtrlRegs.GPCMUX2.bit.GPIO83 = 3;			// 0=GPIO  1=GPIO      2=XA11       3=XA11
	GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 3;			// 0=GPIO  1=GPIO      2=XA12       3=XA12
	GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 3;			// 0=GPIO  1=GPIO      2=XA13       3=XA13
	GpioCtrlRegs.GPCMUX2.bit.GPIO86 = 3;			// 0=GPIO  1=GPIO      2=XA14       3=XA14
	GpioCtrlRegs.GPCMUX2.bit.GPIO87 = 3;			// 0=GPIO  1=GPIO      2=XA15       3=XA15


	EDIS;						// Disable EALLOW protected register access
}

// Define Zone0 & Zone7, Zone6 is not used
void INIT_USER_XINTF(void)
{
    // Make sure the XINTF clock is enabled, Predefined in the InitPeripheralClocks Func.
   	SysCtrlRegs.PCLKCR3.bit.XINTFENCLK = 1;

    // This shows how to write to the XINTF registers.  The
    // values used here are the default state after reset.
    // Different hardware will require a different configuration.

    // For an example of an XINTF configuration used with the
    // F28335 eZdsp, refer to the examples/run_from_xintf project.

    // Any changes to XINTF timing should only be made by code
    // running outside of the XINTF.

    // All Zones---------------------------------
    // Timing for all zones based on XTIMCLK = 1/2 SYSCLKOUT
    EALLOW;
    //* XintfRegs.XINTCNF2.bit.XTIMCLK = 1;   // XTIMCLK = 1/2 SYSCLKOUT, Predefined in InitPeripheralClocks Func.

    XintfRegs.XINTCNF2.bit.WRBUFF = 0;    // No write buffering

    //* XintfRegs.XINTCNF2.bit.CLKOFF = 0;    // XCLKOUT is enabled, Predefined in InitPeripheralClocks Func.

    //* XintfRegs.XINTCNF2.bit.CLKMODE = 1;   // XCLKOUT = XTIMCLK/2, Predefined in InitPeripheralClocks Func.


    // Zone 0------------------------------------
    // When using ready, ACTIVE must be 1 or greater
    // Lead must always be 1 or greater
    // Zone write timing
    XintfRegs.XTIMING0.bit.XWRLEAD = 3;
    XintfRegs.XTIMING0.bit.XWRACTIVE = 7;
    XintfRegs.XTIMING0.bit.XWRTRAIL = 3;
    // Zone read timing
    XintfRegs.XTIMING0.bit.XRDLEAD = 3;
    XintfRegs.XTIMING0.bit.XRDACTIVE = 7;
    XintfRegs.XTIMING0.bit.XRDTRAIL = 3;

    // double all Zone read/write lead/active/trail timing
    XintfRegs.XTIMING0.bit.X2TIMING = 1;

    // Zone will be not used sample XREADY signal
    XintfRegs.XTIMING0.bit.USEREADY = 0;
    XintfRegs.XTIMING0.bit.READYMODE = 1;  // sample asynchronous

    // Size must be either:
    // 0,1 = x32 or
    // 1,1 = x16 other values are reserved
    XintfRegs.XTIMING0.bit.XSIZE = 3;

    // Zone 6------------------------------------
    // When using ready, ACTIVE must be 1 or greater
    // Lead must always be 1 or greater
    // Zone write timing
    XintfRegs.XTIMING6.bit.XWRLEAD = 3;
    XintfRegs.XTIMING6.bit.XWRACTIVE = 7;
    XintfRegs.XTIMING6.bit.XWRTRAIL = 3;
    // Zone read timing
    XintfRegs.XTIMING6.bit.XRDLEAD = 3;
    XintfRegs.XTIMING6.bit.XRDACTIVE = 7;
    XintfRegs.XTIMING6.bit.XRDTRAIL = 3;

    // double all Zone read/write lead/active/trail timing
    XintfRegs.XTIMING6.bit.X2TIMING = 1;

   // Zone will be not used sample XREADY signal
    XintfRegs.XTIMING6.bit.USEREADY = 0;
    XintfRegs.XTIMING6.bit.READYMODE = 1;  // sample asynchronous

    // Size must be either:
    // 0,1 = x32 or
    // 1,1 = x16 other values are reserved
    XintfRegs.XTIMING6.bit.XSIZE = 3;


    // Zone 7------------------------------------
    // When using ready, ACTIVE must be 1 or greater
    // Lead must always be 1 or greater
    // Zone write timing
    XintfRegs.XTIMING7.bit.XWRLEAD = 3;
    XintfRegs.XTIMING7.bit.XWRACTIVE = 7;
    XintfRegs.XTIMING7.bit.XWRTRAIL = 3;
    // Zone read timing
    XintfRegs.XTIMING7.bit.XRDLEAD = 3;
    XintfRegs.XTIMING7.bit.XRDACTIVE = 7;
    XintfRegs.XTIMING7.bit.XRDTRAIL = 3;

    // double all Zone read/write lead/active/trail timing
    XintfRegs.XTIMING7.bit.X2TIMING = 1;

   // Zone will be not used sample XREADY signal
    XintfRegs.XTIMING7.bit.USEREADY = 0;
    XintfRegs.XTIMING7.bit.READYMODE = 1;  // sample asynchronous

    // Size must be either:
    // 0,1 = x32 or
    // 1,1 = x16 other values are reserved
    XintfRegs.XTIMING7.bit.XSIZE = 3;

    // Bank switching
    // Assume Zone 7 is slow, so add additional BCYC cycles
    // when ever switching from Zone 7 to another Zone.
    // This will help avoid bus contention.
    XintfRegs.XBANK.bit.BANK = 7;
    XintfRegs.XBANK.bit.BCYC = 7;
    EDIS;
   //Force a pipeline flush to ensure that the write to
   //the last register configured occurs before returning.

    asm(" RPT #7 || NOP");

}

// Initialize Timer0
void INIT_TIMER01(void)
{
	// This function can be found in DSP2833x_CpuTimers.c
	// For this example, only initialize the Cpu Timers
	InitCpuTimers();

	// Configure CPU-Timer 0, 1, and 2 to interrupt every 1 milliseconds:
	// 150MHz CPU Freq, 50000uS(50ms)
	ConfigCpuTimer(&CpuTimer0, 150, 1000);
	ConfigCpuTimer(&CpuTimer1, 150, 100000); // 12. 10. 30

	// To ensure precise timing, use write-only instructions to write to the entire register. Therefore, if any
	// of the configuration bits are changed in ConfigCpuTimer and InitCpuTimers (in DSP2833x_CpuTimers.h), the
	// below settings must also be updated.
	CpuTimer0Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
	CpuTimer1Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0

	// Step 5. User specific code, enable interrupts:
	// Enable CPU int1 which is connected to CPU-Timer 0, CPU int13
	// which is connected to CPU-Timer 1, and CPU int 14, which is connected
	// to CPU-Timer 2:
	IER |= M_INT1;
	IER |= M_INT13;	// 12. 10. 30

	// Enable TINT0 in the PIE: Group 1 interrupt 7
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
}

// This function initializes SCI to a known state.
void INIT_SCI_UNIT(unsigned int port, unsigned int baud_rate, unsigned int stop,
              unsigned int parity,  unsigned int odd_even)
{
  	switch(port)
   	{
    case SCIDA :
	 	SciaRegs.SCICCR.all = (stop << 7) + (odd_even << 6) + (parity << 5) + 0x07;  //  (ONE/TOW)(O/E)(P)00111
		SciaRegs.SCICTL1.all =0x0003;			// enable TX, RX, RX_ERR internal SCICLK,Disable SLEEP, TXWAKE
		SciaRegs.SCICTL2.all =0x0003;			// RX interrupt enable
		SciaRegs.SCICTL2.bit.TXINTENA = 1;
		SciaRegs.SCICTL2.bit.RXBKINTENA =1;
		SciaRegs.SCIHBAUD = (baud_rate >> 8) & 0xFF;   // BRR = (LSPCLK/(BAUDRATE*8)) - 1, 9600bps(0x1E7),19200(0xF3),38400(0x79)
		SciaRegs.SCILBAUD = baud_rate & 0xFF;          // LSPCLK = SYSCLKOUT/(LOSPCP.LSPCLK*2) = 150Mhz/4 = 37.5Mhz

		SciaRegs.SCICTL1.all =0x0023;			// Relinquish SCI from Reset.SCIFFCT.all=0x0;

		SciaRegs.SCIFFTX.all=0xC020;
		SciaRegs.SCIFFRX.all=0x2021;
		SciaRegs.SCIFFCT.all=0x0;
		SciaRegs.SCIFFTX.bit.TXFIFOXRESET=1;
		SciaRegs.SCIFFRX.bit.RXFIFORESET=1;

		PieCtrlRegs.PIECTRL.bit.ENPIE = 1;		// Enable the PIE block
   		PieCtrlRegs.PIEIER9.bit.INTx1 = 1;		// Enable all SCIA RXINT interrupt
	   	IER |= 0x0100;	         			// Enable INT9 in IER to enable PIE group9
	break;

	case SCIDB :
	 	ScibRegs.SCICCR.all = (stop << 7) + (odd_even << 6) + (parity << 5) + 0x07;  //  (ONE/TOW)(O/E)(P)00111
		ScibRegs.SCICTL1.all =0x03;			// enable TX, RX, RX_ERR internal SCICLK,Disable SLEEP, TXWAKE
		ScibRegs.SCICTL2.all =0x03;			// RX interrupt enable
		ScibRegs.SCICTL2.bit.TXINTENA = 1;
		ScibRegs.SCICTL2.bit.RXBKINTENA =1;
		ScibRegs.SCIHBAUD = (baud_rate >> 8) & 0xFF;   // BRR = (LSPCLK/(BAUDRATE*8)) - 1, 9600bps(0x1E7),19200(0xF3),38400(0x79)
		ScibRegs.SCILBAUD = baud_rate & 0xFF;          // LSPCLK = SYSCLKOUT/(LOSPCP.LSPCLK*2) = 150Mhz/4 = 37.5Mhz

		ScibRegs.SCICTL1.all =0x23;			// Relinquish SCI from Reset.SCIFFCT.all=0x0;

		ScibRegs.SCIFFTX.all=0xC020;
		ScibRegs.SCIFFRX.all=0x2021;
		ScibRegs.SCIFFCT.all=0x0;
		ScibRegs.SCIFFTX.bit.TXFIFOXRESET=1;
		ScibRegs.SCIFFRX.bit.RXFIFORESET=1;

		PieCtrlRegs.PIECTRL.bit.ENPIE = 1;		// Enable the PIE block
   		PieCtrlRegs.PIEIER9.bit.INTx3 = 1;		// Enable all SCIB RXINT interrupt
	   	IER |= 0x0100;	         			    // Enable INT9 in IER to enable PIE group9

	break;

	case SCIDC :
	 	ScicRegs.SCICCR.all = (stop << 7) + (odd_even << 6) + (parity << 5) + 0x07;  //  (ONE/TOW)(O/E)(P)00111
		ScicRegs.SCICTL1.all =0x0003;			// enable TX, RX, RX_ERR internal SCICLK,Disable SLEEP, TXWAKE
		ScicRegs.SCICTL2.all =0x0003;			// RX interrupt enable
		ScicRegs.SCICTL2.bit.TXINTENA = 1;
		ScicRegs.SCICTL2.bit.RXBKINTENA =1;
		ScicRegs.SCIHBAUD = (baud_rate >> 8) & 0xFF;   // BRR = (LSPCLK/(BAUDRATE*8)) - 1, 9600bps(0x1E7),19200(0xF3),38400(0x79)
		ScicRegs.SCILBAUD = baud_rate & 0xFF;          // LSPCLK = SYSCLKOUT/(LOSPCP.LSPCLK*2) = 150Mhz/4 = 37.5Mhz

		ScicRegs.SCICTL1.all =0x0023;			// Relinquish SCI from Reset.SCIFFCT.all=0x0;

		ScicRegs.SCIFFTX.all=0xC020;
		ScicRegs.SCIFFRX.all=0x2021;
		ScicRegs.SCIFFCT.all=0x0;
		ScicRegs.SCIFFTX.bit.TXFIFOXRESET=1;
		ScicRegs.SCIFFRX.bit.RXFIFORESET=1;

		PieCtrlRegs.PIECTRL.bit.ENPIE = 1;		// Enable the PIE block
   		PieCtrlRegs.PIEIER8.bit.INTx5 = 1;		// Enable all SCIC RXINT interrupt
	   	IER |= 0x0080;	         			    // Enable INT8 in IER to enable PIE group9
	break;

	default :
	break;

   }
}

// Initialize all SCI A and SCI B
void INIT_SCI_ALL(void)
{
	INIT_SCI_UNIT(SCIDA, BPS_115200, ONE_STOP_BIT, NO_PARITY, ODD_PARITY);
	INIT_SCI_UNIT(SCIDB, BPS_9600, ONE_STOP_BIT, NO_PARITY, ODD_PARITY);
	INIT_SCI_UNIT(SCIDC, BPS_38400, ONE_STOP_BIT, NO_PARITY, ODD_PARITY);
}

// SCI PORT_A Onebyte Transmitte
void SCIA_PUTCH(char value)
{
	while(SciaRegs.SCIFFTX.bit.TXFFST != 0){}	// FIFO CHECK
	SciaRegs.SCITXBUF = value;
}

// SCI PORT_B Onebyte Transmitte
void SCIB_PUTCH(char value)
{
	while(ScibRegs.SCIFFTX.bit.TXFFST != 0){}	// FIFO CHECK
	ScibRegs.SCITXBUF = value;
}

// SCI PORT_C Onebyte Transmitte
void SCIC_PUTCH(unsigned int value)
{
	while(ScicRegs.SCIFFTX.bit.TXFFST != 0){}	// FIFO CHECK
	ScicRegs.SCITXBUF = value;
}

// SCI PORT_A multiple bytes Transmitte
void SCIA_PUT_STRING(char *string, int len)
{
	unsigned int i;

	for(i=0;i<len;i++)
	{
		SCIA_PUTCH(string[i]);
	}
}

// SCI PORT_B multiple bytes Transmitte
void SCIB_PUT_STRING(char *string, int len)
{
	unsigned int i;

	for(i=0;i<len;i++)
	{
		SCIB_PUTCH(string[i]);
	}
}

// SCI PORT_C multiple bytes Transmitte
void SCIC_PUT_STRING(char *string, int len)
{
	unsigned int i;

	for(i=0;i<len;i++)
	{
		SCIC_PUTCH(string[i]);
	}
}

// Initialize SPI
void INIT_SPI(void)
{

	// Initialize SPI FIFO registers
	SpiaRegs.SPIFFTX.all=0xE040;
	SpiaRegs.SPIFFRX.all=0x204F;
	SpiaRegs.SPIFFCT.all=0x0;

	// Initialize SPI
	SpiaRegs.SPICCR.all =0x4F;	     // Reset on, rising edge, 16-bit char bits
	SpiaRegs.SPICTL.all =0x0E;    	 // Enable master mode, one half cycle,
                                     // enable talk, and SPI int disabled.
//	SpiaRegs.SPIBRR =0x007F;
	SpiaRegs.SPIBRR =0x0F;
	SpiaRegs.SPICCR.all =0xCF;		 // Relinquish SPI from Reset
	SpiaRegs.SPIPRI.bit.FREE = 1;
}

//
unsigned int SPI_SEND_8(unsigned int value)
{
	unsigned int data;
	data = (value & 0x00FF) << 8;
	SpiaRegs.SPICCR.all =0x0007;	     // Reset on, rising edge, 8-bit char bits
	SpiaRegs.SPICCR.all =0x0087;		 // Relinquish SPI from Reset
	GpioDataRegs.GPADAT.bit.GPIO20 = 0;
	SpiaRegs.SPITXBUF = data;
	while(SpiaRegs.SPIFFRX.bit.RXFFST !=1) { }
	delay_ms(5);
	GpioDataRegs.GPADAT.bit.GPIO20 = 1;
	data = SpiaRegs.SPIRXBUF;
	return data;
}

//
unsigned int EPR_READ_STATUS_REGISTER(void)
{
	unsigned int data;

	SpiaRegs.SPICCR.all =0x000F; // Reset on, rising edge, 16-bit char bits(8-instruction, 8-read)
	SpiaRegs.SPICCR.all =0x008F; // Relinquish SPI from Reset

	data = (0x05 << 8) | 0x00; // read instruction + clock generation
	GpioDataRegs.GPADAT.bit.GPIO24 = 0;
	delay_us(1);
	SpiaRegs.SPITXBUF = data;
	while(SpiaRegs.SPIFFRX.bit.RXFFST != 1) { }
	data = SpiaRegs.SPIRXBUF;
	delay_us(1);
	GpioDataRegs.GPADAT.bit.GPIO24 = 1;

	return (data & 0xFF);
}

//
unsigned int EPR_WRITE_STATUS_REGISTER(unsigned int val)
{
	unsigned int data;

	SpiaRegs.SPICCR.all =0x000F; // Reset on, rising edge, 16-bit char bits(8-instruction, 8-val)
	SpiaRegs.SPICCR.all =0x008F; // Relinquish SPI from Reset

	data = (0x01 << 8) | (val & 0xFF); // write status register instruction + value
	GpioDataRegs.GPADAT.bit.GPIO24 = 0;
	delay_us(1);
	SpiaRegs.SPITXBUF = data;
	while(SpiaRegs.SPIFFRX.bit.RXFFST != 1) { }
	data = SpiaRegs.SPIRXBUF;
	delay_us(1);
	GpioDataRegs.GPADAT.bit.GPIO24 = 1;

	while((EPR_READ_STATUS_REGISTER() & 0x01)){};

	return data;
}

//
unsigned int EPR_SET_WRITE_ENABLE(void)
{
	unsigned int data;

	SpiaRegs.SPICCR.all =0x0007;	     // Reset on, rising edge, 8-bit char bits
	SpiaRegs.SPICCR.all =0x0087;		 // Relinquish SPI from Reset

	data = 0x06 << 8;
	GpioDataRegs.GPADAT.bit.GPIO24 = 0;
	delay_us(1);
	SpiaRegs.SPITXBUF = data;
	while(SpiaRegs.SPIFFRX.bit.RXFFST != 1) { }
	data = SpiaRegs.SPIRXBUF;
	delay_us(1);
	GpioDataRegs.GPADAT.bit.GPIO24 = 1;

	while((EPR_READ_STATUS_REGISTER() & 0x01)){};

	return data;
}

//
unsigned int EPR_READ(unsigned int addr)
{
	unsigned int data1, data2;

	SpiaRegs.SPICCR.all =0x000F;	     // Reset on, rising edge, 8-bit char bits
	SpiaRegs.SPICCR.all =0x008F;		 // Relinquish SPI from Reset

	GpioDataRegs.GPADAT.bit.GPIO24 = 0;
	delay_us(1);

	data1 = (0x03 << 8) | ((addr & 0x0700) >> 8); // read instruction + msbyte of addr
	data2 = ((addr & 0x00FF) << 8) | 0x00; // lsbyte of addr + clock generation

	SpiaRegs.SPITXBUF = data1;
	while(SpiaRegs.SPIFFRX.bit.RXFFST != 1) { }
	data1 = SpiaRegs.SPIRXBUF;

	SpiaRegs.SPITXBUF = data2;
	while(SpiaRegs.SPIFFRX.bit.RXFFST != 1) { }
	data1 = SpiaRegs.SPIRXBUF;

	delay_us(1);
	GpioDataRegs.GPADAT.bit.GPIO24 = 1;

	return (data1 & 0xFF);
}

//
unsigned int EPR_WRITE(unsigned int addr, unsigned int val)
{
	unsigned int data1, data2;

	EPR_SET_WRITE_ENABLE();

	SpiaRegs.SPICCR.all =0x000F;	     // Reset on, rising edge, 8-bit char bits
	SpiaRegs.SPICCR.all =0x008F;		 // Relinquish SPI from Reset

	GpioDataRegs.GPADAT.bit.GPIO24 = 0;
	delay_us(1);

	data1 = (0x02 << 8) | ((addr & 0x0700) >> 8); // write instruction + msbyte of addr
	data2 = ((addr & 0x00FF) << 8) | (val & 0xFF); // lsbyte of addr + value

	SpiaRegs.SPITXBUF = data1;
	while(SpiaRegs.SPIFFRX.bit.RXFFST != 1) { }
	data1 = SpiaRegs.SPIRXBUF;

	SpiaRegs.SPITXBUF = data2;
	while(SpiaRegs.SPIFFRX.bit.RXFFST != 1) { }
	data1 = SpiaRegs.SPIRXBUF;

	delay_us(1);
	GpioDataRegs.GPADAT.bit.GPIO24 = 1;
//	data1 = SpiaRegs.SPIRXBUF;

	while((EPR_READ_STATUS_REGISTER() & 0x01)){};

	return data1;
}

//
unsigned int SPI_SEND_16(unsigned int value)
{
	unsigned int data;
//	SpiaRegs.SPICCR.all =0x004F;	     // Reset on, rising edge, 16-bit char bits
	SpiaRegs.SPICCR.all =0x00CF;		 // Relinquish SPI from Reset
	delay_us(1);
	GpioDataRegs.GPADAT.bit.GPIO19 = 0;
	SpiaRegs.SPITXBUF = value;
	while(SpiaRegs.SPIFFRX.bit.RXFFST !=1) { }
	delay_us(5);
	GpioDataRegs.GPADAT.bit.GPIO19 = 1;
	data = SpiaRegs.SPIRXBUF;
	return data;
}


//
unsigned int SPI_VOL_SEND_16(unsigned int value)
{
	unsigned int data;
//	SpiaRegs.SPICCR.all =0x004F;	     // Reset on, rising edge, 16-bit char bits

	SpiaRegs.SPICCR.all =0x00CF;		 // Relinquish SPI from Reset
	delay_us(1);
	GpioDataRegs.GPADAT.bit.GPIO19 = 0;
	SpiaRegs.SPITXBUF = value;
	while(SpiaRegs.SPIFFRX.bit.RXFFST !=1) { }
	delay_us(5);
	GpioDataRegs.GPADAT.bit.GPIO19 = 1;
	data = SpiaRegs.SPIRXBUF;

	return data;
}

// Enable Interrupt
void ENABLE_INT(void)
{
	// Enables the PIE module and CPU interrupts and global Interrupt INTM
	// refer to DSP2833x_PieCtrl.c file
	EnableInterrupts();

	// Enable Global realtime interrupt DBGM
	ERTM;
}

// This function initializes ADC to a known state.
void INIT_ADC(void)
{
    extern void DSP28x_usDelay(Uint32 Count);


    // *IMPORTANT*
	// The ADC_cal function, which  copies the ADC calibration values from TI reserved
	// OTP into the ADCREFSEL and ADCOFFTRIM registers, occurs automatically in the
	// Boot ROM. If the boot ROM code is bypassed during the debug process, the
	// following function MUST be called for the ADC to function according
	// to specification. The clocks to the ADC MUST be enabled before calling this
	// function.
	// See the device data manual and/or the ADC Reference
	// Manual for more information.

 	EALLOW;
    SysCtrlRegs.HISPCP.all = 3;	// HSPCLK = SYSCLKOUT/HISCP*2, (HISCP = 0(SYSCLKOUT)~7(SYSCLKOUT/14))
	                            // SYSCLKOUT : 150Mhz, (150/6 = 25Mhz)
	SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;
	ADC_cal();
	EDIS;

    // To powerup the ADC the ADCENCLK bit should be set first to enable
    // clocks, followed by powering up the bandgap, reference circuitry, and ADC core.
    // Before the first conversion is performed a 5ms delay must be observed
	// after power up to give all analog circuits time to power up and settle

    // Please note that for the delay function below to operate correctly the
	// CPU_RATE define statement in the DSP2833x_Examples.h file must
	// contain the correct CPU clock period in nanoseconds.

    AdcRegs.ADCREFSEL.bit.REF_SEL = 1; // 0=internal, 1=2.048V, 2= 1.5V, 3= 1.024V
    AdcRegs.ADCTRL3.all = 0x00E0;      // Power up bandgap/reference/ADC circuits. Sequential Mode
    DELAY_US(ADC_usDELAY);             // Delay before converting ADC channels

   	// Set ADC core clock prescaler
	// ADCCLK = HSPCLK/((ADCTRL3.ADCCLKPS * 2)*(ADCTRL1.bit.CPS + 1))
	// 12.5MHz = 25MHz/(1 * 2)
	// 735 KHZ conversion rate
   	AdcRegs.ADCTRL1.bit.CPS = 0;        // ADC module clock = HSPCLK/2*ADC_CKPS   = 25.0MHz/(1*2) = 12.5MHz
	AdcRegs.ADCTRL3.bit.ADCCLKPS = 1;
    AdcRegs.ADCTRL1.bit.ACQ_PS = 0xF;  // Sequential mode: Sample rate   = 1/[(2+ACQ_PS)*ADC clock in ns]
                                       //  = 1/(17*80ns) = 735KHz (for 150 MHz SYSCLKOUT)
					   			       // If Simultaneous mode enabled: Sample rate = 1/[(3+ACQ_PS)*ADC clock in ns]

    AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;  // 1=Cascaded mode , 0 = Dual Senquencer Mode
    AdcRegs.ADCTRL1.bit.CONT_RUN = 0;  //0=Start-Stop Mode, 1=Continuous Mode

    AdcRegs.ADCMAXCONV.all = 0xF;      ///SEQ Operation 16ch

    AdcRegs.ADCCHSELSEQ1.all = 0x3210;
	AdcRegs.ADCCHSELSEQ2.all = 0x7654;
	AdcRegs.ADCCHSELSEQ3.all = 0xBA98;
	AdcRegs.ADCCHSELSEQ4.all = 0xFEDC;
}

// Initialize I2C
void INIT_I2C(void)
{
	I2caRegs.I2CMDR.all = 0x0000;	      // Take I2C  reset
	delay_ms(1);                        // it must need reset(BB)
	I2caRegs.I2CPSC.all = 14;           // Prescaler - need 7-12 Mhz on module clk (150Mhz/(14+1)) = 10MHz)
	I2caRegs.I2CCLKL = 20;			  // NOTE: must be non zero(200Khz)
	I2caRegs.I2CCLKH = 20;			  // NOTE: must be non zero(200Khz)
	I2caRegs.I2COAR = 0x01;             // I2C OWN Address
	I2caRegs.I2CIER.all = 0x00;		  // Disable all I2C Interrupt
	I2caRegs.I2CFFTX.all = 0x6000;      // Enable FIFO mode and TXFIF
	I2caRegs.I2CFFRX.all = 0x2000;	  // Enable RXFIFO, clear RXFFINT,
	I2caRegs.I2CMDR.all = 0x0020;	      // Take I2C out of reset	    			                      // Stop I2C when suspen
}

// Write one data to eeprom(Random Acess)
void EEPROM_WRITEDATA(unsigned int slave_address,unsigned int eeprom_address, unsigned int data)
{
	while((I2caRegs.I2CMDR.bit.STP == 1) || (I2caRegs.I2CSTR.bit.BB == 1));  //stop & busy check
	I2caRegs.I2CSAR = slave_address;                  //Setup Slave address(0X0000~0X1FFF)
	I2caRegs.I2CCNT = 3;                              // ADD(H),ADD(L) & DATA
	I2caRegs.I2CDXR = (eeprom_address >> 8) & 0x00FF; //Address High Write
	I2caRegs.I2CDXR = eeprom_address & 0x00FF;        //Address Low  Write
	I2caRegs.I2CDXR = data; //data                    //Data Write
	I2caRegs.I2CMDR.all = 0x6E20;                      //SEND Slave Module
	delay_ms(5);                                        //eeprom write time
}

// Read one data from eeprom(Random Acess)
unsigned int EEPROM_READDATA(unsigned int slave_address,unsigned int eeprom_address)
{
	unsigned int data;
	delay_us(2);                                      //bus free time
 	while((I2caRegs.I2CMDR.bit.STP == 1) || (I2caRegs.I2CSTR.bit.BB == 1));  //stop & busy check
	I2caRegs.I2CSAR = slave_address;                  //Setup Slave address
	I2caRegs.I2CCNT = 2;                              // ADD(H),ADD(L)
	I2caRegs.I2CDXR = (eeprom_address >> 8) & 0x00FF; // Write Address High
	I2caRegs.I2CDXR = eeprom_address & 0x00FF;        // Write Address Low
	I2caRegs.I2CMDR.all = 0x6620;			            // Send Address to EEPROM(no stop bit)

	while(I2caRegs.I2CSTR.bit.ARDY != 1);             // Register-Acess-ready
	I2caRegs.I2CCNT = 1;                              // read one byte data
	I2caRegs.I2CMDR.all = 0x2C20;                     // Send restart as master receiver(stop bit)

	while(I2caRegs.I2CMDR.bit.STP == 1);
	data = I2caRegs.I2CDRR;
	return data;
}

// Initialize PWM
void INIT_EPWM5A(void)
{
	// ePWM5 register configuration with HRPWM
	// ePWM5A toggle low/high with MEP control on Rising edge

	EPwm5Regs.TBCTL.bit.PRDLD = 0x01;	                 // set Immediate load
	EPwm5Regs.TBPRD = 10;            		             // PWM frequency = 1 / period
	EPwm5Regs.CMPA.half.CMPA = 5;	                     // set duty 50% initially
    EPwm5Regs.CMPA.half.CMPAHR = (1 << 8);               // initialize HRPWM extension
	EPwm5Regs.TBPHS.all = 0;
	EPwm5Regs.TBCTR = 0;

	EPwm5Regs.TBCTL.bit.CTRMODE = 0;
	EPwm5Regs.TBCTL.bit.PHSEN = 0;		                 // ePWM5 is the Master
	EPwm5Regs.TBCTL.bit.SYNCOSEL = 3;
	EPwm5Regs.TBCTL.bit.HSPCLKDIV = 7;                   //TBCLK=150Mhz(128*14) = 83705.35Hz(41852.67Hz)
	EPwm5Regs.TBCTL.bit.CLKDIV = 7;

	EPwm5Regs.CMPCTL.bit.LOADAMODE = 0;
	EPwm5Regs.CMPCTL.bit.LOADBMODE = 0;
	EPwm5Regs.CMPCTL.bit.SHDWAMODE = 0;
	EPwm5Regs.CMPCTL.bit.SHDWBMODE = 0;

	EPwm5Regs.AQCTLA.bit.ZRO = 2;                      // PWM out High("NOT" in the CPLD)
	EPwm5Regs.AQCTLA.bit.CAU = 2;

	EALLOW;
	EPwm5Regs.HRCNFG.all = 0x0;
	EPwm5Regs.HRCNFG.bit.EDGMODE = 1;                //MEP control on Rising edge
	EPwm5Regs.HRCNFG.bit.CTLMODE = 0;
	EPwm5Regs.HRCNFG.bit.HRLOAD  = 0;
	EDIS;

    EPwm2Regs.CMPA.half.CMPAHR = 0;

    delay_ms(10);

    EALLOW;
  	//First GPIO8 is Input
   	GpioCtrlRegs.GPAMUX1.bit.GPIO8  = 1;			 // 0=GPIO_8  1=EPWM5A     2=CANTXB     3=ADCSOCAO
    EDIS;
}

// Output PWM
void PWM_OUT(unsigned int fre, unsigned int ctr) //fre(1~xxxx), (fre=0 & ctr=EN is always "1"), (ctr=DI is always "0")
{
	unsigned int period;

	if(ctr == EN)
	{
		if(fre == 0) //set output is always "1"
		{
			EPwm5Regs.AQCTLA.bit.ZRO = 1;                          // PWM LOW(CPLD HIGH)
			EPwm5Regs.AQCTLA.bit.CAU = 1;
		}
		else
     	{
			period = (unsigned int)(83705.35/fre);
			EPwm5Regs.TBPRD = period-1;		                       // PWM frequency = 1 / period
			EPwm5Regs.CMPA.half.CMPA = (unsigned int)(period / 2); // set duty 50% initially
			EPwm5Regs.AQCTLA.bit.ZRO = 1;                          // PWM toggle low/high
			EPwm5Regs.AQCTLA.bit.CAU = 2;
		}
	}
	else
 	{
		EPwm5Regs.AQCTLA.bit.ZRO = 2;                             // PWM  HIGH(CPLD LOW)
		EPwm5Regs.AQCTLA.bit.CAU = 2;
	}
}

// Read all of ADC Channel
void READ_ADC_ALL(unsigned int *ADCH) //ADCH[1]~ADCH[16], ADCH[0] is not used
{
	AdcRegs.ADCMAXCONV.all = 0xF;             //SEQ Operation 16ch
	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0;      // Other Channel was defined previously in the Init_Adc()
	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         //Reset SEQ1
	AdcRegs.ADCTRL2.all = 0x2000;             //Start SEQ1
	while (AdcRegs.ADCST.bit.INT_SEQ1== 0) {} // Wait for interrupt
	AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
	ADCH[0]  = 0;
	ADCH[1]  = AdcRegs.ADCRESULT0  >> 4;
 	ADCH[2]  = AdcRegs.ADCRESULT1  >> 4;
	ADCH[3]  = AdcRegs.ADCRESULT2  >> 4;
	ADCH[4]  = AdcRegs.ADCRESULT3  >> 4;
	ADCH[5]  = AdcRegs.ADCRESULT4  >> 4;
	ADCH[6]  = AdcRegs.ADCRESULT5  >> 4;
	ADCH[7]  = AdcRegs.ADCRESULT6  >> 4;
  	ADCH[8]  = AdcRegs.ADCRESULT7  >> 4;
	ADCH[9]  = AdcRegs.ADCRESULT8  >> 4;
	ADCH[10] = AdcRegs.ADCRESULT9  >> 4;
	ADCH[11] = AdcRegs.ADCRESULT10 >> 4;
	ADCH[12] = AdcRegs.ADCRESULT11 >> 4;
	ADCH[13] = AdcRegs.ADCRESULT12 >> 4;
	ADCH[14] = AdcRegs.ADCRESULT13 >> 4;
	ADCH[15] = AdcRegs.ADCRESULT14 >> 4;
	ADCH[16] = AdcRegs.ADCRESULT15 >> 4;
}

// Read One of ADC channel
unsigned int READ_ADC_ONE(unsigned int ch)    // ch value must be from 1 to 16
{
	AdcRegs.ADCMAXCONV.all = 0x0;               // SEQ Operation 1 ch
	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;           // Reset SEQ1
	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = (ch - 1); // Set ADC input one channel selection sequence(from 1 to 16)
	AdcRegs.ADCTRL2.all = 0x2000; //Start SEQ1
	while (AdcRegs.ADCST.bit.INT_SEQ1 == 0) {}  // Wait for interrupt
	AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
	return (AdcRegs.ADCRESULT0  >> 4);
}

// Interrupt SCI A Receive Data
// ADMIN Process
interrupt void SCIA_GETCH_ISR(void)
{
	unsigned int status,length;
	char RcvChar;
	static unsigned int rxa_cnt = 0, SciaStep = 0, free_rxa_cnt=0;

	status = SciaRegs.SCIRXST.all & 0xFF;

	if((status & 0x9C) != 0)
	{
		SciaRegs.SCICTL1.bit.SWRESET=0;			// reset
		SciaRegs.SCICTL1.bit.SWRESET=1;			// non reset
	}
	else
	{
		length = (SciaRegs.SCIFFRX.all >> 8) & 0x1F;
//		for(i=0; i<length; i++)
//		{RXABUFFER[i] = SciaRegs.SCIRXBUF.all;}

		// PROTOCOL
		// RobotFreeMotion: {0xB7, 0xB8, 0xA1, 0x17, 0x60, LeftVel_H, LeftVel_L, RightVel_H, RightVel_L, 0xFA}
		// Where: 0xB7, 0xB8 --> Start  Byte ; 0xA1 --> DSP receiving ; 0x17 --> ID of Nova, 0x60 --> Free Direction, 0xFA --> End Byte
		// S111222E : 111 - M1 Speed(0~100%), 222 - M2 Speed(0~100%)
		// A111222E : 111 - M1 Acc(ms), 222 - M2 Acc(ms)
		// Mxxxxx0E :
		//        0 - STOP, 1 - FORWARD, 2 - LEFT TURN, 3 - RIGHT TURN, 4 - BACKWARD
		// GxxxxxxE : Gyro Data Request

		RcvChar = SciaRegs.SCIRXBUF.all;

		//free motion processing
		if(RcvChar == 0xb7){

			SCIA_PUTCH('A');
			free_rxa_cnt=0;
			flagDebug = 5;
			FreeMotionInterrupt = 1;
		}

		FreeMotion[free_rxa_cnt] = RcvChar;
		free_rxa_cnt++;

		if(free_rxa_cnt > 10)
			free_rxa_cnt = 0;

		switch(SciaStep)
		{
			case 0:
				if((RcvChar == 'A') || (RcvChar == 'M') || (RcvChar == 'S') || (RcvChar == 'G')){
					RXABUFFER[rxa_cnt] = RcvChar;
					rxa_cnt++;
					SciaStep++;
				}
			break;

			case 1:
				RXABUFFER[rxa_cnt] = RcvChar;
				rxa_cnt++;
				if(rxa_cnt == 7) SciaStep++;
			break;

			case 2:
				if(RcvChar == 'E'){
//					if(RXABUFFER[0] == 'W') FlagWComplete = TRUE;
					RXABUFFER[rxa_cnt] = RcvChar;
					RcvComplete = TRUE;
					rxa_cnt = 0;
					SciaStep = 0;
				}else SciaStep = 0;
			break;
		}
	}

	SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1; 			// Clear Overflow flag
	SciaRegs.SCIFFRX.bit.RXFFINTCLR=1; 			// Clear Interrupt flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;		// Must acknowledge the PIE group
}

// Interrupt SCI B Receive Data
interrupt void SCIB_GETCH_ISR(void)  //Communicate with sub board
{
/*	char rcv;
	unsigned int status, length, i;
	static unsigned char src_step = 0, mcu_step = 0, sen_step = 0, mcu_cnt = 0;
	static unsigned int rx_cnt = 0, start, finish, sen_count = 0;
	static unsigned int rx_step = 0;

	status = ScibRegs.SCIRXST.all & 0xFF;
	if((status & 0x9C) != 0)
 	{
		ScibRegs.SCICTL1.bit.SWRESET=0;			// reset
		ScibRegs.SCICTL1.bit.SWRESET=1;			// non reset
	}
	else
	{
		length = (ScibRegs.SCIFFRX.all >> 8) & 0x1F;

		rcv = ScibRegs.SCIRXBUF.all;

		if(rcv == '0' || rcv == '1' || rcv == '2' || rcv == '3' || rcv == '4')
		{
			RXBBUFFER[0] = rcv;

		}

				delay_ms(100);
				GpioDataRegs.GPADAT.bit.GPIO4 = 0;
				delay_ms(500);
				GpioDataRegs.GPADAT.bit.GPIO4 = 1;
				Timer_Monitor = 0;

//				GYRO_PROCESS();
		//QRFlag++;
	}

	ScibRegs.SCIFFRX.bit.RXFFOVRCLR=1;  			// Clear Overflow flag
	ScibRegs.SCIFFRX.bit.RXFFINTCLR=1; 			// Clear Interrupt flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;		// Must acknowledge the PIE group*/

}

// Interrupt SCI C Receive Data
interrupt void SCIC_GETCH_ISR(void)
{
	unsigned int status, length,i;
	status = ScicRegs.SCIRXST.all & 0xFF;
	if((status & 0x9C) != 0)
	{
		ScicRegs.SCICTL1.bit.SWRESET=0;			// resetScicRegs.SCICTL1.bit.SWRESET=1;			// non reset
  	}
 	else
  	{
		length = (ScicRegs.SCIFFRX.all >> 8) & 0x1F;
		for(i=0; i<length; i++)
		{RXCBUFFER[i] = ScicRegs.SCIRXBUF.all;}
 	}
	ScicRegs.SCIFFRX.bit.RXFFOVRCLR=1; 	 		// Clear Overflow flag
	ScicRegs.SCIFFRX.bit.RXFFINTCLR=1; 			// Clear Interrupt flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;		// Must acknowledge the PIE group
}

//**************************************
// NiCC Control Functions
//**************************************

// Convert String to Interger
int STR_TO_INT(char *str, int start, int finish)
{
	int st, i, loop, num, digit = 10, result = 0;

	// 양수, 음수 판별
	if(str[start] == '-') st = start + 1;
	else st = start;

	// 최대 자리수 결정
	loop = finish - st - 1;
	if(loop == -1) digit = 1;
	else if(loop == 0) digit = 10;
	else{
		for(i=0;i<loop;i++){
			digit = digit * 10;
		}
	}

	// 문자열을 자리수마다 정수로 변환
	loop = finish + 1;
	for(i=st;i<loop;i++){
		num = str[i] - '0';
		result = (num * digit) + result;
		digit = digit / 10;
	}

	// 양수, 음수 처리
	if(str[start] == '-') result = result * -1;

	return result;
}

void FIR_LPF_Coef(float fs, float fc, float *h)
{
	int n, Q;
	float mu1 = fc * 2.0 / fs, coef[31];

	Q = (N - 1) / 2;

	coef[0] = mu1;

	for(n=1;n<=Q;n++){
		coef[n] = (sin((float)n * M_PI * mu1) / (M_PI * (float)n)) * (0.54 + 0.46 * cos((float)n * M_PI / (float)Q));
	}
	for(n=0;n<(int)Q;n++) h[n] = coef[(int)Q-n];
	for(n=(int)Q;n<N;n++) h[n] = coef[n-(int)Q];
}

void RX_STACK1(int num, int dat, int *buf)
{
	static int i = 0;
	int j;

	if(i < num){
		buf[i] = dat;
		i++;
	}else{
		for(j=0;j<(num-1);j++) buf[j] = buf[j+1];
		buf[num-1] = dat;
	}
}

void RX_STACK2(int num, int dat, int *buf)
{
	static int i = 0;
	int j;

	if(i < num){
		buf[i] = dat;
		i++;
	}else{
		for(j=0;j<(num-1);j++) buf[j] = buf[j+1];
		buf[num-1] = dat;
	}
}

float Convolute1(int num, int *dat, float *coe)
{
	int i;
	float tmp = 0;

	for(i=0;i<num;i++){
//		tmp += Coe[i] * dat[((j-i+MAX) % MAX)];
		tmp += coe[i] * dat[i];
	}

	return tmp;
}

float Convolute2(int num, int *dat, float *coe)
{
	int i;
	float tmp = 0;

	for(i=0;i<num;i++){
//		tmp += Coe[i] * dat[((j-i+MAX) % MAX)];
		tmp += coe[i] * dat[i];
	}

	return tmp;
}

float STDE_EW(int num, float dat) // Standard Deviation
{
//	static int buf[100], i=0;
/*	static float Test_buf[500];
	static int i=0;
	int j;
	float sum = 0, avg, temp, sigma;

	if(i < num){
		Test_buf[i] = dat;
		i++;
		sigma = 0;
	}else{
		for(j=0;j<(num-1);j++){
			Test_buf[j] = Test_buf[j+1];
			sum += Test_buf[j];
		}
		Test_buf[num-1] = dat;
		sum += Test_buf[j];

		avg = sum / (float)num;

		sum = 0;
		for(j=0;j<num;j++){
			temp = Test_buf[j];
			temp = (temp - avg) * (temp - avg);
			sum += (double)temp;
		}
		sigma = sqrt(sum / (double)num);
	}

	return sigma;*/
	static unsigned int i = 0, j = 0;
	static float dat_sum = 0, sqrt_sum = 0, data_buf[500], sqrt_buf[500];
	float mean, mean_sq, sigma;

	if(i < num){
		dat_sum += dat;
		data_buf[i] = dat;
//		D_buf[i] = data_buf[i]; //

		sqrt_sum += dat * dat;
		sqrt_buf[i] = dat * dat;
//		S_buf[i] = sqrt_buf[i]; //

		i++;

		sigma = 0;
	}else{
		dat_sum = dat_sum - data_buf[j] + dat;
		data_buf[j] = dat;
//		D_buf[j] = data_buf[j]; //

		sqrt_sum = sqrt_sum - sqrt_buf[j] + (dat * dat);
		sqrt_buf[j] = dat * dat;
//		S_buf[j] = sqrt_buf[j]; //

		mean = dat_sum / num;
//		MEan = mean; //
		mean_sq = mean * mean;

		j++;
		if(j == num) j = 0;

		sigma = sqrt((sqrt_sum / (float)num) - mean_sq);
	}

	return sigma;
}

float STDE_NS(int num, float dat) // Standard Deviation
{
	static unsigned int i = 0, j = 0;
	static float dat_sum = 0, sqrt_sum = 0, data_buf[500], sqrt_buf[500];
	float mean, mean_sq, sigma;

	if(i < num){
		dat_sum += dat;
		data_buf[i] = dat;

		sqrt_sum += dat * dat;
		sqrt_buf[i] = dat * dat;

		i++;

		sigma = 0;
	}else{
		dat_sum = dat_sum - data_buf[j] + dat;
		data_buf[j] = dat;

		sqrt_sum = sqrt_sum - sqrt_buf[j] + (dat * dat);
		sqrt_buf[j] = dat * dat;

		mean = dat_sum / num;
		mean_sq = mean * mean;

		j++;
		if(j == num) j = 0;

		sigma = sqrt((sqrt_sum / (float)num) - mean_sq);
	}

	return sigma;
}

float EW_AV_STDE(int num, float dat)
{
	float avg, temp, sigma;
	static float sum = 0, sq = 0;

	if(num != 0){
		sum += dat;

		avg = sum / (float)num;

		temp = dat * dat;
		sq += temp;

		sigma = sqrt((sq / (float)num) - (avg * avg));
	}else{
		sum = 0;
		sq = 0;
		sigma = 0;
	}

	return sigma;
}

float NS_AV_STDE(int num, float dat)
{
	float avg, temp, sigma;
	static float sum = 0, sq = 0;

	if(num != 0){
		sum += dat;

		avg = sum / (float)num;

		temp = dat * dat;
		sq += temp;

		sigma = sqrt((sq / (float)num) - (avg * avg));
	}else{
		sum = 0;
		sq = 0;
		sigma = 0;
	}

	return sigma;
}

//THIS FUNCTION IS USING SPI TO CONTROL MOTOR
unsigned int MOTOR_SPEED(unsigned int uiM1, unsigned int uiM2)
{
	unsigned int data,uiM1Data, uiM2Data,value,delvel_ul=0;

	//Delta velocity from actual survey to balance 2 motors: left and right
	delvel_ul = 4;
	uiM1Data = uiM1;//min = 0 | max = 3000 (rpm)
	uiM2Data = uiM2;// + delvel_ul;//min = 0 | max = 3000 - dV (rpm)

	SpiaRegs.SPICCR.all =0x00CF;		 // Relinquish SPI from Reset
	delay_us(1);
	GpioDataRegs.GPADAT.bit.GPIO19 = 0; //DISABLE CHIP SELECT PIN
	value = 0x5000 | (uiM1Data & 0x0FFF);// Value of velocity is transfered directly to motor via SPI protocol with max=4095
	SpiaRegs.SPITXBUF = value;  //SPITXBUF레지스터에 스피트값을 넣어 다음에 전송하고 그값을 지운다.

	while(SpiaRegs.SPIFFRX.bit.RXFFST !=1) { }  // 다음 전송을 위한 대기

	delay_us(5);
	GpioDataRegs.GPADAT.bit.GPIO19 = 1; //ENABLE CHIP SELECT PIN
	data = SpiaRegs.SPIRXBUF; //수신된 데이터 값을 data 변수에 넣


	SpiaRegs.SPICCR.all =0x00CF;		 // Relinquish SPI from Reset
	delay_us(1);
	GpioDataRegs.GPADAT.bit.GPIO19 = 0;
	value = 0xC000 | (uiM2Data & 0x0FFF);// Value of velocity is transfered directly to motor via SPI protocol with max=4095
	SpiaRegs.SPITXBUF = value;

	while(SpiaRegs.SPIFFRX.bit.RXFFST !=1) { }

	delay_us(5);
	GpioDataRegs.GPADAT.bit.GPIO19 = 1;
	data = SpiaRegs.SPIRXBUF;

	return data;
}

//
unsigned long SPI_SEND_24(unsigned long value)
{
	unsigned long data1, data2, rx1, rx2;

	GpioDataRegs.GPADAT.bit.GPIO21 = 0;
	SpiaRegs.SPICCR.all =0x0007;	     // Reset on, rising edge, 8-bit char bits
	SpiaRegs.SPICCR.all =0x0087;		 // Relinquish SPI from Reset
	data1 = (value & 0xFF0000) >> 8;
	SpiaRegs.SPITXBUF = data1;
	while(SpiaRegs.SPIFFRX.bit.RXFFST !=1) { }
	rx1 = SpiaRegs.SPIRXBUF;
//	SpiaRegs.SPICCR.all =0x000F;	     // Reset on, rising edge, 16-bit char bits
	SpiaRegs.SPICCR.all =0x008F;		 // Relinquish SPI from Reset
	data2 = (value & 0x00FFFF);
	SpiaRegs.SPITXBUF = data2;
	while(SpiaRegs.SPIFFRX.bit.RXFFST !=1) { }
	delay_us(5);
	GpioDataRegs.GPADAT.bit.GPIO21 = 1;
	rx2 = SpiaRegs.SPIRXBUF;
	return ((rx1 << 16) | rx2);
}

unsigned long SPI_CUR_SEND_EW(unsigned long value)
{
	unsigned long data1, data2;

	SpiaRegs.SPICCR.all =0x0007;	     // Reset on, rising edge, 8-bit char bits
	SpiaRegs.SPICCR.all =0x0087;		 // Relinquish SPI from Reset

	GpioDataRegs.GPADAT.bit.GPIO21 = 0;

	data1 = (value & 0xFF0000) >> 8;
	SpiaRegs.SPITXBUF = data1;
	while(SpiaRegs.SPIFFRX.bit.RXFFST !=1) { }
	data1 = SpiaRegs.SPIRXBUF;

//	SpiaRegs.SPICCR.all =0x000F;	     // Reset on, rising edge, 16-bit char bits
	SpiaRegs.SPICCR.all =0x008F;		 // Relinquish SPI from Reset

	data2 = (value & 0x00FFFF);
	SpiaRegs.SPITXBUF = data2;
	while(SpiaRegs.SPIFFRX.bit.RXFFST !=1) { }
	data2 = SpiaRegs.SPIRXBUF;

	delay_us(1);
	GpioDataRegs.GPADAT.bit.GPIO21 = 1;

	return ((data1 << 16) | data2);
}

unsigned long SPI_CUR_SEND_NS(unsigned long value)
{
	unsigned long data1, data2;

	SpiaRegs.SPICCR.all =0x0007;	     // Reset on, rising edge, 8-bit char bits
	SpiaRegs.SPICCR.all =0x0087;		 // Relinquish SPI from Reset

	GpioDataRegs.GPADAT.bit.GPIO20 = 0;

	data1 = (value & 0xFF0000) >> 8;
	SpiaRegs.SPITXBUF = data1;
	while(SpiaRegs.SPIFFRX.bit.RXFFST !=1) { }
	data1 = SpiaRegs.SPIRXBUF;

//	SpiaRegs.SPICCR.all =0x000F;	     // Reset on, rising edge, 16-bit char bits
	SpiaRegs.SPICCR.all =0x008F;		 // Relinquish SPI from Reset

	data2 = (value & 0x00FFFF);
	SpiaRegs.SPITXBUF = data2;
	while(SpiaRegs.SPIFFRX.bit.RXFFST !=1) { }
	data2 = SpiaRegs.SPIRXBUF;

	delay_us(1);
	GpioDataRegs.GPADAT.bit.GPIO20 = 1;

	return ((data1 << 16) | data2);
}

/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Initialize System
void INIT_SYSTEM(void)
{
	InitSysCtrl();      // Disable WatchDog, Init PLL and PeripheralClocksu
	INIT_USER_GPIO();   // GPIO USER Board Setting
	DINT;               // Disable CPU interrupts
	InitPieCtrl();      // Initialize the PIE control registers to their default state.
	IER = 0x0000;       // Disable CPU interrupts
	IFR = 0x0000;       // Clear all CPU interrupt flags
	InitPieVectTable(); // Initialize the PIE vector table with pointers to the shell
	                  	//Interrupt Service Routines (ISR).

	// Interrupts that are used in this example are re-mapped to
	// ISR functions found within this file.
	EALLOW;  // This is needed to write to EALLOW protected registers

	//  PieVectTable.TINT0 = &TIMER0_SW_ISR;
	PieVectTable.TINT0 = &TIMER0_SW_ISR;
	PieVectTable.XINT13 = &TIMER1_SW_ISR; // 12. 10. 30
	PieVectTable.SCIRXINTA = &SCIA_GETCH_ISR;
	PieVectTable.SCIRXINTB = &SCIB_GETCH_ISR;
	PieVectTable.SCIRXINTC = &SCIC_GETCH_ISR;
	EDIS;    // This is needed to disable write to EALLOW protected registers

	INIT_USER_XINTF(); // define XINTF Timing

	// FLASH Code Copy and Init Routine--------------------------------------------
	// Copy time critical code and Flash setup code to RAM
	// The  RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart
	// symbols are created by the linker. Refer to the FLASH.cmd file.
	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

	// Call Flash Initialization to setup flash waitstates
	// This function must reside in RAM
	InitFlash();

	INIT_TIMER01();  //150Mhz, 50000us(50ms)
	INIT_SCI_ALL(); //SCI_A/B/C Initalize
	INIT_SPI();
	INIT_ADC();     // ADC Initialize & channel Select
	INIT_I2C(); 	  //I2C Initalize
}

