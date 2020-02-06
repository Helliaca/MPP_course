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



    /*
     * Die Sinus-Funktion soll
     *  * 1V   = U_0 Amplitude,
     *  * 1.5V = U_b Offset,
     *  * 50Hz = freq Frequenz
     *  besitzen. Bei einer Referenzspannung U_ref = 3.3V und einer 12 Bit Auflösung:
     *  * U_0 * 4096 / U_ref = 1241,21212121 ~ 1241 Digitalwert
     *  * U_b * 4096 / U_ref = 1861,81818181 ~ 1862 Digitalwert
     *  * 1/freq = 0,02s = period
     *
     *  Damit die softwareseitig laufende Sinus-Generator mit der richtigen Frequenz
     *  die Spannung ausgibt, muss das Value
     */
    int i=0;
    for(i=0; i<100; i++) {
        sine_values[i] = sin(2 * M_PI * i / 100);
    }

    init_DAC_sinewave();
    while(1) {

    }

}
