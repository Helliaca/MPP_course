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
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;   //Medium speed
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

//A04-01.1
void init_iwdg(void) {
	// Schreibrechte aktivieren
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	// den Vorteiler (4, 8 , 16 ,..., 256) setzen
	IWDG_SetPrescaler(IWDG_Prescaler_64);
	// den Wert (0...4095) einstellen ab dem runtergezählt wird
	IWDG_SetReload(2500);
	// setzt den Wachdog auf den eingestellten Maximalwert
	IWDG_ReloadCounter();
	// aktiviert dem IWDG
	IWDG_Enable();
	// Das Zeitintervall t berechnet sich folgendermaßen
	// t = (1/32000) * 64 * 2500 = 5,30 Sekunden
}


//A05-01.01
void init_taste1_irq() {
	// vor Aufruf der Funktion gilt es init_tasten() aufzurufen
	// Taste 1 liegt am PC8 Pin (gedrückt-0,nicht gedrückt-1)

	/*
	 * nicht gedrückt =1 (High)-> gedrückt =0 (Low) entspricht einer HL-Flanke als Trigger Bedingung
	 */

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	//==========================================================
	//========= Interrupt Konfiguration
	//==========================================================
	// Bindet Port C Leitung 8 an die EXTI_Line8 Leitung
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource8);

	// Struct anlegen
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;					// EXTI_Line zweisen
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			// Interrupt Mode setzen
	/* A wake-up event is simply a peripheral interrupt line that is not enabled as an interrupt within the NVIC. This allows a peripheral to signal the Cortex core to wake up and continue processing without  the  need  for  an  interrupt  service  routine. */
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		// Triggerbedingung setzen: HL-Flanke
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					// Interrupt erlauben

	// Regiser aus dem Struct heraus setzen
	EXTI_Init(&EXTI_InitStructure);


	//==========================================================
	//========= Interruptcontroller Konfiguration
	//==========================================================

	// Anlegen eines NVIC Struct
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			// Festlegung der Interruptquelle

	// Festlegung der Priorität entweder in 5 Gruppen
	//======================================================
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);				// NVIC_PriorityGroup_0 hohe Priorität - NVIC_PriorityGroup_4 niedrige Priorität
	//======================================================

	// oder feiner gegliedert in Priorität und Subpriorität
	//======================================================
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	// 0..15
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			// 0..15
	//======================================================

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				// Interruptkanal Freigabe

	// Register aus dem Struct heraus schreiben
	NVIC_Init(&NVIC_InitStructure);
}

void init_taste2_irq() {
	// vor Aufruf der Funktion gilt es init_tasten() aufzurufen
	// Taste 2 liegt am PC5 Pin (gedrückt-1,nicht gedrückt-0)

	/*
	 * nicht gedrückt =0 (LOW)-> gedrückt =1 (HIGH) entspricht einer LH-Flanke als Trigger Bedingung
	 */

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	//==========================================================
	//========= Interrupt Konfiguration
	//==========================================================
	// Bindet Port C Leitung 5 an die EXTI_Line8 Leitung
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource5);

	// Struct anlegen
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;					// EXTI_Line zweisen
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			// Interrupt Mode setzen
	/* A wake-up event is simply a peripheral interrupt line that is not enabled as an interrupt within the NVIC. This allows a peripheral to signal the Cortex core to wake up and continue processing without  the  need  for  an  interrupt  service  routine. */
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;		// Triggerbedingung setzen: HL-Flanke
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					// Interrupt erlauben

	// Regiser aus dem Struct heraus setzen
	EXTI_Init(&EXTI_InitStructure);


	//==========================================================
	//========= Interruptcontroller Konfiguration
	//==========================================================

	// Anlegen eines NVIC Struct
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			// Festlegung der Interruptquelle

	// Festlegung der Priorität entweder in 5 Gruppen
	//======================================================
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);				// NVIC_PriorityGroup_0 hohe Priorität - NVIC_PriorityGroup_4 niedrige Priorität
	//======================================================

	// oder feiner gegliedert in Priorität und Subpriorität
	//======================================================
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	// 0..15
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			// 0..15
	//======================================================

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				// Interruptkanal Freigabe

	// Register aus dem Struct heraus schreiben
	NVIC_Init(&NVIC_InitStructure);
}



