#ifndef _headfile_h
#define _headfile_h

#include <stdint.h>
#include "fsl_common.h"

#include "fsl_debug_console.h"
#include "fsl_iomuxc.h"
#include "fsl_pit.h"

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_cache.h"
#include "common.h"
#include "zf_vector.h"

//------文件系统相关头文件
#include "ff.h"
#include "diskio.h"
#include "fsl_sd_disk.h"

#include "SEEKFREE_PRINTF.h"

//------逐飞科技单片机外设驱动头文件
#include "zf_gpio.h"
#include "zf_iomuxc.h"
#include "zf_pit.h"
#include "zf_pwm.h"
#include "zf_uart.h"
#include "zf_spi.h"
#include "zf_systick.h"
#include "zf_qtimer.h"
#include "zf_adc.h"
#include "zf_iic.h"
#include "zf_flash.h"
#include "zf_camera.h"
#include "zf_csi.h"
#include "zf_rom_api.h"
#include "zf_usb_cdc.h"
#include "zf_sdcard.h"

//------逐飞科技产品驱动头文件
#include "SEEKFREE_FONT.h"
#include "SEEKFREE_18TFT.h"
#include "SEEKFREE_IPS114_SPI.h"
#include "SEEKFREE_IPS200_PARALLEL8.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_VIRSCO.h"
#include "SEEKFREE_FUN.h"
#include "SEEKFREE_MPU6050.h"
#include "SEEKFREE_OLED.h"
#include "SEEKFREE_NRF24L01.h"
#include "SEEKFREE_MMA8451.h"
#include "SEEKFREE_L3G4200D.h"
#include "SEEKFREE_ICM20602.h"
#include "SEEKFREE_MT9V03X.h"
#include "SEEKFREE_WIRELESS.h"
#include "SEEKFREE_MT9V03X_CSI.h"
#include "SEEKFREE_W25QXXJV.h"
#include "SEEKFREE_SCC8660_CSI.h"
#include "SEEKFREE_SCC8660.h"

/*------	my header files	------*/
#include "stdlib.h"
#include "adc.h"
#include "motor.h"
#include "control.h"
#include "imu.h"
#include "my_math.h"
#include "imageprocess.h"
#include "my_uart.h"
#include "key.h"
#include "map.h"
#include "path.h"
#include "bluetooth.h"
#include "Fuzzy_D.h"
#include "init.h"
#include "servo.h"
#include "beep.h"
#include "electromagnet.h"


/*
*	variable
*/
extern MotorDef Motor[4];
extern CarDef Car;
extern ImageCorrectDef ImagePID;
extern uint8 circle_count;
extern uint8 finish_flag;
extern uint8 start_flag;
extern uint8 start;
extern float Intergral1,Intergral2,Wintergral;
extern uint8 PeakSpeed;


/*
*	Locate
*/
extern uint8 coordinate_list[20][2]; 
extern int baseVelocity;
extern float current_x,current_y;
extern float next_x,next_y;
extern enum locate_step locate_state;
extern int16 delaytime;
extern uint8 real_x,real_y;
extern float target_x,target_y;
extern uint8 MoveMode;
extern uint8 ReturnMode;

/*
*	Gyro
*/
extern int16 icm_gyro_x,icm_gyro_y,icm_gyro_z;
extern int16 icm_acc_x,icm_acc_y,icm_acc_z;
extern int16 vx,vy,vz;
extern float Gyro_x,Gyro_y,Gyro_z;
extern float Acc_x,Acc_y,Acc_z;
extern float Pitch,Yaw,Roll;
extern float Yaw_zero;
extern uint8 gyro_check;


/*
*	UART
*/
extern uint8 example_rx_buffer;
extern lpuart_transfer_t   example_receivexfer;
extern lpuart_handle_t     example_g_lpuartHandle;
extern uint8 send_buff[7];
extern uint8 *uart_data;
extern uint8 uart_send;
extern int16  RevArray[3];


/*
*	Key
*/
extern uint8 key1_status;
extern uint8 key2_status;
extern uint8 key1_last_status;
extern uint8 key2_last_status;
extern uint8 key1_flag;
extern uint8 key2_flag;


/*
*	ImageProcess
*/
extern uint8 Threshold;
extern uint8 Bin_Image[120][188]; 
extern uint8 Above_Left[2];
extern uint8 Above_Right[2];
extern uint8 Above_TurnPoint[2];
extern uint8 Above_TurnPresence[2];
extern uint8 Above_Point[188][2];
extern uint8 above_cnt;
extern uint8 Above_Centre[2];
extern int16 WhiteCnt;
extern int16 BottomWhiteCount;
extern uint8 Bottom_Point[188][2];
extern uint8 Bottom_Angular[2];
extern uint8 bottom_cnt;
extern uint8 PhototThreshold;
extern uint8 photoflag;
extern uint8 PositionColumn;


/*
*	map
*/
extern uint8 BlackThres;
extern uint8 image[120][188]; 
extern uint8 flagimage[120][188];
extern uint8 TopLeft[2];
extern uint8 BottomRight[2];
extern uint8 circlecount;
extern uint8 CircleCenter[20][2];
extern uint8 map[20][2]; 
extern uint8 line_start;
extern uint8 line_end;
extern uint8 line_threshold;
extern uint8 ConnectDomainIndex;
extern uint8 CDBdotCnt;
extern uint8 offset4[4][2];
extern uint16 Rectangledot;


/*
*	Path
*/
extern uint8 PointNum;
extern int16 DistanceMatrix[20][20];
extern uint8 Path0[20];
extern uint8 PointFlag[12];


/*
*	Time
*/   
extern int second;
extern int millisecond;


/*
*	Carry
*/
extern uint8 MainClass;
extern uint8 CarryMapIndex[3];
extern float target_Cx,target_Cy;
extern uint8 RunMode;
extern uint8 SkipYellow;


/*
*	ADC
*/
extern uint16  ad_value1;


/*
*	Route
*/
extern float PathPointSUP[10][2];
extern float PathPointSEED[500][2];
extern int16 NowPathPoint;
extern uint8 CntPathPointSUP;
extern int16 CntPathPointSEED;
extern uint8 NowCntPathPointSUP;
extern uint8 NowCntMapPoint;

#endif

