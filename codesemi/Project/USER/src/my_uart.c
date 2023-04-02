#include "my_uart.h"
uint8 example_rx_buffer;
lpuart_transfer_t   example_receivexfer;
lpuart_handle_t     example_g_lpuartHandle;

uint8 RevBuff[20];
uint8 send_buff[7] = {0xfe,0x01,0,0,0,0x01,0xfe};
uint8 *uart_data;
uint8 uart_send;
int16  RevArray[3];
int8 Uart_temp;

void example_uart_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
    if(kStatus_LPUART_RxIdle == status)
    {
		static uint8 RevCount = 0;
		
//		lcd_showuint8(0,0,example_rx_buffer);
//		lcd_showuint8(0,1,RevCount);
		RevBuff[RevCount] = example_rx_buffer;
		if(RevCount == 0)
		{
			if(example_rx_buffer == FRAME_HEADER1)
			{
				RevCount++;		
				goto RevFinish;				
			}
			else
			{
				goto RevFinish;
			}
			
		}
		if(RevCount == 1)
		{
			if(example_rx_buffer == FRAME_HEADER2)
			{
				RevCount++;
				goto RevFinish;
			}	
			else
			{
				RevCount = 0;
				goto RevFinish;
			}
		}
		if(RevBuff[RevCount] == FRAME_TAIL2 && RevBuff[RevCount-1] == FRAME_TAIL1)
		{
			RevCount = 0;
			goto UartHandle;
		}
		RevCount++;
		goto RevFinish;
    }
	
	UartHandle:
		Uart_temp = RevBuff[2] & 0xff;
		RevArray[0] = Uart_temp;
		Uart_temp = RevBuff[3] & 0xff;
		RevArray[1] = Uart_temp;
		MainClass = RevArray[0];
		Uart_temp = RevBuff[4] & 0xff;
		RevArray[2] = Uart_temp;
		if(RevArray[2] == 1)
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
			
			str[11] = RevArray[0]%10 + '0';
			str[12] = RevArray[1]%10 + '0';
			bluetooth_ch9141_send_buff((uint8 *)str,strlen(str));
			locate_state = PutOn1;
			gpio_set(B13,0);
		}
//		Locate();
//		debug_send(RevArray,3);
	
//		lcd_showint8(0,3,RevBuff[2]);
//		lcd_showint8(0,4,RevBuff[3]);
//		lcd_showint8(0,5,RevBuff[4]);
		
	RevFinish:    
		handle->rxDataSize = example_receivexfer.dataSize;  
		handle->rxData = example_receivexfer.data;          
}


void Uart_Init(void)
{

    uart_init(USART_5, 115200,UART5_TX_C28,UART5_RX_C29);	
    NVIC_SetPriority(LPUART8_IRQn,15);         
    uart_rx_irq(USART_5,1);
    
    //配置串口接收的缓冲区及缓冲区长度
    example_receivexfer.dataSize = 1;
    example_receivexfer.data = &example_rx_buffer;
    
    //设置中断函数及其参数
    uart_set_handle(USART_5, &example_g_lpuartHandle, example_uart_callback, NULL, 0, example_receivexfer.data, 1);

}


