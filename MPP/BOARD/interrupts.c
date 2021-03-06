#include "interrupts.h"
#include <inttypes.h>

void hard_fault_handler_c(unsigned int * hardfault_args);

//=========================================================================
void NMI_Handler(void)
{
	while (1){ ; }
}

//=========================================================================
void HardFault_Handler(void)
{
	asm ("TST LR, #4");
	asm ("ITE EQ");
	asm ("MRSEQ R0, MSP");
	asm ("MRSNE R0, PSP");
	asm ("B hard_fault_handler_c");
}

//=========================================================================
void MemManage_Handler(void)
{
	while (1){ ; }
}

//=========================================================================
void BusFault_Handler(void)
{
	while (1){ ; }
}

//=========================================================================
void UsageFault_Handler(void)
{
	while (1){ ; }
}

//=========================================================================
void SVC_Handler(void)
{
	while (1){ ; }
}

//=========================================================================
void DebugMon_Handler(void)
{
	while (1){ ; }
}

//=========================================================================
//void PendSV_Handler(void){
//}

//=========================================================================
void SysTick_Handler(void)
{
	static unsigned long stc_led = 0;
	static unsigned long stc_timer = 0;
	static unsigned long stc0 = 0;
	static unsigned long stc1 = 0;
	static unsigned long stc2 = 0;
	stc_timer++;
	stc_led++;
	stc0++;
	stc1++;
	stc2++;

	//======================================================================
	// DW1000 Timeout
	systickcounter += 1;

	if ( stc0 >= 20 )
		{
			uwbranging_tick();
			stc0 = 0;
		}



	//======================================================================
	//	CoOS_SysTick_Handler alle 10ms in CoOs arch.c aufrufen
	// nur Einkommentieren wenn CoOS genutzt wird
	CoOS_SysTick_Handler();



	//======================================================================
	// CC3100 alle 50ms Sockets aktualisieren
	if (stc2 >= 5)
		{
			stc2 = 0;
			if ( (IS_CONNECTED(WiFi_Status)) && (IS_IP_ACQUIRED(WiFi_Status)) && (!Stop_CC3100_select) && (!mqtt_run) )
			{
			CC3100_select(); // nur aktiv wenn mit AP verbunden
			}
			else
			{
			_SlNonOsMainLoopTask();
			}
		}

	//======================================================================
	// SD-Card
	sd_card_SysTick_Handler();

	//======================================================================
	// MQTT
	MQTT_SysTickHandler();

	/*
	//======================================================================
	// LED zyklisch schalten
	if( stc_led == 500 ) {
		green_LED_OFF;
	}
	else if( stc_led == 3500 ) {
		green_LED_ON;
		stc_led = 0;
	}
	*/

	//======================================================================
	// stc_timer und timer aus interrupts.h aktualisieren
	/*
	if(stc_timer > blink_interval-1 && blink_interval > -1) {
		green_LED_Toggle;
		stc_timer = 0;
	}
	*/
	if(stc_timer > 99) {
		timer--;
		stc_timer = 0;
	}
}


//=========================================================================
void WWDG_IRQHandler(void)
{

	unsigned char value_watchdog_counter = 0x7F;
    WWDG_ClearFlag();
    WWDG_DeInit();
	WWDG_SetCounter(value_watchdog_counter);
    WWDG_ClearFlag();
    usart2_send("WWDG_IRQn\r\n");
}


//=========================================================================
void EXTI0_IRQHandler(void)
{
	//===== CC3100
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
		{
			EXTI_ClearFlag(EXTI_Line0);
			EXTI_ClearITPendingBit(EXTI_Line0);
			WLAN_intHandler();	// ISR fuer CC3100 Interrupt
		}
}


