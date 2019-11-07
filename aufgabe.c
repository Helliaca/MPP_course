#include "aufgabe.h"

//A01-01.2
void init_leds(void) {
	//GPIO_DeInit(GPIOB);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed =	GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType =	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOB, &GPIO_InitStructure);

	green_LED_OFF;
}

//A01-02.02
void init_tasten() {

	//GPIO_DeInit(GPIOC);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_8;

	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN;       //GPIO Input Mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;   //Medium speed
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //PushPull
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;   //PullUp

	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
//A01-02.03

uint8_t taste1_was_pressed = 0;
uint8_t taste1_down = 0;

uint8_t taste2_was_pressed = 0;
uint8_t taste2_down = 0;

uint8_t taste1_pressed() {

	uint8_t tmp = !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8);
	if(tmp<taste1_was_pressed) taste1_down = 1;
	taste1_was_pressed = tmp;
	return taste1_was_pressed;

}

uint8_t taste1_downed() {
	uint8_t tmp = taste1_pressed();
	tmp = taste1_down;
	taste1_down = 0;
	return tmp;
}

uint8_t taste2_pressed() {
	uint8_t tmp = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5);
	if(tmp<taste2_was_pressed) taste2_down = 1;
	taste2_was_pressed = tmp;
	return taste2_was_pressed;

}

uint8_t taste2_downed() {
	uint8_t tmp = taste2_pressed();
	tmp = taste2_down;
	taste2_down = 0;
	return tmp;
}

