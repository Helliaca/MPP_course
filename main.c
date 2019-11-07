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

    //init_PC09();
    init_tasten();
    init_leds();
    init_usart_2();

    while (1)
    {
    	// warten bis ein Zeichen empfangen wurde
    	while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET){}
    	// Zeichen lesen und auf ein char casten
    	char led_mode = (char) USART_ReceiveData(USART2);

    	switch (led_mode) {
			case '1':
				blink_interval = 1000;
				usart_2_print("Gruene LED im 1 Sekundentakt\r\n");
				break;
			case '2':
				blink_interval = 2000;
				usart_2_print("Gruene LED im 2 Sekundentakt\r\n");
				break;
			case '4':
				blink_interval = 4000;
				usart_2_print("Gruene LED im 4 Sekundentakt\r\n");
				break;
			default:
				break;
		}
    }
}
