/*
 * GPIO_Init.c
 *
 *  Created on: Jun 21, 2023
 *      Author: Lost
 */

#include "debug.h"

void GPIOA_Init(uint16_t Pin,GPIOMode_TypeDef Mode) //���� ���� ģʽ
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //ѡ���ʱ��

    GPIO_InitTypeDef GPIO_InitStruture;

    GPIO_InitStruture.GPIO_Mode=Mode; //ѡ��ģʽ
    GPIO_InitStruture.GPIO_Pin=Pin; //ѡ������
    GPIO_InitStruture.GPIO_Speed=GPIO_Speed_50MHz; //ѡ��GPIO���ٶ�

    GPIO_Init(GPIOA,&GPIO_InitStruture);//����GPIOA

}

void GPIOB_Init(uint16_t Pin,GPIOMode_TypeDef Mode)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //ѡ���ʱ��

    GPIO_InitTypeDef GPIO_InitStruture;

    GPIO_InitStruture.GPIO_Mode=Mode; //ѡ��ģʽ
    GPIO_InitStruture.GPIO_Pin=Pin; //ѡ������
    GPIO_InitStruture.GPIO_Speed=GPIO_Speed_50MHz; //ѡ��GPIO���ٶ�

    GPIO_Init(GPIOB,&GPIO_InitStruture);//����GPIOA
}

void GPIOC_Init(uint16_t Pin,GPIOMode_TypeDef Mode)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); //ѡ���ʱ��

    GPIO_InitTypeDef GPIO_InitStruture;

    GPIO_InitStruture.GPIO_Mode=Mode; //ѡ��ģʽ
    GPIO_InitStruture.GPIO_Pin=Pin; //ѡ������
    GPIO_InitStruture.GPIO_Speed=GPIO_Speed_50MHz; //ѡ��GPIO���ٶ�

    GPIO_Init(GPIOC,&GPIO_InitStruture);//����GPIOA
}


void GPIOD_Init(uint16_t Pin,GPIOMode_TypeDef Mode)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE); //ѡ���ʱ��

    GPIO_InitTypeDef GPIO_InitStruture;

    GPIO_InitStruture.GPIO_Mode=Mode; //ѡ��ģʽ
    GPIO_InitStruture.GPIO_Pin=Pin; //ѡ������
    GPIO_InitStruture.GPIO_Speed=GPIO_Speed_50MHz; //ѡ��GPIO���ٶ�

    GPIO_Init(GPIOD,&GPIO_InitStruture);//����GPIOA
}


void GPIOE_Init(uint16_t Pin,GPIOMode_TypeDef Mode)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE); //ѡ���ʱ��

    GPIO_InitTypeDef GPIO_InitStruture;

    GPIO_InitStruture.GPIO_Mode=Mode; //ѡ��ģʽ
    GPIO_InitStruture.GPIO_Pin=Pin; //ѡ������
    GPIO_InitStruture.GPIO_Speed=GPIO_Speed_50MHz; //ѡ��GPIO���ٶ�

    GPIO_Init(GPIOE,&GPIO_InitStruture);//����GPIOA
}
