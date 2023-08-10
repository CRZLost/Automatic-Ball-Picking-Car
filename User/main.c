/*
 *  Author:������
 *  Data:2023.6/7
 *  �Ĵ���ѧ 2020��
 *
 * */

#include "Wheel_Speed_Control.h"
#include "debug.h"
#include "GPIO_Init.h"
#include "OLED.h"
#include "Serial.h"
#include "IMU_Driver.h"
#include "Bluetooth.h"
#include "PWM.h"
#include "Encoder.h"
#include "PID_Controller.h"
#include "DC_Motor.h"
#include "Camera.h"
#include "Power.h"
#include "Data_Central_Process.h"
#include "Timer.h"
#include "math.h"
#include  "Filter.h"

/************************************************************/
//ȫ�ֱ�����

     //����ʱ��
    uint64_t Real_Time=0,Real_Time_ms=0,Current_Time=0;

    //VofaͨѶ����
    uint8_t Received_Data[8];  //����������ָ������
    float Plot_Data[10]; //ͨ��Vofa�������ݸ�����
    float watch[8];//�������
    Serial_Election_t Serial_Election;//ѡ����������
    char Control_Mode='A';

   //IMUģ��
    IMU_DataSetTypedef IMU_DataSet; //IMU���ݰ�
    uint8_t IMU_ReceiveData[28]; //IMU�����ķ������� ��mainͳһ����
    Car_State_Tpyedef Car_State;
    float AngleZ_Store[2]={0}; //��IMU�������ݵĴ���

   //Cameraģ��
    char CameraData[12];//�н�Openmv������ԭʼ������ ��main����ͳһ������
    Camera_DataSet_Typedef Camera_DataSet;//����Openmv�õ�������
    static float R_Theta[3]={0}; //���漫����ϵ [0]���� [1]�ƶ�ƫ��������Y�� [2]����
    float Distance_Store[2]={0};

    //������������
    Wheel_Speed_Reference_Typedef Wheel_Speed_Reference; //�����ٶȵ�Ԥ��ֵ
    float Measured_Wheel_RPM[4]; //��Encoder�õ���4��RPM BL=[0] FL=[1] FR=[2] BR=[3] ��ΪWheel_Speed_Control�ķ���

    //��������
    Power_Manage_Typedef Power_Select;

    //���̱�־����
    uint8_t Spin_Flag=0,Rotate_Pause_Flag=0;