//=========================================================================
void EXTI1_IRQHandler(void)
{
	//==== DW1000
	EXTI->PR = EXTI_Line1;		// Setze den Interrupt zur�ck
	dw1000_irqactive = 1;		// IRQ aktiv merken
	dw1000_handleInterrupt();	// ISR fuer DW1000 Interrupt
	dw1000_irqactive = 0;		// IRQ nicht mehr aktiv merken
}


//=========================================================================
void EXTI2_IRQHandler(void)
{
	//===== nicht belegt
	if(EXTI_GetITStatus(EXTI_Line2) == SET)
		{
			EXTI_ClearFlag(EXTI_Line2);
			EXTI_ClearITPendingBit(EXTI_Line2);
			// nicht belegt
		}
}


//=========================================================================
void EXTI3_IRQHandler(void)
{
	//===== nicht belegt
	if(EXTI_GetITStatus(EXTI_Line3) == SET)
		{
			EXTI_ClearFlag(EXTI_Line3);
			EXTI_ClearITPendingBit(EXTI_Line3);
			// nicht belegt
		}
}


//=========================================================================
void EXTI4_IRQHandler(void)
{	//===== MPU-9250
	if(EXTI_GetITStatus(EXTI_Line4) == SET)
		{
			EXTI_ClearFlag(EXTI_Line4);
			EXTI_ClearITPendingBit(EXTI_Line4);
			// ISR fuer MPU-9250 nichts hinterlegt
		}
}


//=========================================================================
void EXTI9_5_IRQHandler(void)
{
	//===== Taster2
	if (EXTI_GetITStatus(EXTI_Line5) == SET)
		{
			EXTI_ClearFlag(EXTI_Line5);
			EXTI_ClearITPendingBit(EXTI_Line5);
			TASTER2_IRQ();	// ISR fuer Taste 2
		}
	//===== nicht belegt
	if (EXTI_GetITStatus(EXTI_Line6) == SET)
		{
			EXTI_ClearFlag(EXTI_Line6);
			EXTI_ClearITPendingBit(EXTI_Line6);
			// nicht belegt
		}
	//===== nicht belegt
	if (EXTI_GetITStatus(EXTI_Line7) == SET)
		{
			EXTI_ClearFlag(EXTI_Line7);
			EXTI_ClearITPendingBit(EXTI_Line7);
			// nicht belegt
		}
	//===== Taster 1
	if (EXTI_GetITStatus(EXTI_Line8) == SET)
		{
			EXTI_ClearFlag(EXTI_Line8);
			EXTI_ClearITPendingBit(EXTI_Line8);
			TASTER1_IRQ();	// ISR fuer Taste 1
		}
	//===== nicht belegt
	if (EXTI_GetITStatus(EXTI_Line9) == SET)
		{
			EXTI_ClearFlag(EXTI_Line9);
			EXTI_ClearITPendingBit(EXTI_Line9);
			// nicht belegt
		}
}


//=========================================================================
void EXTI15_10_IRQHandler(void)
{
	//===== nicht belegt
	if(EXTI_GetITStatus(EXTI_Line10) == SET)
		{
			EXTI_ClearFlag(EXTI_Line10);
			EXTI_ClearITPendingBit(EXTI_Line10);
			// nicht belegt
		}
	//===== nicht belegt
	if(EXTI_GetITStatus(EXTI_Line11) == SET)
		{
			EXTI_ClearFlag(EXTI_Line11);
			EXTI_ClearITPendingBit(EXTI_Line11);
			// nicht belegt
		}
	//===== nicht belegt
	if (EXTI_GetITStatus(EXTI_Line12) == SET)
		{
			EXTI_ClearFlag(EXTI_Line12);
			EXTI_ClearITPendingBit(EXTI_Line12);
			// nicht belegt
		}
	//===== nicht belegt
	if(EXTI_GetITStatus(EXTI_Line13) == SET)
		{
			EXTI_ClearFlag(EXTI_Line13);
			EXTI_ClearITPendingBit(EXTI_Line13);
			// nicht belegt
		}
	//===== nicht belegt
	if(EXTI_GetITStatus(EXTI_Line14) == SET)
		{
			EXTI_ClearFlag(EXTI_Line14);
			EXTI_ClearITPendingBit(EXTI_Line14);
			// nicht belegt
		}
	//===== nicht belegt
	if(EXTI_GetITStatus(EXTI_Line15) == SET)
		{
			EXTI_ClearFlag(EXTI_Line15);
			EXTI_ClearITPendingBit(EXTI_Line15);
			// nicht belegt
		}

}

