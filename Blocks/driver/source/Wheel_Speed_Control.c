/*
 * Wheel_Speed_Control.c
 *
 *  Created on: Jun 24, 2023
 *      Author: Lost
 */

#include "Wheel_Speed_Control.h"
#include "PID_Controller.h"
#include "debug.h"
#include "DC_Motor.h"


void Wheel_Speed_Control(Wheel_Speed_Reference_Typedef* Wheel_Speed_Reference,float* Measured_Wheel_RPM)
{

    int i,j;
    static double Wheel_Speed_Error[4][3],flag=1; //��BL=[0] FL=[1] FR=[2] BR=[3] ��Ϊʱ���ߵĴ���
    static double Output[4];//��BL=[0] FL=[1] FR=[2] BR=[3] ��Ϊʱ���ߵĴ���
    Wheel_Speed_Typdef Wheel_Speed;

    /*
     * static����Error ��ֹError���ݶ�ʧ �����ϴκ����ϴε�error 4����4������ 3������ �ϴ� ��ε�error error[0]���ϴ�  error[1]�ϴ�   error[2]����
     * BL=[0] FL=[1] FR=[2] BR=[3]
     */

    if(flag)
    {
        for(i=0;i<4;i++)
        {
            for(j=0;j<3;j++)
            {
                Wheel_Speed_Error[i][j]=0;
            }
        }
       Wheel_Speed.BL_Speed=0;
       Wheel_Speed.FL_Speed=0;
       Wheel_Speed.BR_Speed=0;
       Wheel_Speed.FR_Speed=0;
    }flag=0;

    PID_Parameter Wheel_PID;
        //PID������ֵ
    Wheel_PID.Kp=0.409267124576902*4;
    Wheel_PID.Ki=0.0208088878485*4;
    Wheel_PID.Kd=-0.01851624170342353*4;

    //����Error
    for(i=0;i<4;i++)
    {
        for(j=0;j<2;j++)
        {
          Wheel_Speed_Error[i][j]= Wheel_Speed_Error[i][j+1]; //����
        }
        Wheel_Speed_Error[i][2]=*((float*)&(Wheel_Speed_Reference->BL_RPM)+i)-*(Measured_Wheel_RPM+i);
        //���赱ǰ��� *((float*)&(Wheel_Speed_Reference->BL_RPM)+1)תΪfloat�������η���
    }


    Wheel_PID_Controller(&Wheel_PID,Wheel_Speed_Error,Output,400,-400);

    Wheel_Speed.FR_Speed=Output[2];
    Wheel_Speed.BR_Speed=Output[3];
    Wheel_Speed.BL_Speed=Output[0];
    Wheel_Speed.FL_Speed=Output[1];

   //��ֵ��Wheel
   SetMotor_Speed(&Wheel_Speed);


}



