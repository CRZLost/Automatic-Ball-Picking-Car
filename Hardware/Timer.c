#include "debug.h"              // Device header

void Clock_Init()
{
/************************************************************/
    /*ѡ��ʱ���ź�Դ*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE); //����APB1ʱ��

    TIM_InternalClockConfig(TIM1); //�����ڲ�ʱ��2


/************************************************************/
    /*���ü�ʱ��Ԫ�Ļ�������*/
    TIM_TimeBaseInitTypeDef TimeBaseInit;

    TimeBaseInit.TIM_ClockDivision=TIM_CKD_DIV1; //ȷ������Ƶ�ʣ������ź��˲�
    TimeBaseInit.TIM_CounterMode= TIM_CounterMode_Up; //ȷ������ģʽ
    TimeBaseInit.TIM_Period=10-1; //��װֵ ARR
    TimeBaseInit.TIM_Prescaler=7200-1; //Ԥ��Ƶ���� PSC
    TimeBaseInit.TIM_RepetitionCounter=0; //����������ٴκ������ж�
    TIM_TimeBaseInit(TIM1,&TimeBaseInit);

/************************************************************/
    /*�����жϿ���*/
    TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);

/************************************************************/
    /*����NVIC*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel=TIM1_UP_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;

    NVIC_Init(&NVIC_InitStruct);

/************************************************************/
    /*������ʱ��*/
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
    TIM_Cmd(TIM1,ENABLE);
}



