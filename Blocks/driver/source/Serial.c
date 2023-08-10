#include "debug.h"
#include "_ansi.h"
#include <stdio.h>
#include <stdarg.h>
#include "Serial.h"



void Serial_SendByte(Serial_Election_t Election,uint8_t Data) //����һ���ֽ�
{

    switch (Election) {
        case Serial1:
            USART_SendData(USART1,Data); //��ֵ��DR�Ĵ��� ����������λ�Ĵ�����λ���з���
            while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
            break;

        case Serial2:
            USART_SendData(USART2,Data); //��ֵ��DR�Ĵ��� ����������λ�Ĵ�����λ���з���
            while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
            break;

        case Serial3:
            USART_SendData(USART3,Data); //��ֵ��DR�Ĵ��� ����������λ�Ĵ�����λ���з���
            while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);
            break;

        default:
            break;
    }

   /*
    ���TX�Ĵ������ź� ����ǿ� FLAG=RESET ˵������TXδ���뷢����λ�Ĵ���
    ��Ҫ�ȴ�FlAG=SET ȷ��TX�Ѿ�������λ�Ĵ������з��� ��ֹ�����ڴ���ǰ�ͱ�����
    ��TX��д��� FLAG=RESET ��Ӳ���Զ���λ

    �˴��߼��� д��TX�Ĵ�����ȴ�����λ����λ�Ĵ������з��� TXE=SET
    ����ȷ��ÿ��д��TX�Ĵ�����ʱ�����涼�ǿյ� ���������ݱ�����
    */
}

void Serial_SendArray(Serial_Election_t Election,uint8_t *Array_Address,uint16_t Length)
{
    uint16_t i;

    for(i=0;i<Length;i++)
    {
        Serial_SendByte(Election,*(Array_Address+i));
    }
}

void Serial_SendString(Serial_Election_t Election,char* String_Address) //����һ���ַ�����ָ�� ͬʱ�ַ����Դ���ֹλ'\0'
{
    uint16_t i;

    for(i=0;String_Address[i]!='\0';i++)
    {
        Serial_SendByte(Election,*(String_Address+i));
    }

}

uint32_t Pow(uint32_t X,uint32_t Y)
{
    uint32_t Result=1;

    while(Y--)
    {
        Result=Result*X;
    }

    return Result;

}

void Serial_SendNum(Serial_Election_t Election,uint32_t Number,uint8_t Length)
{
    uint8_t i;

    for(i=0;i<Length;i++)
    {
        Serial_SendByte(Election,Number/Pow(10,Length-i-1)%10+'0');
    }
}

//int fputc(int ch,FILE* f)
//{
//    Serial_SendByte(ch); //��printf�ķ����ض�λ�����ڽ������
//    return ch;
//}


//void Serial_Printf(char *format,...) //�ɱ�������Ӻ�������
//{
//    char String[200];
//
//    va_list arg;
//    va_start(arg,format);
//    vsprintf(String,format,arg);
//    va_end(arg);
//    Serial_SendString(String);
//
//}

//�жϺ���
//void USART1_IRQHandler(void)__attribute__((intterrupt()));
//
//void USART1_IRQHandler(void)
//{
//  if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)
//  {
//      ReceiveData=USART_ReceiveData(USART1);
//  }
//}




