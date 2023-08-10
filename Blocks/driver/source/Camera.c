/*
 * Camera.c
 *
 *  Created on: Jun 28, 2023
 *      Author: Lost
 */
#include "debug.h"
#include "Camera.h"

extern char CameraData[12];//�н�Openmv������������ ��main����ͳһ����

void Camera_Init(void)
{

/************************************************/
    //���ڳ�ʼ��
    /***************************************************/
        //����ʱ��ģ��
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);

    /***************************************************/
        //����GPIO
        GPIO_InitTypeDef GPIO_InitStruture;

        GPIO_InitStruture.GPIO_Mode=GPIO_Mode_AF_PP; //ѡ��ģʽ ��������ģʽ ����ģ��ӹ����ŵĸߵ͵�ƽ ����ߵ͵�ƽ
        GPIO_InitStruture.GPIO_Pin=GPIO_Pin_10; //ѡ������
        GPIO_InitStruture.GPIO_Speed=GPIO_Speed_50MHz; //ѡ��GPIO���ٶ�
        GPIO_Init(GPIOC,&GPIO_InitStruture);//����GPIOA�Ĳ���

        GPIO_InitStruture.GPIO_Mode=GPIO_Mode_IN_FLOATING; //ѡ��ģʽ ��������ģʽ ���ſ���Ȩ�ɶ�Ӧ���ڵ�TX�ӹ� ע��TX RX����ģʽ��ͬ
        GPIO_InitStruture.GPIO_Pin=GPIO_Pin_11; //ѡ������
        GPIO_InitStruture.GPIO_Speed=GPIO_Speed_50MHz; //ѡ��GPIO���ٶ�
        GPIO_Init(GPIOC,&GPIO_InitStruture);//����GPIOA�Ĳ���

    /***************************************************/
        //���ô���ģ��

        USART_InitTypeDef USART_InitStructure;
        USART_InitStructure.USART_BaudRate=115200;  //������
        USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //����������
        USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx; //����ģʽ���� ����д
        USART_InitStructure.USART_Parity=USART_Parity_No; //У��λ
        USART_InitStructure.USART_StopBits=USART_StopBits_1; //ֹͣλ�Ŀ��
        USART_InitStructure.USART_WordLength=USART_WordLength_8b; //֡�Ŀ�� 8bits��У��λ 9 bits��1λУ��λ
        USART_Init(UART4,&USART_InitStructure);

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


        DMA_DeInit(DMA2_Channel3);
        DMA_InitTypeDef DMA_InitStruct;
        //USART1->DR ת��USART1��DR�Ĵ��� &ȡ�Ĵ����ĵ�ַ
        DMA_InitStruct.DMA_PeripheralBaseAddr=(uint32_t)&UART4->DATAR; //�����׵�ַ ��ȡ�Ĵ�����ַ
        DMA_InitStruct.DMA_PeripheralDataSize= DMA_PeripheralDataSize_Byte;//�������ݿ��
        DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;//����ָ���Ƿ����
        DMA_InitStruct.DMA_MemoryBaseAddr=(uint32_t)CameraData; //�ڴ��ַ
        DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte; //�ڴ����ݿ��
        DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable ;//�ڴ�ָ���Ƿ����
        DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralSRC  ;//���䷽�� SRC:������ΪԴͷ DIS������ΪĿ�ĵ�
        DMA_InitStruct.DMA_BufferSize=12; //��������� ����n�� ÿ�εݼ� ��0ֹͣ
        DMA_InitStruct.DMA_Mode=DMA_Mode_Circular;//DMAģʽ �Ƿ��Զ���װ
        DMA_InitStruct.DMA_M2M=DMA_M2M_Disable;//�����������Ӳ������ 1���������
        DMA_InitStruct.DMA_Priority=DMA_Priority_Medium;//���ȼ�


        DMA_Init(DMA2_Channel3,&DMA_InitStruct);//ÿ��Ӳ�����������Լ���Ӧ��channel��

        USART_Cmd(UART4,ENABLE); //��������
        DMA_Cmd(DMA2_Channel3,ENABLE);
        USART_DMACmd(UART4,USART_DMAReq_Rx,ENABLE); //���ڶ�ȡ�Ĵ����ǿմ���
}


void CameraData_Process(Camera_DataSet_Typedef* Camera_DataSet)
{

    //!!!!!!!!!!!!!!!1ע�����������������Ҫ�˶�
    Camera_DataSet->Distance=*((float*)CameraData+0);
    Camera_DataSet->Phi=*((float*)CameraData+1);
    Camera_DataSet->Theta=*((float*)CameraData+2);
}



