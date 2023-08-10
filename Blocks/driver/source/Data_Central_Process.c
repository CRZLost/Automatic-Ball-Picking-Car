/*
 * Data_Central_Process.c
 *
 *  Created on: Jun 29, 2023
 *      Author: Lost
 */

#include "Camera.h"
#include "math.h"
#include "debug.h"
#include "IMU_Driver.h"
#include "Data_Central_Process.h"
#include "Wheel_Speed_Control.h"
#include "Power.h"

extern uint8_t Received_Data[8];  //����������ָ������
extern float watch[8];


//��������ͷ���������ݽ�����ƽ���r theta
void Gain_R_Theta(Camera_DataSet_Typedef* Camara_DataSet,float* R_Theta)
{
        // +0ΪR +1ΪTheta
     *R_Theta=sqrtf(pow(Platform_Length,2)+pow(Camara_DataSet->Distance,2)-pow(Camera_Height,2));
     *(R_Theta+1)=Camara_DataSet->Theta-90; //ƽ�Ƴ���
     *(R_Theta+2)=Camara_DataSet->Theta-90; //С������
}






//void Gain_Reference(float* R_Theta,uint32_t* Stage_Flag,float* Velocity_Reference,float* Angular_Reference,uint8_t* Event_Flag)
//{
//    static uint8_t V_Flag,W_Flag,TimePin=0,TimePin2=0;
//    double Distance_Ratio,Angular_Ratio;
//    static uint8_t Wait_Flag=0,Pick_Flag=0;
//    static double Original_Distance,Original_Theta; //��ʼ�ľ��� �Ƕ�
//    static double Current_Time;
//
//    Current_Time=Real_Time*1000+Real_Time_ms;
//
//    //���������׶ζ�V��W��flag�ֱ�ֵ   flag=1�ɺ������� flag=0ֱ�Ӹ�ֵ����
//    switch (*Stage_Flag)
//    {
//        case 1: //Found�׶� ���׶�С��ֹͣ�κζ���2s�Ի������ͷ�ȶ������� ����ͷ��Ȩ��
//            V_Flag=0;W_Flag=0;//�жϺ������
//            Wait_Flag+=1;//�ȴ���־
//            *Velocity_Reference=0; *Angular_Reference=0;//ֱ�Ӹ�ֵ
//            break;
//
//        case 2://Approach�׶� ���׶�С��ֱ�߿���ƹ���� ������ת���� ����ͷ��Ȩ��
//            V_Flag=1;W_Flag=0;
//            break;
//
//        case 3://Spin�׶� ���׶�С��������ת����С�� ����ͷ��Ȩ��
//            V_Flag=0;W_Flag=1;
//                 break;
//
//        case 4://Pick�׶� ���׶�С������С���ʰ���� ����ͷ��Ȩ��
//            V_Flag=0;W_Flag=0;//�жϺ������
//            Pick_Flag+=1;//�ȴ���־
//            *Velocity_Reference=Pick_Velocity; *Angular_Reference=0;//ֱ�Ӹ�ֵ
//                 break;
//
//        case 5://Search�׶� ���׶�С��ԭ����תѰ����һ��С��  ����ͷ��Ȩ��
//            V_Flag=0;W_Flag=0;//�жϺ������
//            *Velocity_Reference=0;*Angular_Reference=Search_Angula_Speed;
//                 break;
//
//        case 6://Stop�׶�С��ԭ�ؾ���
//          V_Flag=0;W_Flag=0;//�жϺ�������
//          *Velocity_Reference=0; *Angular_Reference=0;//ֱ�Ӹ�ֵ
//               break;
//
//        default:
//            break;
//    }
//
//
//    //����Found->Approach
//    if(Wait_Flag==1)//������ڵȴ����� �����ʱ��RealTime ����һ�δ���found״̬ʱ��¼ʱ��
//    {
//        TimePin=Real_Time*1000+Real_Time_ms;
//    }
//    if((Current_Time-TimePin)>2000&&*Stage_Flag==1) //���Found�׶εȴ�2s�� ����Approach�׶� ���waitCount ͬʱ��¼��ʼ����ͳ�ʼ�Ƕ�
//    {
//        *Stage_Flag=2;
//        TimePin=0;
//        Original_Distance=*R_Theta;
//        Original_Theta=*(R_Theta+1); //ƽ�ƽǶ�
//        Wait_Flag=0;
//    }
//


