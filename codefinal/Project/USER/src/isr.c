#include "headfile.h"
#include "isr.h"
int second,millisecond = 0;

void CSI_IRQHandler(void)
{
    CSI_DriverIRQHandler();     //调用SDK自带的中断函数 这个函数最后会调用我们设置的回调函数
    __DSB();                    //数据同步隔离
}

void USART4_Callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
	
}

void PIT_IRQHandler(void)
{
	int16 data1[4];
	float data2[1];
	if(PIT_FLAG_GET(PIT_CH3))
    {
		Encoder_read();
		get_icm20602_gyro_spi();
		Yaw += (icm_gyro_z)*0.061*0.005;
//		XYWvelocity2WheelVelocity();
//		motor_control();
//		motor_run();
        PIT_FLAG_CLEAR(PIT_CH3);
    }
	
    if(PIT_FLAG_GET(PIT_CH0))
    {	
		char* str = "000.000000000\r\n";
		millisecond += 1;
		second = millisecond /1000 ;

		str[0] = second/100 + '0';					//s
		str[1] = (second/10)%10 + '0';
		str[2] = second%10 + '0';	

		str[4] = millisecond%1000/100 + '0';		//ms
		str[5] = (millisecond%1000/10)%10 + '0';
		str[6] = millisecond%1000%10 + '0';	

		str[7] = real_x/10 + '0';					//X
		str[8] = real_x%10 + '0';
		str[9] = real_y/10 + '0';					//Y
		str[10] = real_y%10 + '0';	
		
		str[11] = '0';	
		str[12] = '0';	
		if(locate_state != UartWait)
		{
			if(millisecond % 100 ==0)	
				bluetooth_ch9141_send_buff((uint8 *)str,strlen(str));		
		}

		 PIT_FLAG_CLEAR(PIT_CH0);
    }
	
    if(PIT_FLAG_GET(PIT_CH1))
    {
		get_icm20602_gyro_spi();
		Yaw += (icm_gyro_z)*0.061*0.001;
//		Yaw_PIDUpdate();
//		XYWvelocity2WheelVelocity();
//		Encoder_read();
//		motor_control();
//		motor_run();
		
        PIT_FLAG_CLEAR(PIT_CH1);
    }
	
	if(PIT_FLAG_GET(PIT_CH2))
    {
//		get_icm20602_gyro_spi();
//		Yaw += (icm_gyro_z)*0.061*0.005;
		Encoder_read();
//		data1[0] = current_x;
//		data1[1] = current_y;
//		data1[2] = target_x;
//		data1[3] = target_y;
//		data1[1] = Car.distance_target;
//		debug_send(data1,4);
//		data2[0] = Yaw;
//		debug_send_gyro(data2,1);
		switch(locate_state)
		{
			case DegreeControl:
					if(Yaw > Car.Yaw_Set - 0.2 && Yaw < Car.Yaw_Set + 0.2)
					{
						Car.WVelocitySet = 0;
						Motor[0].Encoder_Integral = 0;
						Motor[1].Encoder_Integral = 0;
						Motor[2].Encoder_Integral = 0;
						Motor[3].Encoder_Integral = 0;
						Car.distance = 0;
						locate_state = DegreeDelay;
						delaytime = 6;
						goto MotorRun;
					}
					Yaw_PIDUpdate();
					break;
			case DegreeDelay:
					delaytime--;
//					GyroW_Control();
					Yaw_PIDUpdate();
					if(delaytime == 0)
					{
						locate_state = DistanceControl;
					}
					break;
			case DistanceControl:
					if(RunMode == 0)
					{
//						Car.YVelocitySet = TrapezoidalVelocityDistribution(40,80,0.03,0.11,Car.distance_target[0],Car.distance);
						if(fabs(Car.distance - Car.distance_target[0]) <= 200)
						{
							ad_value1 = adc_convert(POWER_ADC1_MOD,POWER_ADC1_PIN);
							if(ad_value1!=255||fabs(Car.distance - Car.distance_target[0]) <= 10)
							{
								Car.YVelocitySet = 0.0f;
								locate_state = DistanceDelay;
								delaytime = 1;
								goto MotorRun;							
							}														
						}
					}
					else if(RunMode == 1)
					{
						Car.YVelocitySet = 50.0f;//Fuzzy_Speed(Car.distance_target[0] - Car.distance);
						if(fabs(Car.distance - Car.distance_target[0]) <= 10)
						{
							Car.YVelocitySet = 0.0f;
							locate_state = DistanceDelay;
							delaytime = 1;
							goto MotorRun;	
						}					
					}

					GyroW_Control();
					break;
			case DistanceControl2:
						Correct_velocity();
						if(abs(Car.distance_target[2]) > abs(Car.distance_target[3]))
						{
							if(abs(Car.Xdistance - Car.distance_target[0]) <= 10 )
							{
								NowPathPoint++;
								NowCntPathPointSUP++;
								if(NowCntPathPointSUP < CntPathPointSUP)
								{
	//								Car.distance_target[0] = PathPointSEED[NowPathPoint][0];
	//								Car.distance_target[1] = PathPointSEED[NowPathPoint][1];	
//									Correct_velocity4();
//									Distance_Clear();
									Correct_velocity();
	//								Distance_Clear();
								}
								if(NowCntPathPointSUP == CntPathPointSUP)
								{
									Car.YVelocitySet = 0.0f;
									Car.XVelocitySet = 0.0f;
									locate_state = DistanceDelay;
									delaytime = 30;
									goto MotorRun;								
								}
							
							}
						}
						else
						{
							if(abs(Car.distance - Car.distance_target[1]) <= 10 )
							{
								NowPathPoint++;
								NowCntPathPointSUP++;
								if(NowCntPathPointSUP < CntPathPointSUP)
								{
	//								Car.distance_target[0] = PathPointSEED[NowPathPoint][0];
	//								Car.distance_target[1] = PathPointSEED[NowPathPoint][1];
//									Correct_velocity4();
//									Distance_Clear();
									Correct_velocity();
	//								Distance_Clear();
								}
								if(NowCntPathPointSUP == CntPathPointSUP)
								{
									Car.YVelocitySet = 0.0f;
									Car.XVelocitySet = 0.0f;
									locate_state = DistanceDelay;
									delaytime = 30;
									goto MotorRun;								
								}							
							}
						}						
					GyroW_Control();
			
					break;
			case DistanceDelay:
					delaytime--;
					GyroW_Control();
//					Yaw_PIDUpdate();
					if(delaytime == 0)
					{
						if(RunMode == 0)
						{						
							if(finish_flag == 1)
							{
								Car.Yaw_Set = 0.0f;	
								locate_state = UpdateTarget;
								break;
							}
							locate_state = ImageCorrect;

						}
						else if(RunMode == 1)
						{
							locate_state = PutDown1;
						}
					}
						break;
			case ImageCorrect:
//					GyroW_Control();
					if(mt9v03x_finish_flag)
					{	
						mt9v03x_finish_flag = 0;
						Bin_Image2();
						Above_Detect();	
					}
					Yaw_PIDUpdate();
					ImageXY_PIDUpdate();
					if(abs(Above_Centre[1] - 94 )<2&& abs(Above_Centre[0] - 74 )<2 && abs(Motor[0].Encoder) <=6&& abs(Motor[1].Encoder) <=6&& abs(Motor[2].Encoder) <=6&&abs(Motor[3].Encoder) <=6)
					{
						Car.XVelocitySet = 0;
						Car.YVelocitySet = 0;
						locate_state = ImageCorrectDelay;
						delaytime = 10;
						gpio_set(BEEP_PIN,1);
						goto MotorRun;
					}					
					
					break;
			case ImageCorrectDelay:
					delaytime--;
//					GyroW_Control();
					Yaw_PIDUpdate();
					if(delaytime == 0)
					{
						gpio_set(BEEP_PIN,0);
						Car.WVelocitySet = 0;
						if(abs(Yaw - 270) < 10) //270 -> -90
							Yaw -= 360;
						if(abs(Yaw + 180) < 10) //-180 -> 180
							Yaw += 360;						
						locate_state = UartStart;
					}				
					break;
			case UartStart:
					uart_putchar(USART_5,'1');
					gpio_set(B13,1);
					locate_state = UartWait;
					break;
			case UartWait:
					break;	
			case PutOn1:
					gpio_set(Electromagnet1,1);
					pwm_init(S_MOTOR1_PIN,50,1.45*50000/20);
					delaytime = 80;
					locate_state = PutOnDelay1;
					break;
			case PutOnDelay1:
					delaytime--;
					if(delaytime == 0)
					{
						locate_state = PutOn2;
					}
					break;
			case PutOn2:
					pwm_init(S_MOTOR1_PIN,50,1.65*50000/20);		
					delaytime = 10;
//					delaytime = 200;
					locate_state = PutOnDelay2;
					break;
			case PutOnDelay2:
					delaytime--;
					if(delaytime == 0)
					{
						locate_state = CarryUpdate;
					}
					break;	
			case CarryUpdate:
					RunMode = 1;
					Distance_Clear();
					PIDError_Clear();
					CarryDIS_DEG();
					if(MoveMode == 1)
						locate_state = DegreeControl;
					else if(MoveMode == 2)
					{
						locate_state = DistanceControl2;					
						Distance_Clear();
						Correct_velocity();
//						Correct_velocity4();
					
					}

					RunMode = 1;
					break;
			case PutDown1 :
//					pwm_init(S_MOTOR1_PIN,50,0.9*50000/20);
					gpio_set(Electromagnet1,0);
					delaytime = 5;
					locate_state = PutDownDelay1;
					GyroW_Control();
//					Yaw_PIDUpdate();
					break;
			case PutDownDelay1:
					delaytime--;
					if(delaytime == 0)
					{
						pwm_init(S_MOTOR1_PIN,50,2.4*50000/20);
						locate_state = UpdateTarget;
					}	
					GyroW_Control();
//					Yaw_PIDUpdate();
					break;
			case PutDown2:
					pwm_init(S_MOTOR1_PIN,50,2.4*50000/20);
					delaytime = 1;
					locate_state = PutDownDelay2;
					Yaw_PIDUpdate();
					break;
			case PutDownDelay2:
					delaytime--;
					if(delaytime == 0)
					{					
						locate_state = UpdateTarget;
					}
					Yaw_PIDUpdate();
					break;	
			case UpdateTarget:
					coordinate2DIS_DEGE();
					if(finish_flag == 2)
					{
						Car.YVelocitySet = 0.0f;
						Car.XVelocitySet = 0.0f;
						real_x = 1;
						real_y = 1;
						locate_state = FinishState;
						goto MotorRun;
					}
					else
					{	
						if(MoveMode == 1)
							locate_state = DegreeControl;
						else if(MoveMode == 2)
						{
								locate_state = DistanceControl2;
//								Correct_velocity3();
								Distance_Clear();
//								Correct_velocity4();
								Correct_velocity();
						}


					}
					break;
			case FinishState:
					Car.YVelocitySet = 0.0f;
					Car.XVelocitySet = 0.0f;
					Car.WVelocitySet = 0.0f;
					GyroW_Control();
					break;
		
		}
		MotorRun: 
					XYWvelocity2WheelVelocity();
					motor_control();
					motor_run();
					goto Finish;
		Stop:
					MotorStop(1,1,1,1);
					motor_run();
		
		Finish:
		PIT_FLAG_CLEAR(PIT_CH2);
    }
	


    __DSB();
}