//=========================================================================
void RTC_Alarm_IRQHandler(void)
{
	//===== Time Stamp interrupt
	if(RTC_GetITStatus(RTC_IT_TS) != RESET)
		{
			RTC_ClearITPendingBit(RTC_IT_TS);
			EXTI_ClearITPendingBit(EXTI_Line21);
			// nicht belegt
		}
	//=====	WakeUp Timer interrupt
	if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
		{
			RTC_ClearITPendingBit(RTC_IT_WUT);
			EXTI_ClearITPendingBit(EXTI_Line22);
			// nicht belegt
		}
	//===== RTC_IT_ALRB: Alarm B interrupt
	if(RTC_GetITStatus(RTC_IT_ALRB) != RESET)
		{
			RTC_ClearITPendingBit(RTC_IT_ALRB);
			EXTI_ClearITPendingBit(EXTI_Line17);
			// nicht belegt
		}
	//===== RTC_IT_ALRA: Alarm A interrupt
	if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
		{
			RTC_ClearITPendingBit(RTC_IT_ALRA);
			EXTI_ClearITPendingBit(EXTI_Line17);
			// Ab dem zweiten mal wird keine Interrupt ausgef�hrt. (init ist �berf��l�ssig)
			usart_2_print("Interrupt!");
			init_alarm_every_sec(5, 0);
			green_LED_Toggle;
			//	if (RTC_Alarm_CallBack[0] != NULL)
			//	{
			//	RTC_Alarm_CallBack[0]();
			//	wait_uSek(3000000);
			//	}
		}
	//===== RTC_IT_TAMP1: Tamper 1 event interrupt
	if(RTC_GetITStatus(RTC_IT_TAMP1) != RESET)
		{
			RTC_ClearITPendingBit(RTC_IT_TAMP1);
			EXTI_ClearITPendingBit(EXTI_Line21);
			// nicht belegt
		}
}


int k =0; // Index f�r Ringbuffer
//=========================================================================
void ADC_IRQHandler(void)
{
	//===== ADC EOC interrupt
	if(ADC_GetITStatus(ADC1, ADC_IT_EOC) == SET)
		{
			ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
			uint16_t digitalwert=ADC_GetConversionValue(ADC1);
			mittelwert_buffer[k] = digitalwert;
			k = (k + 1) % zeitfenster;
			int i; int sum=0;
			for (i=0; i< zeitfenster; i++) {
				sum += mittelwert_buffer[i];
			}
			int mov_avg = round(((float) sum )/ zeitfenster);
			DAC_SetChannel1Data(DAC_Align_12b_R, mov_avg);

		}
	//===== ADC AWD interrupt
	if(ADC_GetITStatus(ADC1, ADC_IT_AWD) == SET)
		{
			ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);
			// ... Code f�r analogen Watchdog
		}
}

