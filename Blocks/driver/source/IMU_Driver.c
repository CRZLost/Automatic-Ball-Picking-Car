/*
 * IMU_Driver.c
 *
 *  Created on: Jun 22, 2023
 *      Author: Lost
 */


#include "debug.h"                // Device header
#include "Serial.h"

#define TX_Pin GPIO_Pin_9
#define RX_Pin GPIO_Pin_10

extern uint8_t IMU_ReceiveData[28]; //IMU�����ķ������� ��mainͳһ����
//uint8_t IMU_ReceiveData[28];

typedef struct{
    float Angle_X;
    float Angle_Y;
    float Angle_Z;
    float Angular_Speed_X;
    float Angular_Speed_Y;
    float Angular_Speed_Z;
    float Accerelation_X;
    float Accerelation_Y;
    float Accerelation_Z;
}IMU_DataSetTypedef;


void IMU_Init(void)
{
/***************************************************/
    //����ʱ��ģ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

/***************************************************/
    //����GPIO
    GPIO_InitTypeDef GPIO_InitStruture;

    GPIO_InitStruture.GPIO_Mode=GPIO_Mode_AF_PP; //ѡ��ģʽ ��������ģʽ ����ģ��ӹ����ŵĸߵ͵�ƽ ����ߵ͵�ƽ
    GPIO_InitStruture.GPIO_Pin=TX_Pin; //ѡ������
    GPIO_InitStruture.GPIO_Speed=GPIO_Speed_50MHz; //ѡ��GPIO���ٶ�
    GPIO_Init(GPIOA,&GPIO_InitStruture);//����GPIOA�Ĳ���

    GPIO_InitStruture.GPIO_Mode=GPIO_Mode_IN_FLOATING; //ѡ��ģʽ ��������ģʽ ���ſ���Ȩ�ɶ�Ӧ���ڵ�TX�ӹ� ע��TX RX����ģʽ��ͬ
    GPIO_InitStruture.GPIO_Pin=RX_Pin; //ѡ������
    GPIO_InitStruture.GPIO_Speed=GPIO_Speed_50MHz; //ѡ��GPIO���ٶ�
    GPIO_Init(GPIOA,&GPIO_InitStruture);//����GPIOA�Ĳ���

/***************************************************/
    //���ô���ģ��

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate=115200;  //������
    USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //����������
    USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx; //����ģʽ���� ����д
    USART_InitStructure.USART_Parity=USART_Parity_No; //У��λ
    USART_InitStructure.USART_StopBits=USART_StopBits_1; //ֹͣλ�Ŀ��
    USART_InitStructure.USART_WordLength=USART_WordLength_8b; //֡�Ŀ�� 8bits��У��λ 9 bits��1λУ��λ
    USART_Init(USART1,&USART_InitStructure);

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
    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE); //���ڶ�ȡ�Ĵ����ǿմ���

    DMA_InitTypeDef DMA_InitStruct;
    //USART1->DR ת��USART1��DR�Ĵ��� &ȡ�Ĵ����ĵ�ַ
    DMA_InitStruct.DMA_PeripheralBaseAddr=(uint32_t)(&USART1->DATAR); //�����׵�ַ ��ȡ�Ĵ�����ַ
    DMA_InitStruct.DMA_PeripheralDataSize= DMA_PeripheralDataSize_Byte;//�������ݿ��
    DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;//����ָ���Ƿ����
    DMA_InitStruct.DMA_MemoryBaseAddr=(uint32_t)&IMU_ReceiveData; //�ڴ��ַ
    DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte; //�ڴ����ݿ��
    DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable ;//�ڴ�ָ���Ƿ����
    DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralSRC  ;//���䷽�� SRC:������ΪԴͷ DIS������ΪĿ�ĵ�
    DMA_InitStruct.DMA_BufferSize=28; //��������� ����n�� ÿ�εݼ� ��0ֹͣ
    DMA_InitStruct.DMA_Mode=DMA_Mode_Circular;//DMAģʽ �Ƿ��Զ���װ
    DMA_InitStruct.DMA_M2M=DMA_M2M_Disable;//�����������Ӳ������ 1���������
    DMA_InitStruct.DMA_Priority=DMA_Priority_Medium;//���ȼ�


    DMA_Init(DMA1_Channel5,&DMA_InitStruct);//ÿ��Ӳ�����������Լ���Ӧ��channel��
    DMA_Cmd(DMA1_Channel5,ENABLE);



    USART_Cmd(USART1,ENABLE); //��������
}


void Position_Process(IMU_DataSetTypedef* IMU_DataSet)
{
    uint8_t i;
    static float AcceX[20],AcceY[20],AngularZ[20],Accex_Sum,Accey_Sum,AngularZ_Sum;

    for(i=0;i<19;i++)
    {
        AcceX[i]=AcceX[i+1];
        AcceY[i]=AcceY[i+1];
        AngularZ[i]=AngularZ[i+1];
        Accex_Sum+=AcceX[i];
        Accey_Sum+=AcceY[i];
        AngularZ_Sum+=AngularZ[i];
    }

    AcceX[19]=(float)((int16_t)(IMU_ReceiveData[16]<<8|IMU_ReceiveData[15]))/32768*2*9.18;
    AcceY[19]=(float)((int16_t)(IMU_ReceiveData[18]<<8|IMU_ReceiveData[17]))/32768*2*9.18;
    AngularZ[19]=(float)((int16_t)(IMU_ReceiveData[9]<<8|IMU_ReceiveData[8]))/32768*180;

    IMU_DataSet->Accerelation_X=(Accex_Sum+AcceX[19])/20;
    IMU_DataSet->Accerelation_Y=(Accey_Sum+AcceY[19])/20;
    IMU_DataSet->Angle_Z=(AngularZ_Sum+AngularZ[19])/20;

    IMU_DataSet->Angle_X=(float)((int16_t)(IMU_ReceiveData[5]<<8|IMU_ReceiveData[4]))/32768*180;
    IMU_DataSet->Angle_Y=(float)((int16_t)(IMU_ReceiveData[7]<<8|IMU_ReceiveData[6]))/32768*180;
//    IMU_DataSet->Angle_Z=(float)((int16_t)(IMU_ReceiveData[9]<<8|IMU_ReceiveData[8]))/32768*180;

//    IMU_DataSet->Accerelation_X=(float)((int16_t)(IMU_ReceiveData[16]<<8|IMU_ReceiveData[15]))/32768*2*9.18;
//    IMU_DataSet->Accerelation_Y=(float)((int16_t)(IMU_ReceiveData[18]<<8|IMU_ReceiveData[17]))/32768*2*9.18;
    IMU_DataSet->Accerelation_Z=(float)((int16_t)(IMU_ReceiveData[20]<<8|IMU_ReceiveData[19]))/32768*2*9.18;

    IMU_DataSet->Angular_Speed_X=(float)((int16_t)(IMU_ReceiveData[22]<<8|IMU_ReceiveData[21]))/32768*250;
    IMU_DataSet->Angular_Speed_Y=(float)((int16_t)(IMU_ReceiveData[24]<<8|IMU_ReceiveData[23]))/32768*250;
    IMU_DataSet->Angular_Speed_Z=(float)((int16_t)(IMU_ReceiveData[26]<<8|IMU_ReceiveData[25]))/32768*250;

    Accex_Sum=0;
    Accey_Sum=0;
    AngularZ_Sum=0;
}