void GPIO2_Combined_16_31_IRQHandler(void)
{
    if(GET_GPIO_FLAG(C16))
    {
        CLEAR_GPIO_FLAG(C16);//清除中断标志位
    }
    
    
}



void GPIO2_Combined_0_15_IRQHandler(void)
{
    if(GET_GPIO_FLAG(MT9V03X_VSYNC_PIN))
    {
        //不用清除标志位，标志位在mt9v03x_vsync函数内部会清除
        if(CAMERA_GRAYSCALE == flexio_camera_type)mt9v03x_vsync();
    }
    if(GET_GPIO_FLAG(SCC8660_VSYNC_PIN))
    {
        //不用清除标志位，标志位在scc8660_vsync函数内部会清除
        if(CAMERA_COLOR == flexio_camera_type)scc8660_vsync();
    }
}



/*
GPIO3_Combined_0_15_IRQHandler
此中断函数默认被SD卡检测插入功能占用，如果需要gpio中断建议使用其他IO
或者不使用SD的卡可以将fsl_sdmmc_host.c中的 SDMMCHOST_CARD_DETECT_GPIO_INTERRUPT_HANDLER函数注释掉即可

*/





/*
中断函数名称，用于设置对应功能的中断函数
Sample usage:当前启用了周期定时器中断
void PIT_IRQHandler(void)
{
    //务必清除标志位
    __DSB();
}
记得进入中断后清除标志位
CTI0_ERROR_IRQHandler
CTI1_ERROR_IRQHandler
CORE_IRQHandler
FLEXRAM_IRQHandler
KPP_IRQHandler
TSC_DIG_IRQHandler
GPR_IRQ_IRQHandler
LCDIF_IRQHandler
CSI_IRQHandler
PXP_IRQHandler
WDOG2_IRQHandler
SNVS_HP_WRAPPER_IRQHandler
SNVS_HP_WRAPPER_TZ_IRQHandler
SNVS_LP_WRAPPER_IRQHandler
CSU_IRQHandler
DCP_IRQHandler
DCP_VMI_IRQHandler
Reserved68_IRQHandler
TRNG_IRQHandler
SJC_IRQHandler
BEE_IRQHandler
PMU_EVENT_IRQHandler
Reserved78_IRQHandler
TEMP_LOW_HIGH_IRQHandler
TEMP_PANIC_IRQHandler
USB_PHY1_IRQHandler
USB_PHY2_IRQHandler
ADC1_IRQHandler
ADC2_IRQHandler
DCDC_IRQHandler
Reserved86_IRQHandler
Reserved87_IRQHandler
GPIO1_INT0_IRQHandler
GPIO1_INT1_IRQHandler
GPIO1_INT2_IRQHandler
GPIO1_INT3_IRQHandler
GPIO1_INT4_IRQHandler
GPIO1_INT5_IRQHandler
GPIO1_INT6_IRQHandler
GPIO1_INT7_IRQHandler
GPIO1_Combined_0_15_IRQHandler
GPIO1_Combined_16_31_IRQHandler
GPIO2_Combined_0_15_IRQHandler
GPIO2_Combined_16_31_IRQHandler
GPIO3_Combined_0_15_IRQHandler
GPIO3_Combined_16_31_IRQHandler
GPIO4_Combined_0_15_IRQHandler
GPIO4_Combined_16_31_IRQHandler
GPIO5_Combined_0_15_IRQHandler
GPIO5_Combined_16_31_IRQHandler
WDOG1_IRQHandler
RTWDOG_IRQHandler
EWM_IRQHandler
CCM_1_IRQHandler
CCM_2_IRQHandler
GPC_IRQHandler
SRC_IRQHandler
Reserved115_IRQHandler
GPT1_IRQHandler
GPT2_IRQHandler
PWM1_0_IRQHandler
PWM1_1_IRQHandler
PWM1_2_IRQHandler
PWM1_3_IRQHandler
PWM1_FAULT_IRQHandler
SEMC_IRQHandler
USB_OTG2_IRQHandler
USB_OTG1_IRQHandler
XBAR1_IRQ_0_1_IRQHandler
XBAR1_IRQ_2_3_IRQHandler
ADC_ETC_IRQ0_IRQHandler
ADC_ETC_IRQ1_IRQHandler
ADC_ETC_IRQ2_IRQHandler
ADC_ETC_ERROR_IRQ_IRQHandler
PIT_IRQHandler
ACMP1_IRQHandler
ACMP2_IRQHandler
ACMP3_IRQHandler
ACMP4_IRQHandler
Reserved143_IRQHandler
Reserved144_IRQHandler
ENC1_IRQHandler
ENC2_IRQHandler
ENC3_IRQHandler
ENC4_IRQHandler
TMR1_IRQHandler
TMR2_IRQHandler
TMR3_IRQHandler
TMR4_IRQHandler
PWM2_0_IRQHandler
PWM2_1_IRQHandler
PWM2_2_IRQHandler
PWM2_3_IRQHandler
PWM2_FAULT_IRQHandler
PWM3_0_IRQHandler
PWM3_1_IRQHandler
PWM3_2_IRQHandler
PWM3_3_IRQHandler
PWM3_FAULT_IRQHandler
PWM4_0_IRQHandler
PWM4_1_IRQHandler
PWM4_2_IRQHandler
PWM4_3_IRQHandler
PWM4_FAULT_IRQHandler
Reserved171_IRQHandler
GPIO6_7_8_9_IRQHandler*/



