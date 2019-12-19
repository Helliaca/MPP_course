#include "main.h"

int main ( void )
{
    // Initialisierung des Systems und des Clocksystems
    SystemInit();

    // SysTick initialisieren
    // jede Millisekunde erfolgt dann der Aufruf
    // des Handlers fuer den Interrupt SysTick_IRQn
    //InitSysTick();

    // blink_interval wird win SysTickHandler verwendet und
    // spezifiziert das Blink-Intervall in ms. -1 entspricht "Nicht Blinken".
    //blink_interval = -1;

    // Initialisierung aller Portleitungen und Schnittstellen
    // Freigabe von Interrupten
    // init_board();

    // Start der RTC  falls diese noch
    // nicht initialisiert war wird
    // die RTC mit der LSE-Taktquelle aktiviert
    //start_RTC();

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
    init_leds();
    init_usart_2();
    init_tasten();
    init_taste2_irq();

    green_LED_ON;
    usart_2_print(" System Neustart\n");

    /*
     * Kein sleep modus:
     * 	- LED an: 64-65 mA,
     * 	- LED aus: 65-66 mA
     * Sleep modus (LED immer an):
     * 35 mA
     */
    while(1) {
		wait_mSek(3000);
		green_LED_Toggle;
		if(taste1_pressed()) {
			green_LED_ON;
			usart_2_print("Sleep Mode Start\n");
			__WFI();
			usart_2_print("Sleep Mode Ende\n");
		}
    }
}
