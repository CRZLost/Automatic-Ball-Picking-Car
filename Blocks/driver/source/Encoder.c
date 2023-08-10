/*
 * Encoder.c
 *
 *  Created on: Jun 22, 2023
 *      Author: Lost
 */



#include "debug.h"                 // Device header
#include "Encoder.h"

void Encoder_Init(void)
{

/***************************************************************************************************/
    //Left_Back Motor
/***************************************************************************************/
    //����Ƶ��ʱ�� Ƶ�ʶ�ӦChannel1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //ѡ���ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); //��TIM3�˿ڶ�Ӧ��ʱ��

/*****************************************/
    //����GPIO
    GPIO_InitTypeDef GPIO_InitStruture;

    GPIO_InitStruture.GPIO_Mode= GPIO_Mode_IN_FLOATING; //ѡ��ģʽ
    GPIO_InitStruture.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7; //ѡ������
    GPIO_InitStruture.GPIO_Speed=GPIO_Speed_50MHz; //ѡ��GPIO���ٶ�
    GPIO_Init(GPIOA,&GPIO_InitStruture);//����GPIOA

/**********************************************/
    //���ü�ʱ��TIM3�ļ�����Ԫ

    TIM_TimeBaseInitTypeDef TimeBaseInit;

    TimeBaseInit.TIM_ClockDivision=TIM_CKD_DIV1; //ȷ������Ƶ�ʣ������ź��˲�
    TimeBaseInit.TIM_CounterMode= TIM_CounterMode_Up; //ȷ������ģʽ
    TimeBaseInit.TIM_Period=65536-1; //��װֵ
    TimeBaseInit.TIM_Prescaler=1-1; //Ԥ��Ƶ����
    TimeBaseInit.TIM_RepetitionCounter=0; //����������ٴκ������ж�
    TIM_TimeBaseInit(TIM3,&TimeBaseInit);

/**********************************************/
    //�������벶��Ƶ�����Ų���
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICStructInit(&TIM_ICInitStructure); //����ʼֵ
    TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICFilter=0xF; //�˲����� ֻ��N������ͬ�Żᷢ���źŸı� ��˿��Թ����ź�
    TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising; //�ߵ͵�ƽ���Բ���ת
    TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//�Ƿ�Բɼ����źŷ�Ƶ
    TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI; //ѡ����ֱ�����ǽ�������
    TIM_ICInit(TIM3,&TIM_ICInitStructure);

    TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICFilter=0xF; //�˲����� ֻ��N������ͬ�Żᷢ���źŸı� ��˿��Թ����ź�
    TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising; //�ߵ͵�ƽ���Բ���ת
    TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//�Ƿ�Բɼ����źŷ�Ƶ
    TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI; //ѡ����ֱ�����ǽ�������
    TIM_ICInit(TIM3,&TIM_ICInitStructure);

/**********************************************/
    /*
        ʹ�ü�ʱ����Encoder��ģʽ ʹ���ⲿ����ʱ��
        ����AB�źŵ���Լ��Ծ���+-1
        TIM3 X4ģʽ �����źŶ�����ת
    */
    TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
    TIM_Cmd(TIM3,ENABLE);


/***************************************************************************************************/
        //Left_Front Motor
    /***************************************************************************************/
        //����Ƶ��ʱ�� Ƶ�ʶ�ӦChannel1
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //ѡ���ʱ��
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE); //��TIM3�˿ڶ�Ӧ��ʱ��

    /*****************************************/
        //����GPIO

        GPIO_InitStruture.GPIO_Mode= GPIO_Mode_IN_FLOATING; //ѡ��ģʽ
        GPIO_InitStruture.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7; //ѡ������
        GPIO_InitStruture.GPIO_Speed=GPIO_Speed_50MHz; //ѡ��GPIO���ٶ�
        GPIO_Init(GPIOB,&GPIO_InitStruture);//����GPIOA

    /**********************************************/
        //���ü�ʱ��TIM3�ļ�����Ԫ

        TimeBaseInit.TIM_ClockDivision=TIM_CKD_DIV1; //ȷ������Ƶ�ʣ������ź��˲�
        TimeBaseInit.TIM_CounterMode= TIM_CounterMode_Up; //ȷ������ģʽ
        TimeBaseInit.TIM_Period=65536-1; //��װֵ
        TimeBaseInit.TIM_Prescaler=1-1; //Ԥ��Ƶ����
        TimeBaseInit.TIM_RepetitionCounter=0; //����������ٴκ������ж�
        TIM_TimeBaseInit(TIM4,&TimeBaseInit);

    /**********************************************/
        //�������벶��Ƶ�����Ų���

        TIM_ICStructInit(&TIM_ICInitStructure); //����ʼֵ
        TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;
        TIM_ICInitStructure.TIM_ICFilter=0xF; //�˲����� ֻ��N������ͬ�Żᷢ���źŸı� ��˿��Թ����ź�
        TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising; //�ߵ͵�ƽ���Բ���ת
        TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//�Ƿ�Բɼ����źŷ�Ƶ
        TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI; //ѡ����ֱ�����ǽ�������
        TIM_ICInit(TIM4,&TIM_ICInitStructure);

        TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;
        TIM_ICInitStructure.TIM_ICFilter=0xF; //�˲����� ֻ��N������ͬ�Żᷢ���źŸı� ��˿��Թ����ź�
        TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising; //�ߵ͵�ƽ���Բ���ת
        TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//�Ƿ�Բɼ����źŷ�Ƶ
        TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI; //ѡ����ֱ�����ǽ�������
        TIM_ICInit(TIM4,&TIM_ICInitStructure);


