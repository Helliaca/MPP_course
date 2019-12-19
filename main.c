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
    usart_2_print(" \r\nSystem Neustart");

    /*
     * Kein stop modus:
     * 	- LED an: 64-65 mA,
     * 	- LED aus: 65-66 mA
     * Stop modus (LED immer an):
     * 21 mA
     * Dh. sleep mode 35mA - Stop Mode 21mA -> 14mA ist das Clocksystem das ausgeschaltet wird.
     * Von den übrigen ~21 mA sind ca. 19 Peripherieeinheiten und 2mA Flash speicher etc. (erwarteter Stromverbeauch beim Stop-Mode)
     *
     * Wenn das SystemInit() nicht eingefügt wird, läfut das Clocksystem auf 16MHz (HSI). Daher funktioniert die usart-Ausgabe nicht mehr
     * (falsche BAUD-Rate) und die LED blinkt ca. alle 30 Sekunden (168MHz vs. 16MHz)
     */
    while(1) {
		wait_mSek(3000);
		green_LED_Toggle;
		if(taste1_pressed()) {
			green_LED_ON;
			usart_2_print("\r\nSTOP Mode Start");

			PWR_FlashPowerDownCmd(ENABLE);
			PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
			PWR_FlashPowerDownCmd(DISABLE);

			SystemInit();

			usart_2_print("\r\nSTOP Mode Ende");
		}
    }
}
