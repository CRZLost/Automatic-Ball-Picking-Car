/*
 * PWM.c
 *
 *  Created on: Jun 22, 2023
 *      Author: Lost
 */


#include "debug.h"                  // Device header
#include "GPIO_Init.h"
#include "PWM.h"

/*
    TIM2��������PWM���ο����ٶȵ�ͬʱ
    ÿ�θ��´����ж϶�ȡTIM3�е�CNT��ֵ
    ����v=N/T ����ٶ�
*/
void PWM_Init(void)
{
/**************************************************/
    //���ü�ʱ��TIM2
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); //��TIM2�˿ڶ�Ӧ��ʱ��

    TIM_InternalClockConfig(TIM2); //TIM2���ڲ�ʱ��Ϊʱ��Դ

    TIM_TimeBaseInitTypeDef TimeBaseInit;

    TimeBaseInit.TIM_ClockDivision=TIM_CKD_DIV1; //ȷ������Ƶ�ʣ������ź��˲�
    TimeBaseInit.TIM_CounterMode= TIM_CounterMode_Up; //ȷ������ģʽ
    TimeBaseInit.TIM_Period=400-1; //��װֵ ARR ֱ�������100��
    TimeBaseInit.TIM_Prescaler=900-1; //Ԥ��Ƶ����
    TimeBaseInit.TIM_RepetitionCounter=0; //����������ٴκ������ж�
    TIM_TimeBaseInit(TIM2,&TimeBaseInit);

/**************************************************/
    //��������Ƚ��ź�

    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);//ʹ��TIM��CCR�ϵ�Ԥװ�ؼĴ���
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCStructInit(&TIM_OCInitStruct); //�ȸ���Ĭ��ֵ���ٸĹؼ�����
    TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1; //PWM1ģʽ
    TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High; //reference��Чʱ����ߵ�ƽ
    TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable; //�����������
    TIM_OCInitStruct.TIM_Pulse=0;
    TIM_OC1Init(TIM2,&TIM_OCInitStruct);
    TIM_OC2Init(TIM2,&TIM_OCInitStruct);
    TIM_OC3Init(TIM2,&TIM_OCInitStruct);
    TIM_OC4Init(TIM2,&TIM_OCInitStruct);

/**************************************************/
    //����GPIO����
    GPIOA_Init(GPIO_Pin_0,GPIO_Mode_AF_PP);
    GPIOA_Init(GPIO_Pin_1,GPIO_Mode_AF_PP);
    GPIOA_Init(GPIO_Pin_2,GPIO_Mode_AF_PP);
    GPIOA_Init(GPIO_Pin_3,GPIO_Mode_AF_PP);
    /*
        AF_PP����������� Pin�Ŀ���Ȩ������� �ɸ���������п���
        GPIOA1��ӦOutput Compare Channel 1 ��Ӧ��ϵ����
        �����Կ���AFIO�����Ž�����ӳ��
    */

    /************************************************************/
    /*����ʱ�ӵ��ж�ģʽ*/
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //���������ж�

/************************************************************/
    /*����NVIC*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel=TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;

    NVIC_Init(&NVIC_InitStruct);

    TIM_Cmd(TIM2,ENABLE);//����TIM2
}

//����400
void Duty_Set(Wheel_t Wheel,Wheel_Speed_Typdef* Wheel_Speed)
{
     TIM_OCInitTypeDef TIM_OCInitStruct;
     TIM_OCStructInit(&TIM_OCInitStruct); //�ȸ���Ĭ��ֵ���ٸĹؼ�����
     TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1; //PWM1ģʽ
     TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High; //reference��Чʱ����ߵ�ƽ
     TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable; //�����������

     //����Wheel ѡ��ı��ĸ����ӵ�ת��
    switch (Wheel) {

        case FL:
            TIM_OCInitStruct.TIM_Pulse=(uint32_t)(Wheel_Speed->FL_Speed);
            TIM_OC1Init(TIM2,&TIM_OCInitStruct);
            break;

        case FR:
            TIM_OCInitStruct.TIM_Pulse=(uint32_t)(Wheel_Speed->FR_Speed);
            TIM_OC2Init(TIM2,&TIM_OCInitStruct);
            break;

        case BR:
             TIM_OCInitStruct.TIM_Pulse=(uint32_t)(Wheel_Speed->BR_Speed);
             TIM_OC3Init(TIM2,&TIM_OCInitStruct);
             break;

        case BL:
           TIM_OCInitStruct.TIM_Pulse=(uint32_t)(Wheel_Speed->BL_Speed);
           TIM_OC4Init(TIM2,&TIM_OCInitStruct);
           break;

        default:
            break;
    }

}




