#include "bluetooth.h"

uint8               bluetooth_ch9141_rx_buffer;
lpuart_transfer_t   bluetooth_ch9141_receivexfer;
lpuart_handle_t     bluetooth_ch9141_g_lpuartHandle;

static  fifo_struct     bluetooth_ch9141_fifo;
static 	uint8           bluetooth_ch9141_buffer[BLUETOOTH_CH9141_BUFFER_SIZE];  // ���ݴ������


//-------------------------------------------------------------------------------------------------------------------
// @brief       FIFO ��ʼ�� ���ض�Ӧ������
// @param       *fifo           FIFO ����ָ��
// @param       *buffer_addr    Ҫ���صĻ�����
// @param       size            ��������С
// @return      fifo_state_enum ����״̬
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
fifo_state_enum fifo_init (fifo_struct *fifo, uint8 *buffer_addr, uint32 size)
{
    if(buffer_addr == NULL)
        return FIFO_BUFFER_NULL;
    fifo->buffer    = buffer_addr;
    fifo->head      = 0;
    fifo->end       = 0;
    fifo->size      = size;
    fifo->max       = size;
    return FIFO_SUCCESS;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       FIFO ͷָ��λ��
// @param       *fifo           FIFO ����ָ��
// @param       offset          ƫ����
// @return      void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void fifo_head_offset (fifo_struct *fifo, uint32 offset)
{
    fifo->head += offset;

    while(fifo->max <= fifo->head)                                              // �����Χ���������������С ֱ��С����󻺳�����С
    {
        fifo->head -= fifo->max;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       FIFO βָ��λ��
// @param       *fifo           FIFO ����ָ��
// @param       offset          ƫ����
// @return      void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void fifo_end_offset (fifo_struct *fifo, uint32 offset)
{
    fifo->end += offset;

    while(fifo->max <= fifo->end)                                               // �����Χ���������������С ֱ��С����󻺳�����С
    {
        fifo->end -= fifo->max;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       FIFO ���û�����
// @param       *fifo           FIFO ����ָ��
// @return      void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void fifo_clear (fifo_struct *fifo)
{
    fifo->head      = 0;
    fifo->end       = 0;
    fifo->size      = fifo->max;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       FIFO ��ѯ��ǰ���ݸ���
// @param       *fifo           FIFO ����ָ��
// @return      void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint32 fifo_used (fifo_struct *fifo)
{
    return (fifo->max - fifo->size);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       �� FIFO ��д������
// @param       *fifo           FIFO ����ָ��
// @param       *dat            ������Դ������ָ��
// @param       length          ��Ҫд������ݳ���
// @return      fifo_state_enum ����״̬
// Sample usage:                if(fifo_write_buffer(&fifo,data,32)!=FIFO_SUCCESS) while(1);
//-------------------------------------------------------------------------------------------------------------------
fifo_state_enum fifo_write_buffer (fifo_struct *fifo, uint8 *dat, uint32 length)
{
    uint32 temp_length;
	uint8 isr;
    if(length < fifo->size)                                                     // ʣ��ռ��㹻װ�±�������
    {
		isr=DisableGlobalIRQ();
        temp_length = fifo->max - fifo->head;                                   // ����ͷָ����뻺����β���ж��ٿռ�

        if(length > temp_length)                                                // ���뻺����β���Ȳ���д������ ���λ������ֶβ���
        {
            memcpy(&fifo->buffer[fifo->head], dat, temp_length);                // ������һ������
            fifo_head_offset(fifo, temp_length);                                // ͷָ��ƫ��
            dat += temp_length;                                                 // ��ȡ����ƫ��
            memcpy(&fifo->buffer[fifo->head], dat, length - temp_length);       // ������һ������
            fifo_head_offset(fifo, length - temp_length);                       // ͷָ��ƫ��
        }
        else
        {
            memcpy(&fifo->buffer[fifo->head], dat, length);                     // һ������д��
            fifo_head_offset(fifo, length);                                     // ͷָ��ƫ��
        }

        fifo->size -= length;                                                   // ������ʣ�೤�ȼ�С
		EnableGlobalIRQ(isr);
    }
    else
    {
        return FIFO_SPACE_NO_ENOUGH;
    }
	
    return FIFO_SUCCESS;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       �� FIFO ��ȡ����
// @param       *fifo           FIFO ����ָ��
// @param       *dat            Ŀ�껺����ָ��
// @param       *length         ��ȡ�����ݳ��� ���û����ô����������ᱻ�޸�
// @param       flag            �Ƿ��� FIFO ״̬ ��ѡ���Ƿ���ն�ȡ������
// @return      fifo_state_enum ����״̬
// Sample usage:                if(fifo_read_buffer(&fifo,data,32,FIFO_READ_ONLY)!=FIFO_SUCCESS) while(1);
//-------------------------------------------------------------------------------------------------------------------
fifo_state_enum fifo_read_buffer (fifo_struct *fifo, uint8 *dat, uint32 *length, fifo_operation_enum flag)
{
    uint8 data_check = 0;
    uint32 temp_length;
	uint8 isr;
    if(*length > fifo_used(fifo))
    {
        *length = (fifo->max - fifo->size);                                     // ������ȡ�ĳ���
        data_check = 1;                                                         // ��־���ݲ���
    }

    temp_length = fifo->max - fifo->end;                                        // ����βָ����뻺����β���ж��ٿռ�
    if(*length <= temp_length)                                                  // �㹻һ���Զ�ȡ���
    {
        if(NULL != dat)    memcpy(dat, &fifo->buffer[fifo->end], *length);      // һ���Զ�ȡ���
    }
    else
    {
        if(NULL != dat)
        {
            memcpy(dat, &fifo->buffer[fifo->end], temp_length);                 // ������һ������
            memcpy(&dat[temp_length], &fifo->buffer[0], *length - temp_length); // �����ڶ�������
        }
    }

    if(flag == FIFO_READ_AND_CLEAN)                                             // ���ѡ���ȡ������ FIFO ״̬
    {
		isr=DisableGlobalIRQ();
        fifo_end_offset(fifo, *length);                                         // �ƶ� FIFO ͷָ��
        fifo->size += *length;
		EnableGlobalIRQ(isr);
    }

    return (data_check?FIFO_DATA_NO_ENOUGH:FIFO_SUCCESS);
}

uint8 bluetooth_ch9141_rx_buffer_flag=0;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�� �����жϻص�����
//  @param      void
//  @return     void
//  Sample usage:
//  @note       �ú�����ISR�ļ� �����жϳ��򱻵���
//-------------------------------------------------------------------------------------------------------------------
void bluetooth_ch9141_uart_callback (LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
    // ��ȡ���ߴ��ڵ����� ������λ���ձ�־
	handle->rxDataSize = bluetooth_ch9141_receivexfer.dataSize;  //��ԭ����������
    handle->rxData = bluetooth_ch9141_receivexfer.data;          //��ԭ��������ַ
    fifo_write_buffer(&bluetooth_ch9141_fifo, &bluetooth_ch9141_rx_buffer, 1);       // ���� FIFO
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ���ʼ��
//  @param      mode            ����ģʽ MASTER_MODE(����)����SLAVE_MODE(�ӻ�)
// @return      uint8           ��ʼ��״̬ 0-�ɹ� 1-ʧ��
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8 bluetooth_ch9141_init (void)
{
    // ������ʹ�õĲ�����Ϊ115200 Ϊ����ת����ģ���Ĭ�ϲ����� ����������������ʹ����λ���޸�ģ�����
	    //���ô��ڽ��յĻ�����������������
    bluetooth_ch9141_receivexfer.dataSize = 1;
    bluetooth_ch9141_receivexfer.data = &bluetooth_ch9141_rx_buffer;
    uart_set_handle(BLUETOOTH_CH9141_INDEX, &bluetooth_ch9141_g_lpuartHandle, bluetooth_ch9141_uart_callback, NULL, 0, bluetooth_ch9141_receivexfer.data, 1);
	
    fifo_init(&bluetooth_ch9141_fifo, bluetooth_ch9141_buffer, BLUETOOTH_CH9141_BUFFER_SIZE);
    gpio_init(BLUETOOTH_CH9141_RTS_PIN, GPI, 1, GPIO_INT_CONFIG);                   // ��ʼ����������
    uart_init(BLUETOOTH_CH9141_INDEX, BLUETOOTH_CH9141_BUAD_RATE, BLUETOOTH_CH9141_RX_PIN, BLUETOOTH_CH9141_TX_PIN);
	 uart_rx_irq(BLUETOOTH_CH9141_INDEX, 1);
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�� ���ͺ���
//  @param      buff            ��Ҫ���͵����ݵ�ַ
//  @param      len             ���ͳ���
//  @return     uint32          ʣ��δ���͵��ֽ���
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint32 bluetooth_ch9141_send_buff (uint8 *buff, uint32 len)
{
    uint16 time_count = 0;
    while(len > 30)
    {
        time_count = 0;
        while(gpio_get(BLUETOOTH_CH9141_RTS_PIN) && time_count++ < BLUETOOTH_CH9141_TIMEOUT_COUNT)  // ���RTSΪ�͵�ƽ���������������
            systick_delay_ms(1);
        if(time_count >= BLUETOOTH_CH9141_TIMEOUT_COUNT)
            return len;                                                         // ģ��æ,���������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
        uart_putbuff(BLUETOOTH_CH9141_INDEX, buff, 30);

        buff += 30;                                                             // ��ַƫ��
        len -= 30;                                                              // ����
    }

    time_count = 0;
    while(gpio_get(BLUETOOTH_CH9141_RTS_PIN) && time_count++ < BLUETOOTH_CH9141_TIMEOUT_COUNT)      // ���RTSΪ�͵�ƽ���������������
        systick_delay_ms(1);
    if(time_count >= BLUETOOTH_CH9141_TIMEOUT_COUNT)
        return len;                                                             // ģ��æ,���������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
    uart_putbuff(BLUETOOTH_CH9141_INDEX, buff, len);                            // ������������

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�� ��ȡ����
//  @param      buff            �洢�����ݵ�ַ
//  @param      len             ����
//  @return     uint32          ʵ�ʶ�ȡ�ֽ���
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint32 bluetooth_ch9141_read_buff (uint8 *buff, uint32 len)
{
    uint32 data_len = len;
    fifo_read_buffer(&bluetooth_ch9141_fifo, buff, &data_len, FIFO_READ_AND_CLEAN);
    return data_len;
}