//
//    //����滮
//    Distance_Ratio=*(R_Theta)/(Original_Distance-Distance_Threshold);
//    if(Distance_Ratio<0.1)
//    {
//        *Velocity_Reference=-Maximum_Velocity*(Distance_Ratio)*(Distance_Ratio-0.1)*V_Flag;
//    }
//    if(0.1<Distance_Ratio&&Distance_Ratio<0.9)
//    {
//        *Velocity_Reference=Maximum_Velocity*V_Flag;
//    }
//    if(Distance_Ratio>0.9)
//    {
//        *Velocity_Reference=-Maximum_Velocity*(Distance_Ratio-0.9)*(Distance_Ratio-1)*V_Flag;
//    }
//
//    if(Distance_Ratio>1&&*Stage_Flag==2) //����&�ϸ�Stage�ı�־ ȷ���Ǵ���һ���׶ε���
//    {
//        *Stage_Flag=3; //Approach->Spin !!!!!!!�˴������һ������ͣ��
//    }
//
//
//
//    //�Ƕȹ滮
//    Angular_Ratio=(*(R_Theta+1)-90)/(Original_Theta-90);
//    if(Angular_Ratio<0.1)
//    {
//        *Angular_Reference=-Maximum_Angular*(Angular_Ratio)*(Angular_Ratio-0.1)*W_Flag;
//    }
//    if(Angular_Ratio>0.1&&Angular_Ratio<0.9)
//    {
//       *Angular_Reference=Maximum_Angular*W_Flag;
//    }
//    if(Angular_Ratio>0.9)
//    {
//        *Angular_Reference=-Maximum_Angular*(Angular_Ratio-0.9)*(Angular_Ratio-1)*W_Flag;
//    }
//    //������ת����
//    if((Original_Theta-90)<0&&W_Flag)
//    {
//        *Angular_Reference=-*Angular_Reference;
//    }
//
//    if(Angular_Ratio>1&&*Stage_Flag==3)
//    {
//       *Stage_Flag=4; //Spin->Pick
//    }
//
//
//    if(Pick_Flag==1)//������ڵȴ����� �����ʱ��RealTime ����һ�δ���Pick״̬ʱ��¼ʱ��
//    {
//          TimePin2=Real_Time;
//    }
//    if((Real_Time-TimePin2)>Pick_Duration&&*Stage_Flag==4)
//    {
//       *Stage_Flag=5; //Pick->Search
//        TimePin2=0;
//        Pick_Flag=0;
//        *Event_Flag=0; //���̱�ִ�����
//    }
//
//}


void State_Resolve(IMU_DataSetTypedef* IMUDataSet,Car_State_Tpyedef* Car_State,uint64_t Real_Time)
{
    uint8_t i;
    static float Vx_Increment[10],Vy_Increment[10],Angular_Speed[10],Orient[10],Time_Increment;
    static float Vx_Sum,Vy_Sum,Angular_Speed_Sum,Orient_Sum;
    static uint64_t Last_Monment=0;

    //ȡƽ����������
    for(i=0;i<8;i++)
    {
        Vx_Increment[i]=Vx_Increment[i+1];
        Vy_Increment[i]=Vy_Increment[i+1];
        Angular_Speed[i]=Angular_Speed[i+1];
        Orient[i]=Orient[i+1];
        Vx_Sum+=Vx_Increment[i];
        Vy_Sum+=Vy_Increment[i];
        Orient_Sum+=Orient[i];
        Angular_Speed_Sum+=Angular_Speed[i];
    }

    Time_Increment=Real_Time-Last_Monment; //���ʱ����

    Vx_Increment[9]=IMUDataSet->Accerelation_X*Time_Increment;
    Vy_Increment[9]=IMUDataSet->Accerelation_Y*Time_Increment;
    Angular_Speed[9]=IMUDataSet->Angular_Speed_Z;
    Orient[9]=IMUDataSet->Angle_Z;


    Car_State->Vx+=(Vx_Sum+Vx_Increment[9])/10;
    Car_State->Vy+=(Vy_Sum+Vy_Increment[9])/10;
    Car_State->Wz+=(Angular_Speed_Sum+Angular_Speed[9])/10;
    Car_State->Orient=(Orient_Sum+Orient[9])/10;

    //sum����
    Vx_Sum=0;
    Vy_Sum=0;
    Orient_Sum=0;
    Angular_Speed_Sum=0;

    Last_Monment=Real_Time;//����ռ�

}

void Velocity_Distribution(Wheel_Speed_Reference_Typedef* Wheel_Speed_Reference,float* Velocity_Reference,float* Angular_Reference,float* R_Theta,float Scale)
{
    float W_Vector[4];
    float Vx=-*Velocity_Reference*(sin(0.017453**(R_Theta+1)));
    float Vy=*Velocity_Reference*(cos(0.017453**(R_Theta+1)));
    float W=*Angular_Reference;

    //���󷴽� *60/R �Ǵ�mm/sתΪrpm
     W_Vector[0]=(-Vx+Vy-(Rx+Ry)*W)/R*60;
     W_Vector[1]=(Vx+Vy-(Rx+Ry)*W)/R*60;
     W_Vector[2]=(-Vx+Vy+(Rx+Ry)*W)/R*60;
     W_Vector[3]=(Vx+Vy+(Rx+Ry)*W)/R*60;

    //Scale������PID����
    Wheel_Speed_Reference->FL_RPM=-Scale*W_Vector[0];
    Wheel_Speed_Reference->BL_RPM=-Scale*W_Vector[1]; //ע���������ε������෴
    Wheel_Speed_Reference->BR_RPM=Scale*W_Vector[2];
    Wheel_Speed_Reference->FR_RPM=Scale*W_Vector[3];

}


