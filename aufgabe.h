#ifndef __aufgabe_h__
#define __aufgabe_h__

//#########################################################################
//########## cmsis_lib include
//#########################################################################
#include "stm32f4xx.h"
#include "misc.h"
#include "stm32f4xx_adc.h"
//#include "stm32f4xx_can.h"
//#include "stm32f4xx_crc.h"
//#include "stm32f4xx_cryp_aes.h"
//#include "stm32f4xx_cryp_des.h"
//#include "stm32f4xx_cryp_tdes.h"
//#include "stm32f4xx_cryp.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_dbgmcu.h"
//#include "stm32f4xx_dcmi.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_exti.h"
//#include "stm32f4xx_flash.h"
//#include "stm32f4xx_fsmc.h"
#include "stm32f4xx_gpio.h"
//#include "stm32f4xx_hash_md5.h"
//#include "stm32f4xx_hash_sha1.h"
//#include "stm32f4xx_hash.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_iwdg.h"
#include "stm32f4xx_pwr.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_rng.h"
#include "stm32f4xx_rtc.h"
#include "stm32f4xx_sdio.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_wwdg.h"


//#########################################################################
//########## mpp_lib include
//#########################################################################
#include "BME280.h"
//=========================================================================
#include "beeper.h"
#include "client_ftp.h"
#include "client_ntp.h"
#include "global.h"
#include "i2c.h"
#include "init.h"
#include "interrupts.h"
#include "led.h"
#include "power.h"
#include "rtc.h"
#include "taster.h"
#include "usart.h"
//=========================================================================
#include "simplelink.h"
#include "netapp.h"
#include "CC3100.h"
#include "CC3100_Board.h"
//=========================================================================
#include "dw1000_driver.h"
#include "dw1000_ranging.h"
#include "DW1000.h"
//=========================================================================
#include "MQTT.h"
//=========================================================================
#include "MPU9250.h"
//=========================================================================
#include "SDCARD.h"
//=========================================================================
//#include "usbd_cdc_vcp.h"
//#include "usb_conf.h"
//=========================================================================
#include "CoOS.h"
#include "mpu9250_driver.h"
#include "test.h"

//#########################################################################
//########## standard_lib include
//#########################################################################
#include "stdio.h"
#include "string.h"
#include "math.h"


//#########################################################################
//########## Eigene Funktionen, Macros und Variablen
//#########################################################################


//=========================================================================
// Macros
//=========================================================================

#define green_LED_ON        (GPIO_SetBits(GPIOB, GPIO_Pin_2))
#define green_LED_OFF       (GPIO_ResetBits(GPIOB, GPIO_Pin_2))
#define green_LED_Toggle    (GPIO_ToggleBits(GPIOB, GPIO_Pin_2))
#define PUFFER_SIZE			50


//=========================================================================
// Variablen
//=========================================================================



//=========================================================================
// Funktionen
//=========================================================================
void init_leds(void);
void init_tasten();
uint8_t taste1_pressed();
uint8_t taste2_pressed();
uint8_t taste1_downed();
uint8_t taste2_downed();
void init_PC09(void);
void RCC_WaitForPLLStartUp(void);
void defaultMode();
void slowMode(void);
void fastMode(void);
void init_usart_2_tx(void);
void usart_2_print(char * str_print);
void init_usart_2(void);
void init_iwdg(void);
void init_taste1_irq(void);
void init_taste2_irq(void);
void init_usart_2_irq(void);
void init_alarm1(void);
void init_alarm2(void);
void init_alarm_every_sec(int, int);
void init_RTC_wakeup_timer(uint16_t sec);
void init_tim7_irq();
void init_tim7_irq_2();
void init_tim3();


//=========================================================================
#endif
