#include "main.h"

int main ( void )
{
    // Initialisierung des Systems und des Clocksystems
    SystemInit();

    // SysTick initialisieren
    // jede Millisekunde erfolgt dann der Aufruf
    // des Handlers fuer den Interrupt SysTick_IRQn
    InitSysTick();

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

	// Beispiel f�r die Loesung einer Aufgabe

    //init_PC09();
    init_tasten();
	init_leds();
	init_usart_2();

	char usart2_rx_buffer[PUFFER_SIZE];
	char usart2_tx_buffer[PUFFER_SIZE];
	int str_len = 0;

	while (1)
	{
		// warten bis ein Zeichen empfangen wurde
		while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET){}
		// Zeichen lesen und auf ein char casten
		char zeichen = (char) USART_ReceiveData(USART2);
		if (str_len >= PUFFER_SIZE) {
			str_len = 0;
			char * error_msg = "Puffer size exceeded.\r\n";
			usart_2_print(error_msg);
		} else {
			if(zeichen==0x0D) {
				//Nullterminator an char Array
				usart2_rx_buffer[str_len] = 0x00;
				//Formattiere den zu sendenden String
				sprintf(usart2_tx_buffer,"%s Laenge=%d \r\n",usart2_rx_buffer,str_len);
				usart_2_print(usart2_tx_buffer);
				//Setze Zeiger wieder an Anfang des char arrays
				str_len = 0;
				/*
				 * bei der ersten Ausf�hrung wird noch am Anfang des Strings ein '-' ausgegeben. ???
				 */

			} else {
				usart2_rx_buffer[str_len] = zeichen;
				str_len++;
			}
		}
	}
}
