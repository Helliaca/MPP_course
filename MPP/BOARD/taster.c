#include "taster.h"


//=========================================================================
// Anschlussbelegung
//=========================================================================
//	Taster1 (On/Off) - PC8 	liefert 1 wenn nicht gedrückt
//	Taster2 (Boot)	 - PC5	liefert 0 wenn nicht gedrückt
//=========================================================================

//=========================================================================
void init_TASTER1(void)
//=========================================================================
{

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	GPIO_StructInit(&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_Init(GPIOC, &GPIO_InitStructure);
}



//=========================================================================
void init_Taster1_IRQ8(void)
//=========================================================================
{

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource8);

	EXTI_InitTypeDef EXTI_InitStructure;

	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;

	EXTI_Init(&EXTI_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
}


int taste1_counter = 0;
int taste2_counter = 0;
char msg[50];
//=========================================================================
void TASTER1_IRQ(void)
//=========================================================================
{
	//A8-1-2
	tim7_counter = 0;
	usart_2_print("\r\nZeitmessung begonnen");
	//Ende A8-1-2

	/*
	 * taste1_counter wird bei jedem Drücken inkrementiert. Wurde die Taste 10 Mal betätigt,
	 * wird die EXTI_Line disbabled. Dies ist am taste1_counter = -1 erkennbar.
	 */
	/*
	taste1_counter++;
	if(taste1_counter>9) {
		EXTI_InitTypeDef EXTI_InitStructure;

		EXTI_InitStructure.EXTI_Line = EXTI_Line8;
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;

		EXTI_Init(&EXTI_InitStructure);

		taste1_counter = -1;
	}

	usart_2_print("==>Taster 1 gedrückt\r\n");
	sprintf(msg, "Taste2_counter: %d\r\nTaste1_counter: %d\r\n", taste2_counter, taste1_counter);
	usart_2_print(msg);
	*/

	/*
	usart2_send("==>Taster 1 gedrückt\r\n");
	beep(1000,200,0);
	*/
	//green_LED_ON;

	//	while(!(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_8)))
	// {
	// ;
	// }
}


//=========================================================================
void init_TASTER2(void)
//=========================================================================
{

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	GPIO_StructInit(&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_Init(GPIOC, &GPIO_InitStructure);

}



//=========================================================================
void init_Taster2_IRQ5(void)
//=========================================================================
{

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource5);

	EXTI_InitTypeDef EXTI_InitStructure;

	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;

	EXTI_Init(&EXTI_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

}


//=========================================================================
void TASTER2_IRQ(void)
//=========================================================================
{
	//A8-1-2
	char data[50] = {0};
	sprintf(data, "\r\nTIM7 COUNTER: %f s", ((float)tim7_counter)/100.0f);
	usart_2_print(data);
	tim7_counter = -1;
	//Ende A8-1-2

	//A07.01.01 Begin
	//green_LED_OFF;

	//A07.01.01 End


	/*
	 * taste2_counter wird nur inkrementiert, sobal die Line zu Taste 1 disabled ist.
	 * Erreicht taste2_counter ==2 werden beide counter resetted und die Line wieder neu initialisiert.
	 */
	/*
	if(taste1_counter == -1){
		taste2_counter++;
		if (taste2_counter > 1) {
			taste2_counter =0;
			taste1_counter =0;
			init_taste1_irq();
		}
	}

	usart_2_print("==>Taster 2 gedrückt\r\n");
	sprintf(msg, "Taste2_counter: %d\r\nTaste1_counter: %d\r\n", taste2_counter, taste1_counter);
	usart_2_print(msg);


	green_LED_OFF;
	 */
	//	beep(4000,200,200);
//	beep(4000,200,200);
//	beep(4000,200,200);
//	set_autostart_in(0,0,0,30);
//	while (GPIO_ReadInputData(GPIOC) & 0x0020){;}
}