void Manual_Control(float* Velocity_Reference,float* R_Theta)
{
    float Vx=0,Vy=0;
    float Judge=*(float*)(Received_Data);

    if(Judge==1500)//ȷ��Ϊ����ֵ
    {
        *(R_Theta+2)=-*((float*)Received_Data+1); //����ֵ
    }
    else //Ϊ�ٶȸ�ֵ
    {
         Vx=*(float*)(Received_Data);///1000*Maximum_Velocity
         Vy=*((float*)Received_Data+1);///1000*Maximum_Velocity
         if(Vx>400||Vy>400||Vx<-400||Vy<-400) //�����л�ģʽʱ��ͻ��
         {
             Vx=0;
             Vy=0;
         }
         *(R_Theta+1)=57.296*atan(Vx/Vy); //�����һ��֮ǰͬһ�á�
         if(Vy==0&&(Vx>0|Vx==0))
         {
             *(R_Theta+1)=0;
         }
         if(Vy==0&&Vx<0)
         {
             *(R_Theta+1)=180;
         }
         if(Vy<0)
         {
             *(R_Theta+1)+=180; //������ȡ��
         }
         *Velocity_Reference=sqrt(Vx*Vx+Vy*Vy);
    }
}


/*
 *
 * ����Found------>Search Search->Found�Ľ׶ο���
 * R_Theta[0] ֱ�߾���    R_Theta[1] ƽ����Y��н�   R_Theta[2] ����
 * Stage_Flag�����׶α�־
 * Velocity_Reference ƽ���ٶȵ�����ֵ
 * Angular_Reference ��ת�ٶȵ�����ֵ
 * Event_Flag ����С���¼���־λ
*/
void Camera_Control(float* R_Theta,float* Velocity_Reference,float Real_Time_s,float Real_Time_ms,IMU_DataSetTypedef* IMU_DataSet,Camera_DataSet_Typedef* Camara_DataSet,uint8_t* Spin_Flag,uint8_t* Rotate_Pause_Flag)
{

    uint8_t i;
    static uint8_t Stage_Flag=1,Wait_Flag=0;; //��ʼΪSearch�׶�
    static uint8_t Event_Flag=0,Found_Count=0,Lost_Count=0;
    static uint64_t Current_Time,Time_Pin;
    static uint8_t Exist_Check=0; //ʱ�̼����ڵı�־
    static double Original_Distance,Distance_Ratio;
    static float Distance_Store={0};
    static float Original_Distance_Store[40]={0};
    Power_Manage_Typedef Power_Select;

    Current_Time=Real_Time_ms+Real_Time_s*1000; //��ǰʱ��

/**********************************************************/
    //���ϻ�ȡ��ǰ����ͷ������λ����Ϣ
    if(Camara_DataSet->Distance>0)
    {
        *R_Theta=fabsf(sqrtf(pow(Camara_DataSet->Distance/10,2)+pow(Camera_Height,2)-pow(Platform_Length,2)));
        if(*(R_Theta)==0||*(R_Theta)>10000||*(R_Theta)<0)
        {
            *R_Theta=Distance_Store;
        }
        else
        {
            Distance_Store=*R_Theta;
        }
    }
    else
    {
        *R_Theta=Distance_Store;
    }

/**********************************************************/
    //�ж��Ƿ�Ҫ��ʼһ������
    if(Event_Flag==0)
    {
        Stage_Flag=1; //δ�ҵ�ʱΪSearch�׶�
        if(Camara_DataSet->Distance>0&&Found_Count<3)
        {
            Found_Count++;
        }

        if(Found_Count>2)
        {
            Found_Count=0;Stage_Flag=2;*Rotate_Pause_Flag=1; //������һ�׶�Found�׶� ����һ������
            Event_Flag=1;//����һ������ ��ʱ��1 ȫ�������������
            Exist_Check=1;
        }
    }


    if(Camara_DataSet->Distance<0&&Stage_Flag<4) //Stage=4��ر�����ͷ�ļ�鹦��
    {
        Lost_Count++;
        if(Lost_Count>20)
        {
            Exist_Check=0;
            Event_Flag=0; //������ֹ
            Lost_Count=0;
        }

    }

    if(Camara_DataSet->Distance>0&&Lost_Count<19) //��Ѱ�ػ���
    {
        Lost_Count=0;
    }

/**********************************************************/
    //�����׶εĿ������

    if(Stage_Flag==1) //Search�׶�
    {
        *Spin_Flag=1; //��ʼ����ת
        *Rotate_Pause_Flag=0;
        *(R_Theta+1)=0;
        *Velocity_Reference=0;
    }

    if(Stage_Flag==2) //Found�׶�
    {
        *Spin_Flag=0;
        *(R_Theta+1)=0;
        *Velocity_Reference=0;

        if(Wait_Flag==0)
        {
            Wait_Flag=1;
            Time_Pin=Current_Time;
        }

        if(Wait_Flag==1&&Stage_Flag==2)
        {
            for(i=0;i<39;i++)
            {
               Original_Distance_Store[i]=Original_Distance_Store[i+1]; //����ȴ��ڼ��Distance
            }
            Original_Distance_Store[39]=*(R_Theta);

        }


        if((Current_Time-Time_Pin)>2000&&Wait_Flag==1&&Stage_Flag==2) //ʱ�����ڼ�¼����
        {
            if(Camara_DataSet->Distance>0)
            {
                Original_Distance=Find_Max(Original_Distance_Store,40);//��¼��ʼ����
            }

            for(i=0;i<40;i++)
            {
                Original_Distance_Store[i]=0; //�������
            }

            *Rotate_Pause_Flag=0; //������ת
            Wait_Flag=0;  //ֹͣ�ȴ��׶�
            Stage_Flag=3; //����Approach�׶�
        }

    }



//    if(Stage_Flag==3&&Event_Flag==1) //������ĳ���
//    {
//        *(R_Theta+2)=IMU_DataSet->Angle_Z+Camara_DataSet->Theta-90; //����ͷ������ת
//    }



    //Approach �׶�
    if(Stage_Flag==3)
    {


        //����滮
       Distance_Ratio=(Original_Distance-*(R_Theta))/(Original_Distance-Distance_Threshold);

       if(Distance_Ratio<0.2)
       {
           *Velocity_Reference=-Maximum_Velocity*(Distance_Ratio-0.2)*(Distance_Ratio)+20;
           *(R_Theta+2)=IMU_DataSet->Angle_Z+Camara_DataSet->Theta-90; //��ʼ�Ƕ�ֵ
       }


       if(0.2<Distance_Ratio&&Distance_Ratio<0.9)
       {
           *Velocity_Reference=Maximum_Velocity;
           *(R_Theta+2)=IMU_DataSet->Angle_Z+Camara_DataSet->Theta-90; //����ͷ������ת
//           *(Rotate_Pause_Flag)=0;
       }


       if(Distance_Ratio>0.9)
       {
           *Velocity_Reference=-Maximum_Velocity*(Distance_Ratio-0.9)*(Distance_Ratio-1)+40;
           *(R_Theta+2)=IMU_DataSet->Angle_Z+Camara_DataSet->Theta-90; //����ͷ������ת
       }

       if(Distance_Ratio>1&&Stage_Flag==3&&Wait_Flag==0) //����&�ϸ�Stage�ı�־ ȷ���Ǵ���һ���׶ε���
       {
         Wait_Flag=1;
         Time_Pin=Current_Time;
       }

       if(Wait_Flag==1&&Stage_Flag==3)
       {
           *Velocity_Reference=0; //ͣ��
       }
       if(Wait_Flag==1&&(Current_Time-Time_Pin)>2000&&Stage_Flag==3)
       {
           Wait_Flag=0;
           Stage_Flag=4;//�����½׶�Pick�׶�
           *Rotate_Pause_Flag=1; //��ͣ��ת����
       }
    }

    //Pick�׶�
    if(Stage_Flag==4)
    {

        if(Wait_Flag==0)
        {
            //��ʼ��ǰ��
            *Rotate_Pause_Flag=0;
            Time_Pin=Current_Time; //���ʱ��
            Wait_Flag=1;
            *Velocity_Reference=Pick_Velocity;
            *(R_Theta+1)=0;
            Power_Select=Power_Motor;
            Power_Manage(Power_Select,1);

        }

        if(Wait_Flag==1&&Current_Time-Time_Pin>Pick_Duration)
        {
            Power_Select=Power_Motor;
            Power_Manage(Power_Select,0);
            Wait_Flag=0;
            Event_Flag=0; //����¼���־ ��ʼ��һ��ѭ��
        }
    }

}



float Find_Max(float* Data,uint8_t Size)
{
    uint8_t i;
    float MaxValue=0;

    for(i=0;i<Size;i++)
    {
        if(MaxValue<*(Data+i))
        {
            MaxValue=*(Data+i);
        }
    }

    return MaxValue;
}








