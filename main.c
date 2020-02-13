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

    init_usart_2();


    // Initialisierung des DW1000 Transceivers
    DW1000_init();
    // DW1000 in den Deep Sleep versetzen um Strom zu sparen
    dw1000_idle();
    dw1000_entersleep();
    //...
    // Aufruf der Funktion zur Positionsbestimmung
    // innerhalb der Funktion erfolgt nach der Messung
    // die Ausgabe der Entfernungen über die Serielle Schnittstelle

    while(1) {
    	wait_mSek(100);
    	position_mov_avg();
    }

}
