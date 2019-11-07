#include "aufgabe.h"




void aufgabe_A01_1_1(void)
{
	// Setzt GPIO Port auf den Reset Zustand zurück
	GPIO_DeInit(GPIOB);

	// Taktquelle für die Peripherie aktivieren
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	// Struct anlegen
	GPIO_InitTypeDef GPIO_InitStructure;

	// Struct Initialisieren setzt alle Leitungen auf
	// Eingang ohne PushPull
	GPIO_StructInit(&GPIO_InitStructure);

	// Die Funktionalität der Portleitungen festlegen

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;

	GPIO_InitStructure.GPIO_Mode =
	    // Auswahl des I/O Mode
	    //  GPIO_Mode_IN;       //GPIO Input Mode
	      GPIO_Mode_OUT;        //GPIO Output Mode
	    //  GPIO_Mode_AF;       //GPIO Alternate function Mode
	    //  GPIO_Mode_AN;       //GPIO Analog Mode

	GPIO_InitStructure.GPIO_Speed =
	    // Auswahl der Speed
	        GPIO_Speed_2MHz;    //  Low speed
	    //  GPIO_Speed_25MHz;   //  Medium speed
	    //  GPIO_Speed_50MHz;   //  Fast speed
	    //  GPIO_Speed_100MHz;  //  High speed

	GPIO_InitStructure.GPIO_OType =
	    // Auswahl des Output Typs
	        GPIO_OType_PP;      //  PushPull
	    //  GPIO_OType_OD;      //  OpenDrain

	GPIO_InitStructure.GPIO_PuPd =
	    // Auswahl des Push/Pull Typs
	        GPIO_PuPd_NOPULL;   //  NoPull
	    //  GPIO_PuPd_UP;       //  PullUp
	    //  GPIO_PuPd_DOWN;     //  PullDown

	// PortLeitungen initialisieren
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//GPIO_Write(GPIOB, 0x0004);	//LED ON
	//GPIO_Write(GPIOB, 0x0000);	//LED OFF

	//GPIO_SetBits(GPIOB, GPIO_Pin_2);			//LED ON
	//GPIO_ResetBits(GPIOB, GPIO_Pin_2);		//LED OFF
	//GPIO_ToggleBits(GPIOB, GPIO_Pin_2);		//LED TOGGLE

}