//A05-02.1
void init_usart_2_irq(void) {
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Die Freigabe des zugehörigen Interrupts sieht wie fogt aus:
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

}

//A06-02.1
void init_alarm1(void) {

	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);

	// ALARM A soll um xx:xx:30 ausgelöst werden

	RTC_AlarmTypeDef RTC_Alarm_Struct;
	RTC_AlarmStructInit(&RTC_Alarm_Struct);

	// Alarmzeit setzen
	RTC_Alarm_Struct.RTC_AlarmTime.RTC_Seconds  = 0x30;

	// Alarmmaske setzen (kann auch verodert werden)
	RTC_Alarm_Struct.RTC_AlarmMask          = RTC_AlarmMask_DateWeekDay   // Wochentag oder Tag ausgeblendet
	                                        | RTC_AlarmMask_Hours         // Stunde ausgeblendet
	                                        | RTC_AlarmMask_Minutes;       // Minute ausgeblendet

	// Konfiguration von Alarm A
	RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_Alarm_Struct);

	// RTC Alarm A Interruptkonfiguration

	// Anlegen der benötigten Structs
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// EXTI-Line Konfiguration
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	// NIVC Konfiguration
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Konfigurieren des Alarm A
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);

	// RTC Alarm A freigeben
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

	// Alarmflag löschen
	RTC_ClearFlag(RTC_FLAG_ALRAF);

}

void init_alarm2(void) {

	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);

	// Jeden Montag um 00:30:00 soll ein Alarm A ausgelöst werden

	RTC_AlarmTypeDef RTC_Alarm_Struct;

	RTC_AlarmStructInit(&RTC_Alarm_Struct);
	// Alarmzeit setzen..
	RTC_Alarm_Struct.RTC_AlarmTime.RTC_Hours    = 0x00;
	RTC_Alarm_Struct.RTC_AlarmTime.RTC_Minutes  = 0x30;
	RTC_Alarm_Struct.RTC_AlarmTime.RTC_Seconds  = 0x00;

	// Alarmmaske setzen (kann auch verodert werden)
	RTC_Alarm_Struct.RTC_AlarmMask          = RTC_AlarmMask_None;          // alle Felder werden verglichen

	// Alarm für den Tag oder Wochentag auswählen
	RTC_Alarm_Struct.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay; // Wochentag (Mo-So)

	// Alarm Tag oder Wochentag setzen
	RTC_Alarm_Struct.RTC_AlarmDateWeekDay    =  RTC_Weekday_Monday;      // Wochentag Mo

	// Konfiguration von Alarm A
	RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_Alarm_Struct);

	// RTC Alarm A Interruptkonfiguration

	// Anlegen der benötigten Structs
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// EXTI-Line Konfiguration
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	// NIVC Konfiguration
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Konfigurieren des Alarm A
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);

	// RTC Alarm A freigeben
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

	// Alarmflag löschen
	RTC_ClearFlag(RTC_FLAG_ALRAF);

}

