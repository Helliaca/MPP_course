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
    init_ADC1_EXVOL();
    //init_ADC1_EXVOL_BAT_TEMP();

    char buffer[50];
    float measured_val;
    int digitalwert;

    while(1) {

        //jeden Abschnitt in eine Unterfunktion eventuell

            wait_mSek(1000); //Konversiondauer beträgt 3* 1.4us ist also vernachlässigbar

            //externe Spannung
            ADC_SoftwareStartConv(ADC1);
            while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
            digitalwert=ADC_GetConversionValue(ADC1);
            measured_val = ((float) digitalwert/4095.0f) * 3.3f;      //2^12 = 4096 und V_ref=3.3V

            //dieser Abshcnitt könnte auch mit Analog Watchdog gemacht werden
            if(measured_val >= 1.4f) green_LED_ON;
            else green_LED_OFF;

            sprintf(buffer,"EXT_VOLT = %1.6f V \r\n",measured_val);
            usart_2_print(buffer);

            /*
            //Batteriespannung
            ADC_SoftwareStartConv(ADC1);
            while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
            digitalwert=ADC_GetConversionValue(ADC1);
            measured_val = 2.0f * ((float) digitalwert/4095.0f) * 3.3f; //wieso 2.0?
            sprintf(buffer,"VBAT = %6.3f V\r\n",measured_val);
            usart_2_print(buffer);

            //Temperatursensor
            ADC_SoftwareStartConv(ADC1);
            while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);

            digitalwert = ADC_GetConversionValue(ADC1);
            measured_val = (float)(digitalwert/4095.0f) * 3.3f;

            // aus dem Datenblatt stammen die zwei Werte:
            // V25 = 0,76Volt bei 25°C (Spannungswert des Sensors bei 25°C)
            // Average_Slope = 0,0025mV/°C (Spannungänderung pro Grad °C)
            // und die Berechnungsgleichung:
            // Temperatur = ((Spannungswert - V25)/Average_Slope) + 25) [in °C]
            measured_val = (float) ((measured_val - 0.76f)/0.0025f) + 25.0f;

            sprintf(buffer,"TEMP = %2.3f °C \r\n",measured_val);
            usart_2_print(buffer);
			*/
    }

}