/**********************************************/
    /*
        ʹ�ü�ʱ����Encoder��ģʽ ʹ���ⲿ����ʱ��
        ����AB�źŵ���Լ��Ծ���+-1
        TIM3 X4ģʽ �����źŶ�����ת
    */
    TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
    TIM_Cmd(TIM4,ENABLE);



/***************************************************************************************************/
        //Right_Front Motor
    /***************************************************************************************/
        //����Ƶ��ʱ�� Ƶ�ʶ�ӦChannel1
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); //ѡ���ʱ��
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE); //��TIM3�˿ڶ�Ӧ��ʱ��

    /*****************************************/
        //����GPIO

        GPIO_InitStruture.GPIO_Mode= GPIO_Mode_IN_FLOATING; //ѡ��ģʽ
        GPIO_InitStruture.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7; //ѡ������
        GPIO_InitStruture.GPIO_Speed=GPIO_Speed_50MHz; //ѡ��GPIO���ٶ�
        GPIO_Init(GPIOC,&GPIO_InitStruture);//����GPIOA

    /**********************************************/
        //���ü�ʱ��TIM3�ļ�����Ԫ

        TimeBaseInit.TIM_ClockDivision=TIM_CKD_DIV1; //ȷ������Ƶ�ʣ������ź��˲�
        TimeBaseInit.TIM_CounterMode= TIM_CounterMode_Up; //ȷ������ģʽ
        TimeBaseInit.TIM_Period=65536-1; //��װֵ
        TimeBaseInit.TIM_Prescaler=1-1; //Ԥ��Ƶ����
        TimeBaseInit.TIM_RepetitionCounter=0; //����������ٴκ������ж�
        TIM_TimeBaseInit(TIM8,&TimeBaseInit);

    /**********************************************/
        //�������벶��Ƶ�����Ų���

        TIM_ICStructInit(&TIM_ICInitStructure); //����ʼֵ
        TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;
        TIM_ICInitStructure.TIM_ICFilter=0xF; //�˲����� ֻ��N������ͬ�Żᷢ���źŸı� ��˿��Թ����ź�
        TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising; //�ߵ͵�ƽ���Բ���ת
        TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//�Ƿ�Բɼ����źŷ�Ƶ
        TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI; //ѡ����ֱ�����ǽ�������
        TIM_ICInit(TIM8,&TIM_ICInitStructure);

        TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;
        TIM_ICInitStructure.TIM_ICFilter=0xF; //�˲����� ֻ��N������ͬ�Żᷢ���źŸı� ��˿��Թ����ź�
        TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising; //�ߵ͵�ƽ���Բ���ת
        TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//�Ƿ�Բɼ����źŷ�Ƶ
        TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI; //ѡ����ֱ�����ǽ�������
        TIM_ICInit(TIM8,&TIM_ICInitStructure);

    /**********************************************/
        /*
            ʹ�ü�ʱ����Encoder��ģʽ ʹ���ⲿ����ʱ��
            ����AB�źŵ���Լ��Ծ���+-1
            TIM3 X4ģʽ �����źŶ�����ת
        */
        TIM_EncoderInterfaceConfig(TIM8,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
        TIM_Cmd(TIM8,ENABLE);


/***************************************************************************************************/
        //Right_Back Motor
    /***************************************************************************************/
        //����Ƶ��ʱ�� Ƶ�ʶ�ӦChannel1
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE); //ѡ���ʱ��
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO,ENABLE); //��TIM3�˿ڶ�Ӧ��ʱ��

    /*****************************************/
        //����GPIO

        /*��ӳ��TIM9��ͨ��*/
        GPIO_PinRemapConfig(GPIO_FullRemap_TIM9,ENABLE);
        GPIO_InitStruture.GPIO_Mode= GPIO_Mode_IN_FLOATING; //ѡ��ģʽ
        GPIO_InitStruture.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_11; //ѡ������
        GPIO_InitStruture.GPIO_Speed=GPIO_Speed_50MHz; //ѡ��GPIO���ٶ�
        GPIO_Init(GPIOD,&GPIO_InitStruture);//����GPIOA

    /**********************************************/
        //���ü�ʱ��TIM3�ļ�����Ԫ

        TimeBaseInit.TIM_ClockDivision=TIM_CKD_DIV1; //ȷ������Ƶ�ʣ������ź��˲�
        TimeBaseInit.TIM_CounterMode= TIM_CounterMode_Up; //ȷ������ģʽ
        TimeBaseInit.TIM_Period=65536-1; //��װֵ
        TimeBaseInit.TIM_Prescaler=1-1; //Ԥ��Ƶ����
        TimeBaseInit.TIM_RepetitionCounter=0; //����������ٴκ������ж�
        TIM_TimeBaseInit(TIM9,&TimeBaseInit);

    /**********************************************/
        //�������벶��Ƶ�����Ų���

        TIM_ICStructInit(&TIM_ICInitStructure); //����ʼֵ
        TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;
        TIM_ICInitStructure.TIM_ICFilter=0xF; //�˲����� ֻ��N������ͬ�Żᷢ���źŸı� ��˿��Թ����ź�
        TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising; //�ߵ͵�ƽ���Բ���ת
        TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//�Ƿ�Բɼ����źŷ�Ƶ
        TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI; //ѡ����ֱ�����ǽ�������
        TIM_ICInit(TIM9,&TIM_ICInitStructure);

        TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;
        TIM_ICInitStructure.TIM_ICFilter=0xF; //�˲����� ֻ��N������ͬ�Żᷢ���źŸı� ��˿��Թ����ź�
        TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising; //�ߵ͵�ƽ���Բ���ת
        TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//�Ƿ�Բɼ����źŷ�Ƶ
        TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI; //ѡ����ֱ�����ǽ�������
        TIM_ICInit(TIM9,&TIM_ICInitStructure);

    /**********************************************/
        /*
            ʹ�ü�ʱ����Encoder��ģʽ ʹ���ⲿ����ʱ��
            ����AB�źŵ���Լ��Ծ���+-1
            TIM3 X4ģʽ �����źŶ�����ת
        */
        TIM_EncoderInterfaceConfig(TIM9,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
        TIM_Cmd(TIM9,ENABLE);



}

