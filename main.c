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

    /*
     * value_watchdog_counter gibt den Initialwert des Down-Counters an.
     * window_value ist der Wert, ab welchem (inklusive) refresht werden darf bis
     * 0x3F (exklusive). Der letzte Wert ist fix implementiert. Erreicht der Down-
     * Counter ihn, wird ein Reset durchgeführt.
     * window_value_refresh gibt den von uns gewählten Zeitpunkt/Wert an, an welchem
     * der Down-Counter auf den Initialwert refresht wird.
     * Ein Reset des Down-Counters vor dem window_value, d.h. im verbotenen Bereich,
     * führt ebenfalls zum Reset.
     *
     * vwc > wv > wvr > 3f => endloser Refresh
     * vwc > wv = wvr > 3f => endloser Refresh
     * vwc > wvr > wv > 3f => Reset
     * vwc > wv > wvr = 3f => Reset
     * vwc = wv > wvr > 3f => endloser Refresh
     *
     * Bemerkung zur Grafik:
     * WDGA ist das Windowed WatchDog Enable Bit bzw. MSB (most-significat-bit) im Down-Counter (WWDG_CR).  (T7 bit)
     * Write WDGA_CR sit eine Flag, dass WWDG_CR refresht bzw. beschrieben wurde.
     * Comparator: 1 wenn imv erbotenem Bereich, sonst 0
     * Erster AND gate: "Wenn im verbotenem Bereich gerefresht wurde => Reset
     * OR gate: sobald 3f (T6=0) erreicht wird oder siehe oben => Reset
     * 2nd AND gate: WDGA = 1 bzw. WWDG enabled
     *
     * Mögliche Einsatzszenarien:
     *  - Anwendungen bei den zu frühe Antworten nicht möglich sind
     */
    unsigned char value_watchdog_counter = 0x7f;
    unsigned char window_value = 0x40;
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
