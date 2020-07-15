#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

GPIO_InitTypeDef GPIO_InitStruct;
EXTI_InitTypeDef EXTI_InitStruct;
NVIC_InitTypeDef NVIC_InitStruct;

void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

}

void EXTI_Config()
{
	//KESME AYARLARI(INTERRUPT)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1); //A portunun 1. ve 2. pinleri EXTI1 ve EXTI2 kesmeleri olarak ayarlandý
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource2);

	EXTI_InitStruct.EXTI_Line = EXTI_Line1 | EXTI_Line2; //Cunku 1. ve 2. pinler kullanildi.
	EXTI_InitStruct.EXTI_LineCmd = ENABLE; //hat aktif edildi
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);


	//NVIC AYARI(KESME KONTROLU)
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//Port Oncelik ayarlarý

	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn; //A portunun 1. pinin EXT1 oldugunu soyledik
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  //Hat aktif edildi
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0; //A portunun 1. pininin onceliði verildi
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; //A portunun 2. pininin ikinciliði verildi
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}

void delay(uint32_t time){
	while(time--);
}

void EXTI1_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET) //KESME BAYRAGI KONTROL EDILDI
	{
		GPIO_ResetBits(GPIOD, GPIO_Pin_All);
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		delay(21000000); //INTTERRUP ICINDE GECIKME YAPILMASI TAVSIYE EDILMEZ
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
		delay(21000000);
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
		delay(21000000);
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
		delay(21000000);

		EXTI_ClearITPendingBit(EXTI_Line1); //KESME BAYRAGI TEMIZLENDI
	}
}

void EXTI2_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
		GPIO_ResetBits(GPIOD, GPIO_Pin_All);
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
		delay(21000000);
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
		delay(21000000);
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
		delay(21000000);
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		delay(21000000);

		EXTI_ClearITPendingBit(EXTI_Line2);
	}
}

int main(void)
{
	GPIO_Config();
	EXTI_Config();

  while (1)
  {
	  GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
  }
}



void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
