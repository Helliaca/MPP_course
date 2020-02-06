#include "main.h"

//#define zf 100

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

    init_ADC_EXVOL_TIM2();
    init_DAC_MOV_AVG();
    init_usart_2();

    //Die nachfolgende Variable zeitfenster gibt ein Vielfaches von 100us an.
    //D.h. die Anzahl an ADC-Wandlungen bevor gemittlet wird.
    zeitfenster = 100;

    /*
     * Timer TIM2 wird konfiguriert, um alle 100us ein UpdateEvent an ADC1 zu senden.
     * ADC1 tastet den angelegten Spannungswert am PIN PA0 ab. Der ADC löst dann ein
     * EndOFCOnversion (EOC) Interrupt aus, der in der ADC_IRQ_Handler Routine abgearbeitet
     * wird. Hier drin wird der abgetastete Spannungswert in den Ringbuffer Mittelwert_buffer
     * der Länge zeitfenster geschrieben. Daraufhin wird einml über alle Werte im Buffer
     * gemittelt und der Wert in das Outputregister das DAC geschrieben. Der DAC gibt dann
     * den Wert beim nächsten TIM2 TIck aus, da er als Triggerquelle auch den TIM2 hat.
     *
     * Gleichspannungsanteil 1V und Amplitude 0.5V
     * Ergebnis: DAC Ausgabe pendelt sich (wegen Mittelwertsberechnung) nach etwas Zeit auf ca. 1V
     */

    while(1) {

    }

}