void process(char* data) {
	RTC_DateStructInit(&RTC_Date_IN);
	RTC_TimeStructInit(&RTC_Time_IN);
	int success, error;
	switch(data[0]) {
		case 't': {
			int hh, mm, s, h12;
			success = sscanf(data,"time %d:%d:%d:%d", &hh, &mm, &s, &h12);

			RTC_Time_IN.RTC_Hours		= hh;
			RTC_Time_IN.RTC_Minutes 	= mm;
			RTC_Time_IN.RTC_Seconds 	= s;
			RTC_Time_IN.RTC_H12 		= h12;

			if (success == 4 && 1 == RTC_SetTime(RTC_Format_BIN, &RTC_Time_IN)) {
				usart_2_print("\r\n=====SUCCESSFULLY CHANGED TIME=====");
			} else {error =1;}
			break;
		}
		case 'd': {
			int ye, mo, da, wd;
			success = sscanf(data,"date %d-%d-%d-%d", &ye, &mo, &da, &wd);

			RTC_Date_IN.RTC_Year        = ye;
			RTC_Date_IN.RTC_Month       = mo;
			RTC_Date_IN.RTC_Date        = da;
			RTC_Date_IN.RTC_WeekDay     = wd;

			if (success == 4 && 1 == RTC_SetDate(RTC_Format_BIN, &RTC_Date_IN)) {
				usart_2_print("\r\n=====SUCCESSFULLY CHANGED DATE=====");
			} else {error =1;}
			break;
		}
		default : {
			error = 1;
			break;
		}
	}

	if (error == 1) {
		usart_2_print("\r\nERROR: INPUT STRING ");
		usart_2_print(data);
	}
}

int str_len = 0;
//=========================================================================
void USART2_IRQHandler(void)
{
	// RxD - Empfangsinterrupt
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		char zeichen = (char)USART_ReceiveData(USART2);

		//A11-1-1 --BEGIN
		static int j = 0;
		if (zeichen=='\r') {
			usart_2_rx_buffer[j] = '\r';
			usart_2_rx_buffer[j+1] = '\n';
			usart_2_rx_buffer[j+2] = 0x00 ;

			// die empfangene Zeichenkette wird in einen globalen Puffer kopiert
			//strcpy(storage_buffer, usart_2_rx_buffer);
			//setzt alle werte in usaert2_rx_buffer auf 0
			j=0;

			/*Zeichenkette senden */
			zeichenkette_senden(usart_2_rx_buffer);
			memset(usart_2_rx_buffer,0x00,sizeof(usart_2_rx_buffer));
		}
		else {
			usart_2_rx_buffer[j] = zeichen;
			j++;
			if (j >= sizeof(usart_2_rx_buffer)) {
				//usart_2_print("ERROR: rx buffer overflow");
				j = 0;
			}
		}
		//A11-1-1 --END

		/*
		//A8-2-2 --BEGIN
		if(zeichen=='w') {
			int wert = TIM10->CCR1 + 100;
			if(wert<=2400 && wert>=800) {
				TIM10->CCR1 = wert;
			}
		}
		if(zeichen=='s') {
			int wert = TIM10->CCR1 - 100;
			if(wert<=2400 && wert>=800) {
				TIM10->CCR1 = wert;
			}
		}
		//A8-2-2 --END
		 */

		//A8-2-1 --BEGIN
		/*
		//Wahl der Frequenzen gem�ss Blues-Pentatonik-Tonleiter mit Grundton c1
		switch(zeichen) {
		case 'a': { freq = 261; break; } //c1
		case 's': { freq = 311; break; } //dis1
		case 'd': { freq = 349; break; } //f1
		case 'f': { freq = 391; break; } //g1
		case 'g': { freq = 466; break; } //ais1
		case 'h': { freq = 523; break; } //c2
		}
		//beep(freq, 250, 0);
		 */
		//A8-2-1 --END

		//A8-1-4
		/*
		if(zeichen=='s') {
			messung_started = 1;
		}
		*/
		//Ende A8-1-4

		/*
		if (str_len >= PUFFER_SIZE) {
			str_len = 0;
			char * error_msg = "Puffer size exceeded.\r\n";
			usart_2_print(error_msg);
		} else {
			if(zeichen==0x0D) {
				//Nullterminator an char Array
				usart2_rx_buffer[str_len] = 0x00;
				process(usart2_rx_buffer);
				//Setze Zeiger wieder an Anfang des char arrays
				str_len = 0;
			} else {
				usart2_rx_buffer[str_len] = zeichen;
				str_len++;
			}
		}
		*/
	}
}