// A02-01.2 + A02-01.3
void init_PC09(void) {


	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

	  GPIO_InitTypeDef GPIO_InitStructure;
	  GPIO_StructInit(&GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;

	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	  GPIO_Init(GPIOC, &GPIO_InitStructure);

	  /* GPIO Modus Alternativ-Funktion erlaubt die direkte Ausgabe einer onboard
	   * Peripheral. I/O Pin Multiplexer erlaubt aus einer Vielzahl der onboard
	   * Peripherals das Gewuenschte auszuwaehlen, nur eines erlaubt. MCO ist ein
	   * Makro fuer AF0, welches an systeminterne Tektquellen angebunden ist (z.B.
	   * HSE, LSE, HSI, PLL). MC02 erlaubt die Ausgabe einer Taktquelle über den
	   * Pin C09. Die SystemClock gibt SysTicks (8-fache des CPU-Taktes) an. Zu-
	   * sätzlich soll die Ticks nicht prescaled werden, daher RCC_MCO2Div_1, welches
	   * eines Division durch 1 entspricht.
	   */
	  GPIO_PinAFConfig(GPIOC,GPIO_Pin_9,GPIO_AF_MCO);
	  RCC_MCO2Config(RCC_MCO2Source_SYSCLK,RCC_MCO2Div_1);

	  /* ohne SysInit(): f = 16 MHz und I = (30 +/-1) mA
	   * mit SysInit() : f = 168MHz und I = (72 +/-1) mA
	   */
}

//A02-01.4
/* mit PINC09
 * fastMode(): f = 168 MHz und I = (63 +/-1) mA
 * slowMode(): f =  24 MHz und I = (29 +/-1) mA
 *
 * ohne PINC09
 * fastMode(): I = (57 +/-1) mA
 * slowMode(): I = (26 +/-1) mA
 */


// PLLStartUp Hilfsfunktion
void RCC_WaitForPLLStartUp(void) {
    while ( (RCC->CR & RCC_CR_PLLRDY) == 0 ) {
        __NOP();
    }
}
//==== Taktfrequenz 16MHz ohne HSE-OSC
void defaultMode() {
    RCC_DeInit();
}

//==== Taktfrequenz 24MHz mit HSE-OSC=16MHz
void slowMode(void) {
    RCC_DeInit();

    RCC_HSEConfig(RCC_HSE_ON);
    if (RCC_WaitForHSEStartUp() == ERROR) {
        return;
    }
    // HSEOSC=16MHz SYSCLK=24MHz HCLK=24MHz
    // PCLK1=24 PCLK2=24MHz
    RCC_PLLConfig(RCC_PLLSource_HSE,    //RCC_PLLSource
                                16,     //PLLM
                                192,    //PLLN
                                8,      //PLLP
                                4       //PLLQ
                                );
    RCC_PLLCmd(ENABLE);
    RCC_WaitForPLLStartUp();

    // Configures the AHB clock (HCLK)
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    // Low Speed APB clock (PCLK1)
    RCC_PCLK1Config(RCC_HCLK_Div1);
    // High Speed APB clock (PCLK2)
    RCC_PCLK2Config(RCC_HCLK_Div1);

    // select system clock source
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
}


//==== Taktfrequenz 168MHz mit HSE-OSC=16MHz
void fastMode(void) {
    RCC_DeInit();

    RCC_HSEConfig(RCC_HSE_ON);
    if (RCC_WaitForHSEStartUp() == ERROR) {
        return;
    }
    // HSEOSC=16MHz SYSCLK=168MHz HCLK=168MHz
    // PCLK1=42MHz PCLK2=84MHz
    RCC_PLLConfig(RCC_PLLSource_HSE,    //RCC_PLLSource
                                16,     //PLLM
                                336,    //PLLN
                                2,      //PLLP
                                7       //PLLQ
                                );
    RCC_PLLCmd(ENABLE);
    RCC_WaitForPLLStartUp();

    // Configures the AHB clock (HCLK)
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    // High Speed APB clock (PCLK1)
    RCC_PCLK1Config(RCC_HCLK_Div4);
    // High Speed APB clock (PCLK2)
    RCC_PCLK2Config(RCC_HCLK_Div2);


    // select system clock source
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
}

//A03-01.1
//==== USART2 konfigurieren
void init_usart_2_tx(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// Taktsystem für die USART2 freigeben
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);


	// GPIO Port A Taktsystem freigeben
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// USART2 TX an PA2 mit Alternativfunktion Konfigurieren
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// USART2 TX mit PA2 verbinden
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);

	// Datenprotokoll der USART einstellen
	/*
	 * Hardware Flow Control nur, wenn mit einem Emfpänger aktiv kommuniziert wird über CTS (Clear to send)
	 * RFR (Ready for Receiving, fälschlicherweise als RTS) Leitungen, die jede USART Unit bereit stellt.
	 * Tx Transmission, Rx Receiving. USART muss mit PIN verbunden werden.
	 */
	USART_InitStructure.USART_BaudRate 				= 921600;
	USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;
	USART_InitStructure.USART_StopBits 				= USART_StopBits_1;
	USART_InitStructure.USART_Parity 				= USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode 					= USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	// USART2 freigeben
	USART_Cmd(USART2, ENABLE); // enable USART2
}

//A03-01.2
void usart_2_print(char * str_print) {
	int i = 0;
	for(i = 0;i < strlen(str_print);i++)
	{
		USART_SendData(USART2, str_print[i]);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){}
	}
}

//A03-01.3
void init_usart_2(void) {
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;

		// Taktsystem für die USART2 freigeben
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

		// GPIO Port A Taktsystem freigeben
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

		// USART2 TX an PA02 für Send und PA03 für receive mit Alternativfunktion Konfigurieren
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		// USART2 TX mit PA2 verbinden
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
		// Datenprotokoll der USART einstellen
		/*
		 * Hardware Flow Control nur, wenn mit einem Emfpänger aktiv kommuniziert wird über CTS (Clear to send)
		 * RFR (Ready for Receiving, fälschlicherweise als RTS) Leitungen, die jede USART Unit bereit stellt.
		 * Tx Transmission, Rx Receiving. USART muss mit PIN verbunden werden.
		 */
		USART_InitStructure.USART_BaudRate 				= 921600;
		USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;
		USART_InitStructure.USART_StopBits 				= USART_StopBits_1;
		USART_InitStructure.USART_Parity 				= USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode 					= USART_Mode_Tx | USART_Mode_Rx;
		USART_Init(USART2, &USART_InitStructure);

		//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		//NVIC_EnableIRQ(USART2_IRQn);

		// USART2 freigeben
		USART_Cmd(USART2, ENABLE); // enable USART2

}




