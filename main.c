#include "main.h"

int main ( void )
{
    // Initialisierung des Systems und des Clocksystems
    SystemInit();

    // SysTick initialisieren
    // jede Millisekunde erfolgt dann der Aufruf
    // des Handlers fuer den Interrupt SysTick_IRQn
    //InitSysTick();

    // Initialisierung aller Portleitungen und Schnittstellen
    // Freigabe von Interrupten
    // init_board();

    // Start der RTC  falls diese noch
    // nicht initialisiert war wird
    // die RTC mit der LSE-Taktquelle aktiviert
    // start_RTC();

    // Anmeldung beim WLAN Access Point
    // SSID: MPP_IoT
    //CC3100_set_in_STA_Mode(0); // WLAN

    // Initialisierung des CoCox Real Time Betriebssystems CoOS
    //CoInitOS ();

    // Anmelden notwendiger Task
    //CoCreateTask (...);

    // Start des Betriebssystems CoOS
    //CoStartOS ();

	// Beispiel für die Loesung einer Aufgabe
    unsigned char value_watchdog_counter = 0x7f;
	unsigned char window_value = 0x50;
	unsigned char window_value_refresh = 0x50;
	unsigned char cnt_i = 0;
	unsigned char cnt_j = 0;
	char usart2_tx_buffer[PUFFER_SIZE];

	init_usart_2();

	sprintf(usart2_tx_buffer,"\r\nNeustart\r\n");
	usart_2_print(usart2_tx_buffer);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

	WWDG_SetPrescaler(WWDG_Prescaler_8);
	WWDG_SetWindowValue(window_value);
	WWDG_Enable(value_watchdog_counter);

	cnt_i = (unsigned char) (value_watchdog_counter + 1);

	while(1){
		//übernehme Bitsequenz aus WWDG_CR in ein Char bzw. schneide alles ab 8tem Bit ab
		cnt_j = (unsigned char) ((WWDG->CR) & 0x7F) ;

		/*
		 * sofern j < i, gebe j aus. i nimmt den Wert an, den j im letzten Schleifendurchlauf hatte.
		 * Also prüft er, ob es eine Veränderung des WWDG_CR Registers zwischen den Schleifendurchläufen gab.
		 * Sobald im letzten Schleifendurchlauf der Wert des Refreshwindows erreicht wurde (in dem Fall 0x50),
		 * wird WWDG_CR auf 0x7F resetteted und i := 0x7F +1.
		 */
		if (cnt_j  < cnt_i ) {

			sprintf(usart2_tx_buffer,"i = %u\r\n",cnt_j);
			usart_2_print(usart2_tx_buffer);


			cnt_i = cnt_j;

			if (cnt_i == window_value_refresh ) {

				WWDG_SetCounter(value_watchdog_counter);

				sprintf(usart2_tx_buffer,"####### neu geladen\r\n");
				usart_2_print(usart2_tx_buffer);

				cnt_i = (unsigned char) (value_watchdog_counter + 1);
			}
		}
	}
}
