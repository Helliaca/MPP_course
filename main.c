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

	// Beispiel für die Loesung einer Aufgabe

    init_iwdg();
    init_tasten();
	init_usart_2();

	char * usart_2_tx_neustart = "\r\nNeustart\r\n";
	char * usart_2_tx_schleife = "Schleife\r\n";
	char * usart_2_tx_taste = "Taste 2 gedrückt \r\n";

	usart_2_print(usart_2_tx_neustart);

	while(1) {
		/*
		 * Quelle: Datasheet Seite 101
		 * LSI Frequenz kann zwischen 17 und 47kHz variieren.
		 * Der Neustart erfolgt ungefähr nach 5,3s im Gegensatz zu der eigentlich spezifizierten
		 * Countdown Time von 5s in den IWDG Einstellungen. Dies entspricht einer LSI-Frequenz
		 * von crica 30kHz = (5,3s /(64*2500))^(-1).
		 */
		usart_2_print(usart_2_tx_schleife);
		//wait for 0,5s um die Schleifendurchläufe zu verlangsamen
		wait_uSek(500000);
		IWDG_ReloadCounter();

		if(taste2_downed()) {
			usart_2_print(usart_2_tx_taste);
			//wait for 4,8s
			wait_uSek(4800000);
		}
	}
}