void init_alarm_every_sec(int delta, int init) {
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);

	RTC_AlarmTypeDef RTC_Alarm_Struct;
	RTC_AlarmStructInit(&RTC_Alarm_Struct);

	// Alarmzeit setzen
	RTC_TimeTypeDef  RTC_Time_Aktuell;
	RTC_GetTime(RTC_Format_BIN, &RTC_Time_Aktuell);

	RTC_Alarm_Struct.RTC_AlarmTime.RTC_Seconds = (RTC_Time_Aktuell.RTC_Seconds + delta)%60;

	// Alarmmaske setzen (kann auch verodert werden)
	RTC_Alarm_Struct.RTC_AlarmMask          = RTC_AlarmMask_DateWeekDay   // Wochentag oder Tag ausgeblendet
	                                        | RTC_AlarmMask_Hours         // Stunde ausgeblendet
	                                        | RTC_AlarmMask_Minutes;       // Minute ausgeblendet

	// Konfiguration von Alarm A
	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_Alarm_Struct);

	if(init==1) {
		// RTC Alarm A Interruptkonfiguration

		// Anlegen der benötigten Structs
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		// EXTI-Line Konfiguration
		EXTI_ClearITPendingBit(EXTI_Line17);
		EXTI_InitStructure.EXTI_Line = EXTI_Line17;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);

		// NIVC Konfiguration
		NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}

	// Konfigurieren des Alarm A
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);

	// RTC Alarm A freigeben
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

	// Alarmflag löschen
	RTC_ClearFlag(RTC_FLAG_ALRAF);

	char data[50] = {0};
    sprintf(data,
      "\r\nTIME: %.2d:%.2d:%.2d:%.2d",
      RTC_Time_Aktuell.RTC_Hours,
      RTC_Time_Aktuell.RTC_Minutes,
      RTC_Time_Aktuell.RTC_Seconds,
      RTC_Time_Aktuell.RTC_H12
    );
    usart_2_print(data);

}

void init_RTC_wakeup_timer(uint16_t sec) {

	// Strukt anlegen
	    EXTI_InitTypeDef EXTI_InitStructure;
	    NVIC_InitTypeDef NVIC_InitStructure;

	    // EXTI-Line Konfiguration für WakeUp
	    EXTI_ClearITPendingBit(EXTI_Line22);
	    EXTI_InitStructure.EXTI_Line = EXTI_Line22;
	    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	    EXTI_Init(&EXTI_InitStructure);

	    // NIVC Konfiguration für WakeUp
	    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
	    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	    NVIC_Init(&NVIC_InitStructure);

	    RTC_WakeUpCmd(DISABLE);

	    // Konfiguration der Clock
	    // RTC_WakeUpClock_RTCCLK_Div2;    (122,070usek...4sek) Zeitbasis: 61,035us
	    // RTC_WakeUpClock_RTCCLK_Div4;    (244,140usek...8sek) Zeitbasis: 122,070us
	    // RTC_WakeUpClock_RTCCLK_Div8;    (488,281usek...16sek)Zeitbasis: 244,140us
	    // RTC_WakeUpClock_RTCCLK_Div16;   (976,562usek...32sek)Zeitbasis: 488,281us
	    // RTC_WakeUpClock_CK_SPRE_16bits; (1sek...65535sek)    Zeitbasis: 1s
	    // RTC_WakeUpClock_CK_SPRE_17bits: (1sek...131070sek)   Zeitbasis: 1s
	    RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);

	    // RTC_WakeUpCouter mit einem Wert zwischen 0x0000...0xFFFF setzen
	    // Wert des WakeUpCounter aus Zeitintervall/Zeitbasis berechnen
	    // WakeUpCounterwert für intervall von Eingabewert sec s bei RTC_WakeUpClock_CK_SPRE_16bits
	    // ergibt sich aus sec/1s = sec
	    RTC_SetWakeUpCounter(sec);

	    //a)    Disable the RTC Wakeup interrupt
	    RTC_ITConfig(RTC_IT_WUT,DISABLE);

	    //b)    Clear the RTC Wakeup (WUTF) flag and afterwards the corresponding bit
	    RTC_ClearFlag(RTC_FLAG_WUTF);
	    RTC_ClearITPendingBit(RTC_IT_WUT);

	    //c)     Clear the PWR Wakeup (WUF) flag
	    PWR_ClearFlag(PWR_CSR_WUF);

	    //d)    Enable the RTC Wakeup interrupt
	    RTC_ITConfig(RTC_IT_WUT, ENABLE);   // Bit 14

	    RTC_WakeUpCmd(ENABLE);
}

