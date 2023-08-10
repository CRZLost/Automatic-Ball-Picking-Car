#include "debug.h"
#include "Serial.h"
#include "Data_Central_Process.h"

#define TX_Pin GPIO_Pin_10
#define RX_Pin GPIO_Pin_11

extern uint8_t Received_Data[8];
void Bluetooth_Init(void)
{

/***************************************************/
    //����ʱ��ģ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

/***************************************************/
    //����GPIO
    GPIO_InitTypeDef GPIO_InitStruture;

    GPIO_InitStruture.GPIO_Mode=GPIO_Mode_AF_PP; //ѡ��ģʽ ��������ģʽ ����ģ��ӹ����ŵĸߵ͵�ƽ ����ߵ͵�ƽ
    GPIO_InitStruture.GPIO_Pin=TX_Pin; //ѡ������
    GPIO_InitStruture.GPIO_Speed=GPIO_Speed_50MHz; //ѡ��GPIO���ٶ�
    GPIO_Init(GPIOB,&GPIO_InitStruture);//����GPIOA�Ĳ���

    GPIO_InitStruture.GPIO_Mode=GPIO_Mode_IN_FLOATING; //ѡ��ģʽ ��������ģʽ ���ſ���Ȩ�ɶ�Ӧ���ڵ�TX�ӹ� ע��TX RX����ģʽ��ͬ
    GPIO_InitStruture.GPIO_Pin=RX_Pin; //ѡ������
    GPIO_InitStruture.GPIO_Speed=GPIO_Speed_50MHz; //ѡ��GPIO���ٶ�
    GPIO_Init(GPIOB,&GPIO_InitStruture);//����GPIOA�Ĳ���

/***************************************************/
    //���ô���ģ��

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate=9600;  //������
    USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //����������
    USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx; //����ģʽ���� ����д
    USART_InitStructure.USART_Parity=USART_Parity_No; //У��λ
    USART_InitStructure.USART_StopBits=USART_StopBits_1; //ֹͣλ�Ŀ��
    USART_InitStructure.USART_WordLength=USART_WordLength_8b; //֡�Ŀ�� 8bits��У��λ 9 bits��1λУ��λ
    USART_Init(USART3,&USART_InitStructure);

/***************************************************/
    //����NVIC��USART�ж�

//  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE); //�������ڵĶ�ȡ�ж�
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //NVIC���鷽ʽ ȫ����ֻ����һ�ַ��鷽ʽ
//  NVIC_InitTypeDef NVIC_InitStructure;
//  NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;//�޸�λ ����NVIC������
//  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; //ʹ��
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //������ռ���ȼ�
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;//������Ӧ���ȼ�
//  NVIC_Init(&NVIC_InitStructure);

/***************************************************/
    //����DMA


    DMA_DeInit(DMA1_Channel3);
    DMA_InitTypeDef DMA_InitStruct;
    //USART1->DR ת��USART1��DR�Ĵ��� &ȡ�Ĵ����ĵ�ַ
    DMA_InitStruct.DMA_PeripheralBaseAddr=(uint32_t)&USART3->DATAR; //�����׵�ַ ��ȡ�Ĵ�����ַ
    DMA_InitStruct.DMA_PeripheralDataSize= DMA_PeripheralDataSize_Byte;//�������ݿ��
    DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;//����ָ���Ƿ����
    DMA_InitStruct.DMA_MemoryBaseAddr=(uint32_t)Received_Data; //�ڴ��ַ
    DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte; //�ڴ����ݿ��
    DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable ;//�ڴ�ָ���Ƿ����
    DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralSRC  ;//���䷽�� SRC:������ΪԴͷ DIS������ΪĿ�ĵ�
    DMA_InitStruct.DMA_BufferSize=8; //��������� ����n�� ÿ�εݼ� ��0ֹͣ
    DMA_InitStruct.DMA_Mode=DMA_Mode_Circular;//DMAģʽ �Ƿ��Զ���װ
    DMA_InitStruct.DMA_M2M=DMA_M2M_Disable;//�����������Ӳ������ 1���������
    DMA_InitStruct.DMA_Priority=DMA_Priority_Medium;//���ȼ�


    DMA_Init(DMA1_Channel3,&DMA_InitStruct);//ÿ��Ӳ�����������Լ���Ӧ��channel��

    USART_Cmd(USART3,ENABLE); //��������
    DMA_Cmd(DMA1_Channel3,ENABLE);
    USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE); //���ڶ�ȡ�Ĵ����ǿմ���


}


void Plot(Serial_Election_t Election,float* Data,uint8_t ChannelNum)
{
    uint8_t Tem[4],i,j; //���������ֵ�4�ֽ��ڴ����η���
    uint8_t EndCmd[4]={0x00,0x00,0x80,0x7f}; //Э����ֹ��

    for(i=0;i<ChannelNum;i++)
    {
        for(j=0;j<4;j++)
        {
            Tem[j]=*((uint8_t*)Data+4*i+j); //Data������ָ��תΪ�ֽ������θ�ֵ����
            Serial_SendByte(Election,Tem[j]);
        }

    }

    for(i=0;i<4;i++)
    {
        Serial_SendByte(Election,EndCmd[i]);
    }
}


void Vofa_Cmd_Analyze(char* Control_Mode)
{

   float Judge;
   uint8_t i;

   Judge=*((float*)Received_Data);
   if(Judge==1200||Judge==-1200)
   {
       if(*((float*)Received_Data)==-1200)
       {*Control_Mode='M';}
       if(*((float*)Received_Data)==1200)
       {*Control_Mode='A';}
      for(i=0;i<8;i++)
      {
          Received_Data[i]=0;
      }
   }
   if(Judge==1400)
   {
       *Control_Mode='S';//ֹͣ����
       for(i=0;i<8;i++)
       {
          Received_Data[i]=0;
       }
   }

}