//=========================================================================
void UART5_IRQHandler(void)
{
	//===== USART5
	if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
		{
			USART_SendData(USART2, (char)USART_ReceiveData(UART5));
		}
}


//=========================================================================
void USART6_IRQHandler(void)
{
	//===== USART6
	CC3100_uart6_receive_IRQ();
}

//=========================================================================
void DMA2_Stream6_IRQHandler(void)
{
	//===== DMA2_Stream6
	//DMA2_Stream6_IRQ();
}

//=========================================================================
void DMA1_Stream6_IRQHandler(void)
{

	// ist der DMA Stream komplett?
	if (DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6))
	{
		dma_running = 0;
		// dann wird Transfer Complete interrupt pending bit zur�ckgesetzt
		DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
		//DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);
	}
}

//=========================================================================
void RTC_WKUP_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
		{
			RTC_ClearITPendingBit(RTC_IT_WUT);
			EXTI_ClearITPendingBit(EXTI_Line22);
		}
}

//=========================================================================
void TIM3_IRQHandler(void)
{
	// Interrupt bei update
	usart_2_print("\n\rTIM3_IRQ");
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

		usart_2_print("\n\rCounter Overflow.");
	}
}


//=========================================================================
void TIM5_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);

		usart_2_print("\r\nWarning: Maximum time exceeded.");
	}
}
/*
const double  U_0 = 1;
const double  U_b = 1.5;
double voltage=0;
uint16_t digital_value;
int sine_index=0;
*/
//=========================================================================
void TIM7_IRQHandler(void)
{
	/*
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) {
	        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);

	        	/* A10-1-2
	        	sine_index %= 100;
	            voltage = U_0 * sine_values[sine_index] + U_b;
	            digital_value = (uint16_t) round(voltage / 3.3 * 4096);
	            DAC_SetChannel1Data(DAC_Align_12b_R, digital_value);
	            sine_index++;
	            */

	        	/* A11-1-2
	        	// Gebe die Paketl�nge an (Verschachtelt, damit nur eine Berechnung notwendig ist)
	            DMA_SetCurrDataCounter(DMA1_Stream5, (unsigned short) 1);

	            // Aktiviere den DMA Transfer
	            DMA_Cmd(DMA1_Stream5, ENABLE);
	    }
	*/
    BEEPER_IRQHandler();
}

//=========================================================================
void DMA2_Stream2_IRQHandler(void)
{
	//====DW1000 RXD stream der SPI1
	if (DMA_GetITStatus(DMA2_Stream2, DMA_IT_TCIF2))
	{
	DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2);
	}
}

//=========================================================================
void DMA2_Stream3_IRQHandler(void)
{
	//====DW1000 TXD stream der SPI1
	if (DMA_GetITStatus(DMA2_Stream3, DMA_IT_TCIF3))
	{
	DMA_ClearITPendingBit(DMA2_Stream3, DMA_IT_TCIF3);
	}
}




//=========================================================================
void TIM6_DAC_IRQHandler()
{
	if((TIM6->SR) & (TIM_SR_UIF != 0) )
		{

		}
	TIM6->SR &= ~TIM_SR_UIF;
}


