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
    init_usart_2_irq();
    init_tasten();
    init_taste2_irq();
    init_tim5();

    messung_started = 0;
    uint32_t messungen[10] = {0};

    uint32_t delay;
    while(1) {
    	while(!messung_started) {}
    	usart_2_print("\r\nMessungen started");

    	//10 Messungen
    	int i;
    	for(i=0; i<10; i++) {
    		green_LED_OFF;
    		delay = get_random_int(2000, 10000);

			wait_mSek(delay);

			green_LED_ON;
			reactiontime = -1;


			// Counter auf 0 setzen
			TIM_SetCounter (TIM5, 0x00);
			// Timer TIM5 Freigeben
			TIM_Cmd(TIM5, ENABLE);
			// Messung läuft jetzt

			//Taster interrupt setzt reactiontime auf die vergangene Zeit.
			while(reactiontime < 0) {}
			TIM_Cmd(TIM5, DISABLE);
			messungen[i] = reactiontime;
    	}

    	uint32_t sum = 0;
    	uint32_t min = 4294967295;
    	uint32_t max = 0;

    	int j = 0;
    	for(j=0; j<10; j++) {
    	    sum += messungen[j];
    	    if(messungen[j]<min) min = messungen[j];
    	    if(messungen[j]>max) max = messungen[j];
    	}

    	char data[50] = {0};
    	sprintf(data, "\r\nMin: %1.4f s", (float) min * 0.0001);
    	usart_2_print(data);
    	sprintf(data, "\r\nMax: %1.4f s", (float) max * 0.0001);
    	usart_2_print(data);
    	sprintf(data, "\r\nAvg: %1.4f s", (float) sum/10 * 0.0001);
    	usart_2_print(data);

    	// Restart
    	messung_started = 0;
    	green_LED_OFF;
    }
}