int main(void)
{

/************************************************************/
    //main�����ľֲ�������
    int i;
    static float Velocity_Reference=0,Angular_Reference=0;//�����ٶȺͽ��ٶ�

/************************************************************/
    //������ʼ����
    SystemCoreClockUpdate();
    Delay_Init();
    IMU_Init();
    Bluetooth_Init();
    PWM_Init();
    Encoder_Init();
    DCMotor_Init();
    Camera_Init();
    Power_Manage_Init();
    Clock_Init();
    OLED_Init();



    while(1)
    {
      //��ֹ �ȴ�����ͷ��ʼ�����
      while(Real_Time<4)

      OLED_ShowString(1,1,"Preparing...");

/******************************************************************************/
     //��Դ����
      Power_Select=Power_Wheel;
      Power_Manage(Power_Select,1);

//    Power_Select=Power_Motor;
//    Power_Manage(Power_Select,1);
/******************************************************************************/
    //������� �����ֿػ����Կ�
    Vofa_Cmd_Analyze(&Control_Mode);

/******************************************************************************/
    //Openmv
    CameraData_Process(&Camera_DataSet); //��������ͷ����������

/******************************************************************************/
    //IMUģ��
    Position_Process(&IMU_DataSet);
//    State_Resolve(&IMU_DataSet,&Car_State,Real_Time);//������ǰ��С��״̬

/******************************************************************************/
    //�ź��˲�
    /*
     * �������źŽ����˲�����ȥ��Ư��
     * ��ע����ǶԺ���ǵ������������˲����� ���������еľ�̬Ư��
     */
    AngleZ_Store[0]=AngleZ_Store[1];
    AngleZ_Store[1]=IMU_DataSet.Angle_Z;
    IMU_DataSet.Angle_Z+=High_Pass_Filter(Real_Time_ms, Real_Time,(AngleZ_Store[1]-AngleZ_Store[0]),1);

    Distance_Store[0]=Distance_Store[1];
    Distance_Store[1]=Camera_DataSet.Distance;
    Camera_DataSet.Distance+=Low_Pass_Filter(Real_Time_ms, Real_Time,(Distance_Store[1]-Distance_Store[0]),0.2);

/******************************************************************************/
    //���ݴ�������

    //ģʽѡ������
        if(Control_Mode=='A') //����ǡ�A����MCU�Զ�����
        {
            Camera_Control(R_Theta,&Velocity_Reference,Real_Time,Real_Time_ms,&IMU_DataSet,&Camera_DataSet,&Spin_Flag,&Rotate_Pause_Flag);
            Rotate_PID_Controller(R_Theta,IMU_DataSet.Angle_Z,&Angular_Reference,Spin_Flag,Rotate_Pause_Flag,0.1,-0.1);
        }


        if(Control_Mode=='M')//�����M���˸�ָ��
        {
             Manual_Control(&Velocity_Reference, R_Theta);
             Spin_Flag=0;
             Rotate_PID_Controller(R_Theta,IMU_DataSet.Angle_Z,&Angular_Reference,Spin_Flag,Rotate_Pause_Flag,0.628,-0.628);

        }


        if(Control_Mode=='S')//�����ƶ�
        {
            Velocity_Reference=0;
            Angular_Reference=0;
            Power_Select=Power_Wheel;
            Power_Manage(Power_Select,0);
            Spin_Flag=0;
            Rotate_PID_Controller(R_Theta,IMU_DataSet.Angle_Z,&Angular_Reference,Spin_Flag,Rotate_Pause_Flag,0.628,-0.628); //��ת��Ӧ��PID ����Ƕ�Error�����ת��
        }
        else
        {
            Power_Select=Power_Wheel;
            Power_Manage(Power_Select,1);
        }





    //�����ٶȾ���
    Velocity_Distribution(&Wheel_Speed_Reference,&Velocity_Reference,&Angular_Reference,R_Theta,1);//Scale��PID���

/******************************************************************************/
    //���ӿ�������
    Wheel_Speed_Control(&Wheel_Speed_Reference,Measured_Wheel_RPM); //������ת�ٶȵ�PID����
                                                                    //����ƽ���ٶȵ�PID���� ���Կ��ǶԼ��ٶȼƽ��е�ͨ�˲� ���ʱ����������

/******************************************************************************/
    //����ͨѶ������

        //��ü�������
         if(Control_Mode=='M')
         {
             watch[0]=1;
         }
         else
         {
             watch[0]=2;
         }


         watch[1]=Wheel_Speed_Reference.FR_RPM;
         watch[2]=Measured_Wheel_RPM[2];
         watch[3]=*(R_Theta+2);
         watch[4]=IMU_DataSet.Angle_Z;

        for(i=0;i<4;i++)
        {
          *(Plot_Data+i)= *(watch+i);
        }
        Serial_Election=Serial3;//��������ѡ��
        Plot(Serial_Election,Plot_Data,4);//����ͨѶ��������

    }


}



/************************************************************/
//�жϺ����� ��������PWM��
void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast"))); //��ֹ����ֻ��һ���ж�
void TIM2_IRQHandler(void)
{
    if(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update)==1) //��ȡ�жϱ�־
    {
       Encoder_Get_Wheel_RPM(Measured_Wheel_RPM);
       Clear_Encoder();
       TIM_ClearFlag(TIM2, TIM_FLAG_Update);//����жϱ�־
    }
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