// A8-1-1
void init_tim7_irq() {
	// Konfiguration der Interruptcontrollers
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Taktsystem für TIM7 Freigeben
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

	// Struktur anlegen
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	// TIM7 in der Struktur konfigurieren
	TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1; // 100µs = 8400 * 1/84000000Hz  //Warum -1?
	TIM_TimeBaseStructure.TIM_Period = 10000 - 1;   // 1s = 10000 * 100µs
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	// TIM7 Register aus der Struktur Schreiben
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

	// TIM7 Interrupt erlauben
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);

	// TIM 7 Freigeben (Takt auf Counter schalten)
	TIM_Cmd(TIM7, ENABLE);

	// ab jetzt wird jede Sekunde ein Interrupt ausgelöst
	// der die ISR d.h. den TIM7_IRQHandler aufruft.
}

//A8-1-2
void init_tim7_irq_2() {
    // Konfiguration der Interruptcontrollers
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // Taktsystem für TIM6 Freigeben
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

    // Struktur anlegen
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    // TIM7 in der Struktur konfigurieren
    TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1; // 100µs = 8400 * 1/84000000Hz
    TIM_TimeBaseStructure.TIM_Period = 100 - 1;   // 10ms = 100 * 100µs
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    // TIM7 Register aus der Struktur Schreiben
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

    // TIM7 Interrupt erlauben
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);

    // TIM 7 Freigeben (Takt auf Counter schalten)
    TIM_Cmd(TIM7, ENABLE);
}

//A8-1-3
void init_tim3() {
	// Taktsystem für den Port C Freigeben
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	// Struktur anlegen
	GPIO_InitTypeDef GPIO_InitStructure;

	// Portleitung in der Struktur Konfigurieren
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;

	// Werte aus der Struktur in die Register schreiben
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// Alternativfunktion der Portleitung Freigeben
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);

	// Taktsystem für Timer TIM3 Freigeben
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	// Siehe Input capture schritte im Manual
	// Erkennen steigender Flanke an TI1
	TIM3 -> CCMR1 |= TIM_CCMR1_CC1S_0 ;
	TIM3 -> CR2 |= TIM_CR2_TI1S ;

	// Polarität
	TIM3 -> CCER |= TIM_CCER_CC1P ;

	// Slave Mode, external Clock Mode1, TI1FP1 Signalquelle
	TIM3 -> SMCR |= TIM_SMCR_SMS + TIM_SMCR_TS_2 + TIM_SMCR_ETF_0; //TIM_SMCR_TS_0 ;

	// Jetzt haben wir einen external clock mode aktiviert und diesen mit dem Timer verbunden.
	// Als externe clock (Alternativ-funktion) nehmen wir den Input von PC08 (also der taste)
	// Der counter wird also jedesmal getriggered wenn eine steigende Flanke bei PC08 vorliegt.
	// Als nächsten Schritt erzeugen wir einen Interrupt wenn 10x hochgezählt wurde.

	// Konfiguration der Interruptcontrollers (Timer überhaupt interruptfähig machen)
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Struktur anlegen
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	// TIM3 in der Struktur konfigurieren
	TIM_TimeBaseStructure.TIM_Prescaler = 1;    // Der Input vom taster soll nicht gescaled werden. 1 Tastendruck = 1 up-count
	TIM_TimeBaseStructure.TIM_Period = 10 - 1;  // Auto-reload wert. Bei 10 geschieht ein overflow, ein interrupt wird ausgeführt und wir beginnen von 0
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //Von 0 auf 10 zählen

	// TIM3 Register aus der Struktur Schreiben
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	// TIM3 Interrupt erlauben
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	// Counter auf 0 setzen
	TIM_SetCounter (TIM3, 0x00);

	// Timer TIM3 Freigeben
	TIM_Cmd(TIM3, ENABLE);

	// ab jetzt führt jede Tastenbetätigung an Taster1 zur Erhöhung des
	// CNT-Counter Wertes von Timer TIM3
	// Bei Erreichen des Grenzwertes von 10 Tastenbetätigungen
	// wird die ISR aufgerufen
}