//=========================================================================
// From Joseph Yiu, minor edits by FVH
// hard fault handler in C,
// with stack frame location as input parameter
// called from HardFault_Handler in file
void hard_fault_handler_c(unsigned int * hardfault_args)
{
	char out[256];
	unsigned int stacked_r0;
	unsigned int stacked_r1;
	unsigned int stacked_r2;
	unsigned int stacked_r3;
	unsigned int stacked_r12;
	unsigned int stacked_lr;
	unsigned int stacked_pc;
	unsigned int stacked_psr;

	stacked_r0 = ((unsigned long) hardfault_args[0]);
	stacked_r1 = ((unsigned long) hardfault_args[1]);
	stacked_r2 = ((unsigned long) hardfault_args[2]);
	stacked_r3 = ((unsigned long) hardfault_args[3]);

	stacked_r12 = ((unsigned long) hardfault_args[4]);
	stacked_lr = ((unsigned long) hardfault_args[5]);
	stacked_pc = ((unsigned long) hardfault_args[6]);
	stacked_psr = ((unsigned long) hardfault_args[7]);

	uart_send("\r\n[Hard fault handler - all numbers in hex]\r\n");
	sprintf(out, "R0  = %x\r\n", stacked_r0);
	uart_send(out);
	sprintf(out, "R1  = %x\r\n", stacked_r1);
	uart_send(out);
	sprintf(out, "R2  = %x\r\n", stacked_r2);
	uart_send(out);
	sprintf(out, "R3  = %x\r\n", stacked_r3);
	uart_send(out);
	sprintf(out, "R12 = %x\r\n", stacked_r12);
	uart_send(out);
	sprintf(out, "LR [R14] = %x  subroutine call return address\r\n", stacked_lr);
	uart_send(out);
	sprintf(out, "PC [R15] = %x  program counter\r\n", stacked_pc);
	uart_send(out);
	sprintf(out, "PSR  = %x\r\n", stacked_psr);
	uart_send(out);
	sprintf(out, "BFAR = %lx\r\n", (*((volatile unsigned long *) (0xE000ED38))));
	uart_send(out);
	sprintf(out, "CFSR = %lx\r\n", (*((volatile unsigned long *) (0xE000ED28))));
	uart_send(out);
	sprintf(out, "HFSR = %lx\r\n", (*((volatile unsigned long *) (0xE000ED2C))));
	uart_send(out);
	sprintf(out, "DFSR = %lx\r\n", (*((volatile unsigned long *) (0xE000ED30))));
	uart_send(out);
	sprintf(out, "AFSR = %lx\r\n", (*((volatile unsigned long *) (0xE000ED3C))));
	uart_send(out);
	sprintf(out, "SCB_SHCSR = %x\r\n", (unsigned int) SCB->SHCSR);
	uart_send(out);

	if (SCB->HFSR & SCB_HFSR_DEBUGEVT_Msk) {
		uart_send("##This is a DEBUG FAULT##\r\n");

	} else if (SCB->HFSR & SCB_HFSR_FORCED_Msk) {
		uart_send("##This is a FORCED FAULT##\r\n");

//		if (SCB->CFSR & (0x1 << SCB_CFSR_USGFAULTSR_Msk)) {
//			uart_send("undefined instruction\r\n");
//
//		} else if (SCB->CFSR & (0x2 << SCB_CFSR_USGFAULTSR_Pos)) {
//			uart_send("instruction makes illegal use of the EPSR\r\n");
//
//		} else if (SCB->CFSR & (0x4 << SCB_CFSR_USGFAULTSR_Pos)) {
//			uart_send("Invalid PC load UsageFault, caused by an invalid PC load by EXC_RETURN\r\n");
//
//		} else if (SCB->CFSR & (0x8 << SCB_CFSR_USGFAULTSR_Pos)) {
//			uart_send("The processor does not support coprocessor instructions\r\n");
//
//		} else if (SCB->CFSR & (0x100 << SCB_CFSR_USGFAULTSR_Pos)) {
//			uart_send("Unaligned access\r\n");
//
//		} else if (SCB->CFSR & (0x200 << SCB_CFSR_USGFAULTSR_Pos)) {
//			uart_send("Divide by zero\r\n");
//
//		}
	} else if (SCB->HFSR & SCB_HFSR_VECTTBL_Pos) {
		sprintf(out,"##This is a BUS FAULT##\r\n");
		uart_send(out);
	}
	uart_send("HARDFAULT HANDLER !!!!\r\n");
	while (1) {;}
}

