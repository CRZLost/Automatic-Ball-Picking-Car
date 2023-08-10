#include "debug.h"                  // Device header
#include "Servo.h"
#include "PWM.h"
#include "Camera.h"
#include "OLED.h"
#include "GPIOSection_Init.h"
#include "PID.h"
#include "stdlib.h"
#include "Timer.h"
#include "Filter.h"

#define Pan_Init 97
#define Tilt_Init 150
#define TRUE 1
#define FALSE 0

/* �Զ��庯�������� */
float Ave(const float *arr, int n);
void err_store(float *arr, int length, float err_new);

PID_TypeDef PID_Pan, PID_Tilt;

/*����ȥë��*/
int cnt_T = 0, cnt_F = 0;                                       // ״̬�б������                                                                                                      // ����״̬ȥë��
float ang_err_T_True[2][10] = {0.0};
/*0 for pan, 1 for tilt*/

/*�ؼ������жϱ�־*/
int blobs_detect =FALSE;
uint8_t blobs_detect_last=FALSE;
int PID_Switch = TRUE;
uint8_t Change_Flag=FALSE;

/*����������*/
float pan_ang=Pan_Init, tilt_ang=Tilt_Init;
float pan_ang_delta = 0, tilt_ang_delta = 0;
// float err_ang[2][3];

/*
tilt_ang_delta <- Phi_Error
pan_ang_delta <- Theta_Error
*/

/*����ͨ�����*/
char CameraData[12];
uint8_t MCU_Cmd;
float Data2MCU[3];
Openmv_DataSet_Typedef Openmv_DataSet;

//ʱ�ӱ���
uint64_t Real_Time,Real_Time_ms;

//�˲�����
float Phi_Error_Store[2];
float Theta_Error_Store[2];
float Disatance_Store[2];

//��������
uint16_t detect_count=0;
uint16_t lost_count=0;

//��־λ
uint32_t Fail_Count=0;
uint8_t Validation_flag=0;