void Encoder_Get_CCR(Encoder_CCR_Typedef* Encoder_CCR)
{
    Encoder_CCR->BL_CCR=(uint16_t)TIM_GetCounter(TIM3);
    Encoder_CCR->FL_CCR=(uint16_t)TIM_GetCounter(TIM4);
    Encoder_CCR->FR_CCR=(uint16_t)TIM_GetCounter(TIM8);
    Encoder_CCR->BR_CCR=(uint16_t)TIM_GetCounter(TIM9);
}

void Clear_Encoder(void)
{
    TIM_SetCounter(TIM3,0);//����Encoder ����
    TIM_SetCounter(TIM4,0);//����Encoder ����
    TIM_SetCounter(TIM8,0);//����Encoder ����
    TIM_SetCounter(TIM9,0);//����Encoder ����
}

void Encoder_Get_Wheel_RPM(float* Measured_Wheel_RPM)
{
    uint16_t i,j;
    int16_t CCR_Sum=0;
    float CCR_Ave[4];
    static int16_t CCR[4][20]; //����3��20�е�CCR ÿ�д���һ������ BL=[0] FL=[1] FR=[2] BR=[3]
    static uint16_t flag=1;

    if(flag)
    {
        for(i=0;i<4;i++)
        {
            for(j=0;j<20;j++)
            {
                CCR[i][j]=0;
            }
            CCR_Ave[i]=0;
        }
        flag=0;
    }

    //����20��CCR��ֵ���ȡƽ��ȥë��
    for(i=0;i<4;i++)
    {
        for(j=0;j<20;j++)
          {
              if(j<18)
              {
                  CCR[i][j]=CCR[i][j+1]; //����CCR ����ǰ��
              }
              else
              {
                 switch (i) {
                    case 0:
                        CCR[i][j]=(int16_t)TIM_GetCounter(TIM3); //BL
                        break;
                    case 1:
                       CCR[i][j]=(int16_t)TIM_GetCounter(TIM4); //FL
                       break;
                    case 2:
                       CCR[i][j]=(int16_t)TIM_GetCounter(TIM8); //FR
                       break;
                    case 3:
                       CCR[i][j]=(int16_t)TIM_GetCounter(TIM9); //BR
                       break;
                    default:
                        break;
                }
              }
              CCR_Sum+=CCR[i][j];
          }
      CCR_Ave[i]=CCR_Sum*0.05;
      CCR_Sum=0;
    }

    for(i=0;i<4;i++)
    {
      *(Measured_Wheel_RPM+i)=(*(CCR_Ave+i))*7.69231; // 1/200:��������  60:һ����60�� 4 X4ģʽ 390 ����һȦ390������
    }
}