//A8-1-4
void init_tim5() {
    // Signal zur Frequenzmessung wird an die Portleitung
    // PA01 TIM5CH2 angeschlossen

    // Taktsystem für die Port A Freigeben
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    // Struktur anlegen
    GPIO_InitTypeDef GPIO_InitStructure;
    // Struktur Initialisieren
    GPIO_StructInit(&GPIO_InitStructure);
    // Portleitung in der Struktur Konfigurieren
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    // Werte aus der Struktur in die Register schreiben
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    // Taktsystem für TIM5 Freigeben
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    // Alternativfunktion der Portleitung Freigeben
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
    //Struktur anlegen
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    // Timer in der Struktur konfigurieren
    /*
     * Prescaler und Reload register sind immer 16 bit, egal welcher Timer. Dh. maximalwert von ~65000
     * Darasu folgt dass Prescaler <= 8400 für gerade zahlen was zu einer
     * minimalen Auflösung von 0.1ms Führt.
     * Da Auflösung = 0.1ms und Reload Register max. 65k -> maximal ein Wert von 6,5s messbar.
     * Für menschl. Reaktionszeit ist dies angemessen.
     */
    TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1; //Zeitauflösung 0.1ms für Reaktionszeitmessung
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF -1;  //Maximalzeit 6.5s für Reaktionszeitmessung
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    // TIM5 Register aus dem Strukt Schreiben
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
    // dadurch zählt der Counter in 0,00001sek Schritten hoch


    // TIM5CH2 Erkennen steigender Flanke an TI2
    TIM5->CCMR1 |= TIM_CCMR1_CC2S_0;
    // Input Filter setzen
    TIM5->CCMR1 |= TIM_CCMR1_IC2F_2 + TIM_CCMR1_IC2F_1 + TIM_CCMR1_IC2F_0;
    // Trigger auf Steigende Flanke setzen
    TIM5->CCER   |= TIM_CCER_CC2P;
    // Prescaler auf Null setzen
    TIM5->CCMR1   &= ~(TIM_CCMR1_IC2PSC_1 + TIM_CCMR1_IC2PSC_0);
    // Capture freigeben
    TIM5->CCER   |= TIM_CCER_CC2E;
    // Enable Interrupt
    TIM5->DIER   |= TIM_DIER_CC2IE;

    // Konfiguration der Interruptcontrollers (Timer überhaupt interruptfähig machen)
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// TIM5 Interrupt erlauben
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
}

uint32_t get_random_int(uint32_t min, uint32_t max)
{
    uint32_t zahl=0;

    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);
    RNG_Cmd(ENABLE);

    while(RNG_GetFlagStatus(RNG_FLAG_DRDY)== RESET);
    zahl = RNG_GetRandomNumber();

    RNG_Cmd(DISABLE);
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, DISABLE);

    uint32_t result = min + (max-min)*((float) zahl/4294967295);
    return result;
}

//A8-2-XX fehlt hier


