/*
 * Data_Central_Process.h
 *
 *  Created on: Jun 29, 2023
 *      Author: Lost
 */

#ifndef BLOCKS_DRIVER_HEADER_DATA_CENTRAL_PROCESS_H_
#define BLOCKS_DRIVER_HEADER_DATA_CENTRAL_PROCESS_H_
#include "debug.h"
#include "Wheel_Speed_Control.h"
#include "Camera.h"
#include "IMU_Driver.h"

//�����б� ��������� ���г��ȵ�λΪmm �Ƕȵ�λΪ�� �ٶ�Ϊmm/s ʱ�䵥λΪms

//!!!!!!!!!!!!!!��װ����Ҫ����
#define Camera_Height 20
#define Platform_Length 260
#define Maximum_Velocity 70
#define Distance_Threshold 100  //������ֵ �����Ƿ����Pickģʽ
#define Pick_Duration 6000
#define Pick_Velocity 70

#define Rx 150
#define Ry 100
#define R 75


typedef struct{double Vx;double Vy;double Wz;double Orient;} Car_State_Tpyedef;

float Find_Max(float* Data,uint8_t Size);
void Gain_R_Theta(Camera_DataSet_Typedef* Camara_DataSet,float* R_Theta);
//void Gain_Reference(float* R_Theta,uint32_t* Stage_Flag,float* Velocity_Reference, float* Angular_Reference,uint8_t* Event_Flag);
void State_Resolve(IMU_DataSetTypedef* IMUDataSet,Car_State_Tpyedef* Car_State,uint64_t Real_Time);
void Velocity_Distribution(Wheel_Speed_Reference_Typedef* Wheel_Speed_Reference,float* Velocity_Reference,float* Angular_Reference,float* R_Theta,float Scale);
void Manual_Control(float* Velocity_Reference,float* R_Theta);
void Camera_Control(float* R_Theta,float* Velocity_Reference,float Real_Time_s,float Real_Time_ms,IMU_DataSetTypedef* IMU_DataSet,Camera_DataSet_Typedef* Camara_DataSet,uint8_t* Spin_Flag,uint8_t* Rotate_Pause_Flag);
#endif /* BLOCKS_DRIVER_HEADER_DATA_CENTRAL_PROCESS_H_ */