int main(void)
{

    uint8_t i,j;

    SystemCoreClockUpdate();
    Delay_Init();
    Clock_Init();
    PWM_Init();                                                     // PWM�����ʼ��
    /* PB8 -> SCL, PB9 -> SDA */
    OLED_Init();
    /*����Ƕȳ�ʼ��*/
    Servo_SetAngle_Pan(Pan_Init);
    Servo_SetAngle_Tilt(Tilt_Init);
    Camera_Serial_Init();
    MCU_Serial_Init();
//  GPIOA_Init(GPIO_Pin_0,GPIO_Mode_Out_PP);
    PID_init(0.3,0.0005,0.3,&PID_Pan);
    PID_init(0.40,0.0005,0.45, &PID_Tilt);
//    OLED_Clear();
//    OLED_ShowString(1, 1, "Pan_A:");
//    OLED_ShowString(2, 1, "Tilt_A:");
//    OLED_ShowString(3, 1, "Pan_D:");
//    OLED_ShowString(4, 1, "Tilt_D:");
        while(TRUE)
        {
        /*��OpenMV��ȡ�Ƕ�����*/
        // ��鴮��ͨ�Ų�����
            CameraData_Process(&Openmv_DataSet);
            /* blobs_detect �ж� */

            //�˴���Ҫ��Openmv���������ݽ����˲����� ���������ж��źŽ��м��


            //�����ж�  һ���ж��Ƿ���ֻ�ɫ  �ڶ����ж��Ƿ�pass

            //��һ���жϣ��ж��Ƿ���ֻ�ɫ
            if((Openmv_DataSet.Distance==-1)||(Openmv_DataSet.Phi_Error==-181)||(Openmv_DataSet.Theta_Error == -181))
            {
              blobs_detect=FALSE;
              // GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
            }
            else
            {
                detect_count++;
                if(detect_count>500)
                {
                    blobs_detect = TRUE;
                    detect_count=0;
                }
                // GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
            }


            //ͻ���¼�ȥ��֮ǰ�¼���Ӱ��
            if(blobs_detect_last!=blobs_detect)//�ж��Ƿ���֡���ʧС��
            {
                Change_Flag=1;
            }
            blobs_detect_last=blobs_detect; //���汾���ж�

            if(Change_Flag)
            {
                for(i=0;i<2;i++)
                {
                    for(j=0;j<5;j++)
                    {
                        ang_err_T_True[i][j]=0;//������ͻ���¼�ȥ���ϴ�����
                    }
                }
                Change_Flag=0;
            }


            //�ж������Ƿ���Ч
            if(blobs_detect)
            {
                if((Openmv_DataSet.Distance==-2)||(Openmv_DataSet.Phi_Error==-200)||(Openmv_DataSet.Theta_Error == -200))
                {
                    Validation_flag=0;
                }
                else
                {
                    Validation_flag=1;
                }
            }



            if(blobs_detect&&Validation_flag)
            {
                cnt_F=0;//�����ʧͳ��

                //��Openmv���������ݽ��е�ͨ�˲���ȥ����Ƶ������
                Phi_Error_Store[0]=Phi_Error_Store[1];
                Phi_Error_Store[1]=Openmv_DataSet.Phi_Error;

                Theta_Error_Store[0]=Theta_Error_Store[1];
                Theta_Error_Store[1]=Openmv_DataSet.Theta_Error;

                Openmv_DataSet.Phi_Error+=Low_Pass_Filter(Real_Time_ms, Real_Time,Phi_Error_Store[1]-Phi_Error_Store[0],0.05);
                Openmv_DataSet.Theta_Error+=Low_Pass_Filter(Real_Time_ms, Real_Time,Theta_Error_Store[1]-Theta_Error_Store[0],0.05);

                tilt_ang_delta = Openmv_DataSet.Phi_Error;
                pan_ang_delta = Openmv_DataSet.Theta_Error;

                err_store(ang_err_T_True[0],10, pan_ang_delta);
                err_store(ang_err_T_True[1],10, tilt_ang_delta);


                /*ֱ�ӿ��� or PID����ѡ��*/
                if(PID_Switch)
                {
                    /* PID�Ƕȵ��� */
                    pan_ang += pid(-Ave(ang_err_T_True[0],10),0,&PID_Pan);
                    tilt_ang -= pid(-Ave(ang_err_T_True[1],10),0,&PID_Tilt);
                }
                else
                {/*ֱ�ӿ���*/
                    /* ֱ�ӽǶȵ��� */
                    pan_ang+=Ave(ang_err_T_True[0], 5);
                    tilt_ang-=Ave(ang_err_T_True[1], 5);
                }

                {/* ������ */
                    if(pan_ang>150)
                    {
                        pan_ang=150;

                    }
                    if(pan_ang<30)
                    {
                        pan_ang=30;

                    }
                    if(tilt_ang<30)
                    {
                        tilt_ang=30;

                    }
                    if(tilt_ang>180)
                    {
                        tilt_ang=180;

                    }
                }

                // �������Pan, Tilt�Ƕ�
                Servo_SetAngle_Pan(pan_ang);
                Servo_SetAngle_Tilt(tilt_ang);
                Delay_Ms(100);//ע��ʹ��delayʹ������㹻��ʱ��


            }
            else
            {

                /*�������ж��Ƿ�λ�����ⷴ������*/
                {
                    cnt_F++;
                    if(cnt_F==1500)
                    {
                        cnt_F = 0;
                        pan_ang_delta = 0;
                        tilt_ang_delta = 0;
                        pan_ang = Pan_Init;
                        tilt_ang = Tilt_Init;
                        // ����Ƕȵ���
                        Servo_SetAngle_Pan(pan_ang);
                        Servo_SetAngle_Tilt(tilt_ang);
                    }
                }

            }


            /* �������ݰ� */
            {
                Data2MCU[0] = Openmv_DataSet.Distance;
                Data2MCU[1] = tilt_ang;
                Data2MCU[2] = pan_ang;
                Serial_SendFloat(Data2MCU,3);
            }
        }



}

/* �Զ��庯�������� */
float Ave(const float *arr, int length)
{
    int i;
    float ave_list = 0;

    for(i = 0; i<length; i++)
    {
            ave_list += *(arr+i);
    }
    ave_list /= length;
    return ave_list;
}
void err_store(float *arr, int length, float err_new)
{
    int i;
    for(i=0; i<length;i++)
    {
        arr[i] = arr[i+1];
    }
    arr[i-1] = err_new;
}



//�����жϺ��� ������ʱ
void TIM1_UP_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast"))); //��ֹ����ֻ��һ���ж�
void TIM1_UP_IRQHandler(void)
{
    if(TIM_GetFlagStatus(TIM1, TIM_FLAG_Update)==1)
    {
        Real_Time_ms+=1;
        if(Real_Time_ms==1000)
        {
            Real_Time+=1;


            Real_Time_ms=0;
        }
        TIM_ClearFlag(TIM1, TIM_FLAG_Update);
    }
}






