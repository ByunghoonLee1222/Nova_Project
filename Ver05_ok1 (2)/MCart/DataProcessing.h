#define VERSION_NO                    1
#define RELEASE_NO                    0

#define NUMBER_OF_DATA                145

#define ON                            0
#define OFF                           1

#define EAST                          1
#define WEST                          2
#define NORTH                         1
#define SOUTH                         2
#define UP                            1
#define DOWN                          2
#define STOP                          0

#define STANDBY                       0
#define POSITIVE_ERR_INC              1
#define POSITIVE_ERR_DEC              2
#define NEGATIVE_ERR_INC              3
#define NEGATIVE_ERR_DEC              4
#define DECEL                         5
#define REST                          6

#define T_ACCEL_2SPEED                1
#define T_CONST_ACCEL_2SPEED          2
#define T_DECEL_2SPEED                3
#define T_CONST_DECEL_2SPEED          4
#define T_CONST_COMPLETE              5
#define T_DECEL_BEFORE_COMPLETE       6

#define UD_START_DELAY                1
#define UD_ACCEL                      2
#define UD_CONST                      3
#define UD_DECEL                      4

#define CART_READY					0
#define CART_ACC					1
#define CART_DEC					2
#define CART_STOP					3
#define CART_CCW_READY              4
#define CART_CCW_START              5
#define CART_CCW_END                6
#define CART_CW_READY               7
#define CART_CW_START               8
#define CART_CW_END                 9
#define CART_CCW_TYPE               10
#define CART_CW_TYPE                11


#define ANGLE_ROT                   90

#define PARAM_KP                0.5
#define PARAM_KD                0.008
#define PARAM_KI                10.0

#define GYRO_SLAVE_ADDRESS			0x20


#define SENSOR_FORK_LIMIT_OUT	GpioDataRegs.GPBDAT.bit.GPIO61
#define SENSOR_FORK_LIMIT_IN	GpioDataRegs.GPBDAT.bit.GPIO60
#define SENSOR_LIFT_MIDDLE_ONE  GpioDataRegs.GPBDAT.bit.GPIO57
#define SENSOR_LIFT_LOW_ONE     GpioDataRegs.GPBDAT.bit.GPIO59
interrupt void TIMER0_SW_ISR(void);
interrupt void TIMER1_SW_ISR(void);
int anglePos(int angle);
int PidSpeed_ctrl(int _fYaw);

#define SENSOR_FORK_LIMIT_OUT   GpioDataRegs.GPBDAT.bit.GPIO61
#define SENSOR_FORK_LIMIT_IN   GpioDataRegs.GPBDAT.bit.GPIO60
#define SENSOR_LIFT_HIGH_TWO    GpioDataRegs.GPBDAT.bit.GPIO54
#define SENSOR_LIFT_HIGH_ONE    GpioDataRegs.GPBDAT.bit.GPIO55
#define SENSOR_LIFT_MIDDLE_ONE  GpioDataRegs.GPBDAT.bit.GPIO57
#define SENSOR_LIFT_MIDDLE_TWO  GpioDataRegs.GPBDAT.bit.GPIO56
#define SENSOR_LIFT_LOW_ONE     GpioDataRegs.GPBDAT.bit.GPIO59
#define SENSOR_LIFT_LOW_TWO     GpioDataRegs.GPBDAT.bit.GPIO58
