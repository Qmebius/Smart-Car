#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "headfile.h"

typedef struct
{
    uint8     *buffer;                                                          // ����ָ��
    uint32    head;                                                             // ����ͷָ�� ����ָ��յĻ���
    uint32    end;                                                              // ����βָ�� ����ָ��ǿջ��棨����ȫ�ճ��⣩
    uint32    size;                                                             // ����ʣ���С
    uint32    max;                                                              // �����ܴ�С
}fifo_struct;

typedef enum
{
    FIFO_SUCCESS,

    FIFO_BUFFER_NULL,
    FIFO_SPACE_NO_ENOUGH,
    FIFO_DATA_NO_ENOUGH,
}fifo_state_enum;

typedef enum
{
    FIFO_READ_AND_CLEAN,
    FIFO_READ_ONLY,
}fifo_operation_enum;

fifo_state_enum fifo_init           (fifo_struct *fifo, uint8 *buffer_addr, uint32 size);
void            fifo_head_offset    (fifo_struct *fifo, uint32 offset);
void            fifo_end_offset     (fifo_struct *fifo, uint32 offset);
void            fifo_clear          (fifo_struct *fifo);
uint32          fifo_used           (fifo_struct *fifo);

fifo_state_enum fifo_read_buffer    (fifo_struct *fifo, uint8 *dat, uint32 *length, fifo_operation_enum flag);
fifo_state_enum fifo_write_buffer   (fifo_struct *fifo, uint8 *dat, uint32 length);



#define BLUETOOTH_CH9141_INDEX              USART_8                              // ����ģ�� 1 ��Ӧʹ�õĴ��ں�
#define BLUETOOTH_CH9141_BUAD_RATE          115200                              // ����ģ�� 1 ��Ӧʹ�õĴ��ڲ�����
#define BLUETOOTH_CH9141_TX_PIN             UART8_RX_D17                         // ����ģ�� 1 ��Ӧģ��� TX Ҫ�ӵ���Ƭ���� RX
#define BLUETOOTH_CH9141_RX_PIN             UART8_TX_D16                        // ����ģ�� 1 ��Ӧģ��� RX Ҫ�ӵ���Ƭ���� TX
#define BLUETOOTH_CH9141_RTS_PIN            D26                                 // ����ģ�� 1 ��Ӧģ��� RTS ����

#define BLUETOOTH_CH9141_BUFFER_SIZE        64
#define BLUETOOTH_CH9141_TIMEOUT_COUNT      500


uint8       bluetooth_ch9141_init               (void);

uint32      bluetooth_ch9141_send_buff          (uint8 *buff, uint32 len);
uint32      bluetooth_ch9141_read_buff          (uint8 *buff, uint32 len);





#endif
