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
    //init_leds();
    init_usart_2();
    init_tasten();
    init_taste1_irq();
    init_taste2_irq();

    /* Reference Manual für genaue Befehlsabfolge zum Enablen des Standby-Modus: Seite 105/1416. Unterpunkt 2.
     *
     * Stromverbrauch im Standbye-Modus: 17-18 mA verursacht im Wesentlichen durch Peripherieeinheiten
     * da der Stromverbauch des Chips nach Website 4uA ist.
     */

    init_tim7_irq_2();

    tim7_counter=-1;

    while(1) {
    	// Do things
    }
}