//A9-1-1
void init_ADC1_EXVOL(void) {
	//ADC1 deinitialisieren und ausschalten
	ADC_DeInit();

    // Taktquelle einschalten für PA00 und ADC1
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);

    //initialisiere PA00 als analoge Eingangsquelle zur Abnahme einer externen SPannung < 3V
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //Lege Multi ADC Mode fest
    //=== ADC1 Common configuration
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_CommonStructInit(&ADC_CommonInitStructure);
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8; // 84MHz / 8 = 10.5 MHz ADC_CLK
    //eine Konversion mit 3 Sample Cycles und 12 Conversion cycles => 10.5MHz/15 =0.7 MSamples/s
    //Total Conversion time : (0.7 MSamples/s)^(-1) = 1,429 us
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //not used only. only for interleaved dual/triple mode
    ADC_CommonInit(&ADC_CommonInitStructure);

    //Konfiguriere ADC1
    //=== ADC1 Structure Init
    ADC_InitTypeDef ADC_InitStructure;
    ADC_StructInit(&ADC_InitStructure);
    ADC_InitStructure.ADC_Resolution =  ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE; //Unterschied zu COntinous?
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //kein Trigger für Polling
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1; //nur für externe Trigger
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    //konfiguriere Conversion Groups
    //=== ADC1 Channel Config
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_3Cycles);

    //=== ADC1 Enable
    ADC_Cmd(ADC1, ENABLE);

}

//A9-1-2
void init_ADC1_BAT(void) {
	//ADC1 deinitialisieren und ausschalten
	ADC_DeInit();
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, DISABLE);

    // Taktquelle einschalten für ADC1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);

    //Lege Multi ADC Mode fest
    //=== ADC1 Common configuration
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_CommonStructInit(&ADC_CommonInitStructure);
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8; // 84MHz / 8 = 10.5 MHz ADC_CLK
    //eine Konversion mit 3 Sample Cycles und 12 Conversion cycles => 10.5MHz/15 =0.7 MSamples/s
    //Total Conversion time : (0.7 MSamples/s)^(-1) = 1,429 us
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //not used only. only for interleaved dual/triple mode
    ADC_CommonInit(&ADC_CommonInitStructure);

    //Konfiguriere ADC1
    //=== ADC1 Structure Init
    ADC_InitTypeDef ADC_InitStructure;
    ADC_StructInit(&ADC_InitStructure);
    ADC_InitStructure.ADC_Resolution =  ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE; //Unterschied zu COntinous?
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //kein Trigger für Polling
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1; //nur für externe Trigger
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;            //Anzahl der Kanäle in der Gruppe
    ADC_Init(ADC1, &ADC_InitStructure);

	//konfiguriere Conversion Groups
	//=== ADC1 Channel Config
	ADC_RegularChannelConfig(ADC1, ADC_Channel_Vbat, 1, ADC_SampleTime_3Cycles);
	//=== ADC1 Enable
	ADC_Cmd(ADC1, ENABLE);
    //Knopfbatterie einschalten
    ADC_VBATCmd(ENABLE);

}

void init_ADC1_TEMP(void) {
	//ADC1 deinitialisieren und ausschalten
	ADC_DeInit();
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, DISABLE);

    // Taktquelle einschalten für ADC1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);

    //Lege Multi ADC Mode fest
    //=== ADC1 Common configuration
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_CommonStructInit(&ADC_CommonInitStructure);
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8; // 84MHz / 8 = 10.5 MHz ADC_CLK
    //eine Konversion mit 3 Sample Cycles und 12 Conversion cycles => 10.5MHz/15 =0.7 MSamples/s
    //Total Conversion time : (0.7 MSamples/s)^(-1) = 1,429 us
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //not used only. only for interleaved dual/triple mode
    ADC_CommonInit(&ADC_CommonInitStructure);

    //Konfiguriere ADC1
    //=== ADC1 Structure Init
    ADC_InitTypeDef ADC_InitStructure;
    ADC_StructInit(&ADC_InitStructure);
    ADC_InitStructure.ADC_Resolution =  ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE; //Unterschied zu COntinous?
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //kein Trigger für Polling
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1; //nur für externe Trigger
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;            //Anzahl der Kanäle in der Gruppe
    ADC_Init(ADC1, &ADC_InitStructure);

    //konfiguriere Conversion Groups
    //=== ADC1 Channel Config
    ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_480Cycles);
    //=== ADC1 Enable
    ADC_Cmd(ADC1, ENABLE);
    // Temperatursensor einschalten
    ADC_TempSensorVrefintCmd(ENABLE);
}


