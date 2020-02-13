#include "main.h"

int main ( void )
{
    // Initialisierung des Systems und des Clocksystems
    SystemInit();

    // SysTick initialisieren
    // jede Millisekunde erfolgt dann der Aufruf
    // des Handlers fuer den Interrupt SysTick_IRQn
    InitSysTick();

    // blink_interval wird win SysTickHandler verwendet und
    // spezifiziert das Blink-Intervall in ms. -1 entspricht "Nicht Blinken".
    //blink_interval = -1;

    // Initialisierung aller Portleitungen und Schnittstellen
    // Freigabe von Interrupten
    // init_board();

    // Start der RTC  falls diese noch
    // nicht initialisiert war wird
    // die RTC mit der LSE-Taktquelle aktiviert
    start_RTC();

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
    //DW1000_init();

    init_tasten();
    init_taste2_irq();

    init_usart_2();
    init_BEEPER();

    // Initialisierung des DW1000 Transceivers
    // und anmeldung der Eventhandler
    DW1000_init();

    while(1) {
    	wait_mSek(500);

    	dw1000_idle();
		dw1000_entersleep();
    	float d = distanz(20);

    	if(d>2.0f) {
    		beep(200, 500, 0);
    	} else {
    		beep(80, 500, 0);
    	}
    }

}
