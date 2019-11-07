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
    //init_board();

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

    init_leds();
    init_tasten();

    green_LED_ON;

    int taste2_counter = 0;
    while (1)
    {
    	//Aufgabe 01.03
    	wait_mSek(500);
    	green_LED_Toggle;

    	//Aufgabe 01.07
    	//if(taste2_downed()) taste2_counter++;

    	//if(taste2_counter>1) {
    	//	taste2_counter = 0;
    	//	green_LED_Toggle;
    	//}

    	//if(taste1_downed()) green_LED_Toggle;

    	//Aufgabe 1.04
    	//wait_mSek(500);
    	//if(taste1_pressed()==1) green_LED_Toggle;
    	//green_LED_Toggle;
    }
}
